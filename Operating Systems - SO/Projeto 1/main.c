#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/wait.h>
#include <pthread.h>

#include "constants.h"
#include "parser.h"
#include "operations.h"

pthread_mutex_t kvs_mutex = PTHREAD_MUTEX_INITIALIZER;

static int active_backup_threads = 0;
//static int active_threads = 0;
int max_backups_threads;
int max_threads;


typedef struct {
    char directory[MAX_PATH_SIZE];
    DIR *dir;
} ThreadArgs;

typedef struct {
    char filepath[MAX_PATH_SIZE];
    char filepath_job[1024];
    char filepath_out[MAX_PATH_SIZE+4];
} JobArgs;

void* process_job(void* args) { 

    ThreadArgs* threadArgs = (ThreadArgs *)args;
    struct dirent *entry;
    
    while (1) {
        pthread_mutex_lock(&kvs_mutex);
        entry = readdir(threadArgs->dir);
        pthread_mutex_unlock(&kvs_mutex);
        if (entry == NULL) {
          free(threadArgs);
          pthread_exit(NULL);
          break;
        }
        if (strstr(entry->d_name, ".job") != NULL) {
            JobArgs* jobArgs = malloc(sizeof(JobArgs));
            snprintf(jobArgs->filepath_job, sizeof(jobArgs->filepath_job), "%s/%s", threadArgs->directory, entry->d_name);
            int fdi = open(jobArgs->filepath_job, O_RDONLY);
            if (fdi < 0) {
              perror("Error opening input file");
              free(jobArgs);
              free(threadArgs);
              pthread_exit(NULL);
            }

            strcpy(jobArgs->filepath, jobArgs->filepath_job);
            char *dot = strrchr(jobArgs->filepath, '.');
            *dot = '\0';

            snprintf(jobArgs->filepath_out, sizeof(jobArgs->filepath_out), "%s.out", jobArgs->filepath);
            int fdo = open(jobArgs->filepath_out, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
            if (fdo < 0) {
              perror("Error opening output file");
              close(fdi);
              free(jobArgs);
              free(threadArgs);
              pthread_exit(NULL);
            }

            int backup_counter = 0;
            int x = 0;
            while (x != EOC) {
              char keys[MAX_WRITE_SIZE][MAX_STRING_SIZE] = {0};
              char values[MAX_WRITE_SIZE][MAX_STRING_SIZE] = {0};
              unsigned int delay;
              size_t num_pairs;

              //printf("> ");
              fflush(stdout);

              switch (get_next(fdi)) {
                case CMD_WRITE:
                  num_pairs = parse_write(fdi, keys, values, MAX_WRITE_SIZE, MAX_STRING_SIZE);
                  if (num_pairs == 0) {
                    fprintf(stderr, "Invalid command. See HELP for usage\n");
                    continue;
                  }
                  pthread_mutex_lock(&kvs_mutex);
                  if (kvs_write(num_pairs, keys, values)) {
                    fprintf(stderr, "Failed to write pair\n");
                  }
                  pthread_mutex_unlock(&kvs_mutex);
                  break;

                case CMD_READ:
                  num_pairs = parse_read_delete(fdi, keys, MAX_WRITE_SIZE, MAX_STRING_SIZE);

                  if (num_pairs == 0) {
                    fprintf(stderr, "Invalid command. See HELP for usage\n");
                    continue;
                  }
                  pthread_mutex_lock(&kvs_mutex);
                  if (kvs_read(fdo, num_pairs, keys)) {
                    fprintf(stderr, "Failed to read pair\n");
                  }
                  pthread_mutex_unlock(&kvs_mutex);
                  break;

                case CMD_DELETE:
                  num_pairs = parse_read_delete(fdi, keys, MAX_WRITE_SIZE, MAX_STRING_SIZE);

                  if (num_pairs == 0) {
                    fprintf(stderr, "Invalid command. See HELP for usage\n");
                    continue;
                  }
                  pthread_mutex_lock(&kvs_mutex);
                  if (kvs_delete(fdo, num_pairs, keys)) {
                    fprintf(stderr, "Failed to delete pair\n");
                  }
                  pthread_mutex_unlock(&kvs_mutex);
                  break;

                case CMD_SHOW:
                  pthread_mutex_lock(&kvs_mutex);
                  kvs_show(fdo);
                  pthread_mutex_unlock(&kvs_mutex);
                  break;

                case CMD_WAIT:
                  if (parse_wait(fdi, &delay, NULL) == -1) {
                    fprintf(stderr, "Invalid command. See HELP for usage\n");
                    continue;
                  }

                  if (delay > 0) {
                    write(fdo, "Waiting...\n", 11);
                    kvs_wait(delay);
                  }
                  break;

                case CMD_BACKUP:
                  pthread_mutex_lock(&kvs_mutex);
                  if (active_backup_threads >= max_backups_threads) {
                    wait(NULL);
                    active_backup_threads--;
                  }
                  pthread_mutex_unlock(&kvs_mutex);  
                  pid_t pid = fork();
                  if (pid < 0) {
                      fprintf(stderr, "Erro na chamada de sistema fork! \n");
                      break;
                  }
                  else if (pid == 0) {
                    pthread_mutex_lock(&kvs_mutex);
                    if (kvs_backup(jobArgs->filepath, backup_counter, active_backup_threads)) {
                      fprintf(stderr, "Failed to perform backup.\n");
                    }
                    exit(0);
                    pthread_mutex_unlock(&kvs_mutex);  
                    break;
                  }
                  else {
                    pthread_mutex_lock(&kvs_mutex);
                    active_backup_threads++;
                    backup_counter++;
                    pthread_mutex_unlock(&kvs_mutex);  
                    break;
                  }

                case CMD_INVALID:
                  fprintf(stderr, "Invalid command. See HELP for usage\n");
                  break;

                case CMD_HELP:
                  printf( 
                      "Available commands:\n"
                      "  WRITE [(key,value)(key2,value2),...]\n"
                      "  READ [key,key2,...]\n"
                      "  DELETE [key,key2,...]\n"
                      "  SHOW\n"
                      "  WAIT <delay_ms>\n"
                      "  BACKUP\n"
                      "  HELP\n"
                  );

                  break;
                  
                case CMD_EMPTY:
                  break;

                case EOC:
                  x = EOC;
                  break;
              }
            }
            close(fdi);
            close(fdo);
            free(jobArgs);
        }
  }  
  free(threadArgs);
}  

  
int main(int argc, char* argv[]) {

  if (argc != 4) {
    fprintf(stderr, "Usage: %s <directory> <max_backups_threads> <max_threads>\n", argv[0]);
    return 1;
  } 

  max_backups_threads = atoi(argv[2]);
  max_threads = atoi(argv[3]);

  if (kvs_init()) {
    fprintf(stderr, "Failed to initialize KVS\n");
    return 1;
  }

  const char *directory = argv[1];
  DIR *dir = opendir(directory);
  if (dir == NULL) {
        perror("Failed to open directory");
        return 1;
  }

  pthread_t threads[max_threads];

  for (int i = 0; i < max_threads; i++) {
        ThreadArgs* args = malloc(sizeof(JobArgs));
        snprintf(args->directory, sizeof(args->directory), "%s", directory);
        args->dir = dir;
        pthread_create(&threads[i], NULL, process_job, args);
  }
     
  for (int i = 0; i < max_threads; i++) {
        pthread_join(threads[i], NULL);
  }
  
  if (closedir(dir) != 0) {
        perror("Failed to close directory");
  }
  kvs_terminate();
  return 0;
}