  #include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <semaphore.h>
#include <signal.h>
#include <stdatomic.h>

#include "constants.h"
#include "parser.h"
#include "operations.h"
#include "io.h"
#include "pthread.h"
#include "src/common/constants.h"
#include "src/common/io.h"////////

struct SharedData {
  DIR* dir;
  char* dir_name;
  pthread_mutex_t directory_mutex;
};

typedef struct {
  int sessionx;
  int reqx;
  int respx;
  int notifx;
} Connection;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t n_current_backups_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t sessions_lock = PTHREAD_MUTEX_INITIALIZER;

sem_t semEscritor;
sem_t semLeitor;
pthread_mutex_t secCritica = PTHREAD_MUTEX_INITIALIZER;
char connectionsBuffer[MAX_SESSION_COUNT][MAX_PIPE_PATH_LENGTH * 3 + 1]; // Prod-Cons
int escPtr = 0;
int leiPtr = 0;

int sigusr1_received = 0;

size_t active_backups = 0;     // Number of active backups
size_t max_backups;            // Maximum allowed simultaneous backups
size_t max_threads;            // Maximum allowed simultaneous threads
char* jobs_directory = NULL;
char* reg_pipe_path = NULL;
Connection* connections[MAX_SESSION_COUNT];

void handle_sigusr1(int signum) {
  // Memorizar o sinal recebido
  if (signum == SIGUSR1) {
    sigusr1_received = 1;
  }  
}

void setup_signal_handler() {
  struct sigaction sa;
  sa.sa_handler = handle_sigusr1;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  if (sigaction(SIGUSR1, &sa, NULL) == -1) {
    perror("sigaction");
    exit(EXIT_FAILURE);
  }
}

int filter_job_files(const struct dirent* entry) {
  const char* dot = strrchr(entry->d_name, '.');
  if (dot != NULL && strcmp(dot, ".job") == 0) {
      return 1;  // Keep this file (it has the .job extension)
  }
  return 0;
}

static int entry_files(const char* dir, struct dirent* entry, char* in_path, char* out_path) {
  const char* dot = strrchr(entry->d_name, '.');
  if (dot == NULL || dot == entry->d_name || strlen(dot) != 4 || strcmp(dot, ".job")) {
    return 1;
  }

  if (strlen(entry->d_name) + strlen(dir) + 2 > MAX_JOB_FILE_NAME_SIZE) {
    fprintf(stderr, "%s/%s\n", dir, entry->d_name);
    return 1;
  }

  strcpy(in_path, dir);
  strcat(in_path, "/");
  strcat(in_path, entry->d_name);

  strcpy(out_path, in_path);
  strcpy(strrchr(out_path, '.'), ".out");

  return 0;
}

static int run_job(int in_fd, int out_fd, char* filename) {
  size_t file_backups = 0;
  while (1) {
    char keys[MAX_WRITE_SIZE][MAX_STRING_SIZE] = {0};
    char values[MAX_WRITE_SIZE][MAX_STRING_SIZE] = {0};
    unsigned int delay;
    size_t num_pairs;

    switch (get_next(in_fd)) {
      case CMD_WRITE:
        num_pairs = parse_write(in_fd, keys, values, MAX_WRITE_SIZE, MAX_STRING_SIZE);
        if (num_pairs == 0) {
          write_str(STDERR_FILENO, "Invalid command. See HELP for usage\n");
          continue;
        }

        if (kvs_write(num_pairs, keys, values)) {
          write_str(STDERR_FILENO, "Failed to write pair\n");
        }
        break;

      case CMD_READ:
        num_pairs = parse_read_delete(in_fd, keys, MAX_WRITE_SIZE, MAX_STRING_SIZE);

        if (num_pairs == 0) {
          write_str(STDERR_FILENO, "Invalid command. See HELP for usage\n");
          continue;
        }

        if (kvs_read(num_pairs, keys, out_fd)) {
          write_str(STDERR_FILENO, "Failed to read pair\n");
        }
        break;

      case CMD_DELETE:
        num_pairs = parse_read_delete(in_fd, keys, MAX_WRITE_SIZE, MAX_STRING_SIZE);

        if (num_pairs == 0) {
          write_str(STDERR_FILENO, "Invalid command. See HELP for usage\n");
          continue;
        }

        if (kvs_delete(num_pairs, keys, out_fd)) {
          write_str(STDERR_FILENO, "Failed to delete pair\n");
        }
        break;

      case CMD_SHOW:
        kvs_show(out_fd);
        break;

      case CMD_WAIT:
        if (parse_wait(in_fd, &delay, NULL) == -1) {
          write_str(STDERR_FILENO, "Invalid command. See HELP for usage\n");
          continue;
        }

        if (delay > 0) {
          printf("Waiting %d seconds\n", delay / 1000);
          kvs_wait(delay);
        }
        break;

      case CMD_BACKUP:
        pthread_mutex_lock(&n_current_backups_lock);
        if (active_backups >= max_backups) {
          wait(NULL);
        } else {
          active_backups++;
        }
        pthread_mutex_unlock(&n_current_backups_lock);
        int aux = kvs_backup(++file_backups, filename, jobs_directory);

        if (aux < 0) {
            write_str(STDERR_FILENO, "Failed to do backup\n");
        } else if (aux == 1) {
          return 1;
        }
        break;

      case CMD_INVALID:
        write_str(STDERR_FILENO, "Invalid command. See HELP for usage\n");
        break;

      case CMD_HELP:
        write_str(STDOUT_FILENO,
            "Available commands:\n"
            "  WRITE [(key,value)(key2,value2),...]\n"
            "  READ [key,key2,...]\n"
            "  DELETE [key,key2,...]\n"
            "  SHOW\n"
            "  WAIT <delay_ms>\n"
            "  BACKUP\n" // Not implemented
            "  HELP\n");

        break;

      case CMD_EMPTY:
        break;

      case EOC:
        printf("EOF\n");
        return 0;
    }
  }
}

//frees arguments
static void* get_file(void* arguments) {
  struct SharedData* thread_data = (struct SharedData*) arguments;
  DIR* dir = thread_data->dir;
  char* dir_name = thread_data->dir_name;

  sigset_t sigset;
  sigemptyset(&sigset);
  sigaddset(&sigset, SIGUSR1);
  pthread_sigmask(SIG_BLOCK, &sigset, NULL);

  if (pthread_mutex_lock(&thread_data->directory_mutex) != 0) {
    fprintf(stderr, "Thread failed to lock directory_mutex\n");
    return NULL;
  }

  struct dirent* entry;
  char in_path[MAX_JOB_FILE_NAME_SIZE], out_path[MAX_JOB_FILE_NAME_SIZE];
  while ((entry = readdir(dir)) != NULL) {
    if (entry_files(dir_name, entry, in_path, out_path)) {
      continue;
    }

    if (pthread_mutex_unlock(&thread_data->directory_mutex) != 0) {
      fprintf(stderr, "Thread failed to unlock directory_mutex\n");
      return NULL;
    }

    int in_fd = open(in_path, O_RDONLY);
    if (in_fd == -1) {
      write_str(STDERR_FILENO, "Failed to open input file: ");
      write_str(STDERR_FILENO, in_path);
      write_str(STDERR_FILENO, "\n");
      pthread_exit(NULL);
    }

    int out_fd = open(out_path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (out_fd == -1) {
      write_str(STDERR_FILENO, "Failed to open output file: ");
      write_str(STDERR_FILENO, out_path);
      write_str(STDERR_FILENO, "\n");
      pthread_exit(NULL);
    }

    int out = run_job(in_fd, out_fd, entry->d_name);

    close(in_fd);
    close(out_fd);

    if (out) {
      if (closedir(dir) == -1) {
        fprintf(stderr, "Failed to close directory\n");
        return 0;
      }

      exit(0);
    }

    if (pthread_mutex_lock(&thread_data->directory_mutex) != 0) {
      fprintf(stderr, "Thread failed to lock directory_mutex\n");
      return NULL;
    }
  }

  if (pthread_mutex_unlock(&thread_data->directory_mutex) != 0) {
    fprintf(stderr, "Thread failed to unlock directory_mutex\n");
    return NULL;
  }

  pthread_exit(NULL);
}

void insertConnection(char buffer[MAX_PIPE_PATH_LENGTH * 3 + 1]) {
  if (sem_wait(&semEscritor)) {
    fprintf(stderr, "Failed at sam_wait of semEscritor\n");
    return;
  }
  pthread_mutex_lock(&secCritica);

  memcpy(connectionsBuffer[escPtr], buffer, MAX_PIPE_PATH_LENGTH * 3 + 1);
  escPtr = (escPtr + 1) % MAX_SESSION_COUNT;

  pthread_mutex_unlock(&secCritica);
  if (sem_post(&semLeitor)) {
    fprintf(stderr, "Failed at sam_post of semLeitor\n");
    return;
  }
}

void removeConnection(char* buffer) {
    if (sem_wait(&semLeitor)) {
      fprintf(stderr, "Failed at sam_wait of semLeitor\n");
      return;
    }
    pthread_mutex_lock(&secCritica);

    memcpy(buffer, connectionsBuffer[leiPtr], MAX_PIPE_PATH_LENGTH * 3 + 1);
    leiPtr = (leiPtr + 1) % MAX_SESSION_COUNT;

    pthread_mutex_unlock(&secCritica);

    if (sem_post(&semEscritor)) {
      fprintf(stderr, "Failed at sam_post of semEscritor\n");
      return;
    }
}

void client_handler(Connection *cli_connection) {
  while(1) {
    char op_code;
    char request[MAX_PIPE_PATH_LENGTH + 1] = {0};
    char key[MAX_STRING_SIZE] = {0};
    if (read_all(cli_connection->reqx, request, sizeof(request), NULL) < 0) {
      if (errno == EPIPE) {
        fprintf(stderr, "[ERROR]: Pipe closed. Client disconnected.\n");
        continue; // Esperar por um novo cliente
      }
      fprintf(stderr, "[ERROR]: Failed to read data\n");
      return;
    }

    switch (op_code = request[0]) {
      case '2':
        if (kvs_cli_disconnect(cli_connection->notifx) == 0) {
          if (write_all(cli_connection->respx, "21", 3) < 0) { // ERRO
            fprintf(stderr, "[ERR]: write_all failed\n");
          }
        }
        else {
          if (write_all(cli_connection->respx, "20", 3) < 0) { // SUCESSO
            fprintf(stderr, "[ERR]: write_all failed\n");
          }
        }
        close(cli_connection->reqx);
        close(cli_connection->respx);
        close(cli_connection->notifx);
        return;
      
      case '3':
        memcpy(key, &request[1], MAX_STRING_SIZE);  // Nome do pipe de pedidos
        key[39] = '\0';
        if (kvs_subscribe(key, cli_connection->notifx) == 0) {
          if (write_all(cli_connection->respx, "30", 3) < 0) { // Chave não existe
            fprintf(stderr, "[ERR]: write_all failed\n");
          }
        }
        else {
          if (write_all(cli_connection->respx, "31", 3) < 0) { // Existe
            fprintf(stderr, "[ERR]: write_all failed\n");
          }
        }
        break;

      case '4':
        memcpy(key, &request[1], MAX_STRING_SIZE);  // Nome do pipe de pedidos
        key[39] = '\0';
        if (kvs_unsubscribe(key, cli_connection->notifx) == 0) {
          if (write_all(cli_connection->respx, "40", 3) < 0) { // Chave não existe
            fprintf(stderr, "[ERR]: write_all failed\n");
          }
        }
        else {
          if (write_all(cli_connection->respx, "41", 3) < 0) { // Existe
            fprintf(stderr, "[ERR]: write_all failed\n");
          }
        }
        break;
          
      default:
        break;
    }
  }
}

static void* sessions_managers() {
  sigset_t sigset;
  sigemptyset(&sigset);
  sigaddset(&sigset, SIGUSR1);
  pthread_sigmask(SIG_BLOCK, &sigset, NULL);

  while (1) {
    char buffer[MAX_PIPE_PATH_LENGTH * 3 + 1];
    removeConnection(buffer);

    if (buffer == NULL) {
      fprintf(stderr, "[ERROR]: removeConnection failed\n");
      continue; // Tenta novamente na próxima iteração
    }
  
    if (buffer[0] != '1') {
      fprintf(stderr, "Pedido de inicio de sessão invalido\n");
      continue;
    }

    char req_pipe_path[MAX_PIPE_PATH_LENGTH] = {0};
    char resp_pipe_path[MAX_PIPE_PATH_LENGTH] = {0};
    char notif_pipe_path[MAX_PIPE_PATH_LENGTH] = {0};
    memcpy(req_pipe_path, &buffer[1], sizeof(req_pipe_path));  // Nome do pipe de pedidos
    memcpy(resp_pipe_path, &buffer[1 + MAX_PIPE_PATH_LENGTH], sizeof(resp_pipe_path));  // Nome do pipe de respostas
    memcpy(notif_pipe_path, &buffer[1 + 2 * MAX_PIPE_PATH_LENGTH], sizeof(notif_pipe_path));  // Nome do pipe de notificações

    // Garantir que os nomes terminam com '\0'
    req_pipe_path[39] = '\0';
    resp_pipe_path[39] = '\0';
    notif_pipe_path[39] = '\0';

    Connection* cli_connection = malloc(sizeof(Connection));
    if (cli_connection == NULL) {
      fprintf(stderr, "[ERROR]: Failed to allocate memory for cli_connection\n");
      return NULL;
    }

    cli_connection->reqx = open(req_pipe_path, O_RDONLY);
    if (cli_connection->reqx == -1) {
      perror("[ERR]: open failed");
      free(cli_connection);
      return NULL;;
    }
    cli_connection->respx = open(resp_pipe_path, O_WRONLY);
    if (cli_connection->respx == -1) {
      perror("[ERR]: open failed");
      close(cli_connection->reqx);
      free(cli_connection);
      return NULL;
    }
    cli_connection->notifx = open(notif_pipe_path, O_WRONLY);
    if (cli_connection->notifx == -1) {
      perror("[ERR]: open failed");
      close(cli_connection->reqx);
      close(cli_connection->respx);
      free(cli_connection);
      return NULL;
    }

    if (write_all(cli_connection->respx, "10", 3) < 0) {
      fprintf(stderr, "[ERR]: write_all failed\n");
      close(cli_connection->reqx);
      close(cli_connection->respx);
      close(cli_connection->notifx);
      free(cli_connection);
      return NULL;
    }

    pthread_mutex_lock(&sessions_lock);
    for (int i = 0; i < MAX_SESSION_COUNT; i++) {
      if (connections[i] == NULL) {
        connections[i] = cli_connection;
        break;
      }
    }
    pthread_mutex_unlock(&sessions_lock);

    client_handler(cli_connection);

    pthread_mutex_lock(&sessions_lock);
    for (int i = 0; i < MAX_SESSION_COUNT; i++) {
      if (connections[i] == cli_connection) {
        connections[i] = NULL;
        break;
      }
    }
    pthread_mutex_unlock(&sessions_lock);

    free(cli_connection);
  }
  pthread_exit(NULL);
}


static void* sessions_host() {
  /**
   * open pipe for reading
   * this waits for someone to open it for writing
   */
  while (1) {
    if (sigusr1_received == 1) {
      if (kvs_rm_subscriptions() != 1) {;// Eliminar todas as subscrições da hashtable
        fprintf(stderr, "[ERROR]: Failed to remove subscriptions\n");
      }
      pthread_mutex_lock(&sessions_lock);
      for (int i = 0; i < MAX_SESSION_COUNT; i++) { // Fechar todos os FIFOs dos clientes
        if (connections[i] != NULL) {
          close(connections[i]->reqx);
          close(connections[i]->respx);
          close(connections[i]->notifx);
          connections[i] = NULL;
          free(connections[i]);
        }
      }
      pthread_mutex_unlock(&sessions_lock);

      sigusr1_received = 0;
    }

    int new_sessionx = open(reg_pipe_path, O_RDONLY);
    if (new_sessionx == -1) {
      perror("[ERR]: open failed");
      pthread_exit(NULL);
    }
    
    char buffer[MAX_PIPE_PATH_LENGTH * 3 + 1] = {0};
    if (read_all(new_sessionx, buffer, sizeof(buffer), NULL) < 0) {
      fprintf(stderr, "[ERROR]: Failed to read data\n");
      return NULL;
    }
    close(new_sessionx);

    insertConnection(buffer);
  }

  pthread_exit(NULL);
}

static void dispatch_threads(DIR* dir) {
  pthread_t* threads = malloc(max_threads * sizeof(pthread_t));

  if (threads == NULL) {
    fprintf(stderr, "Failed to allocate memory for threads\n");
    return;
  }

  struct SharedData thread_data = {dir, jobs_directory, PTHREAD_MUTEX_INITIALIZER};


  for (size_t i = 0; i < max_threads; i++) {
    if (pthread_create(&threads[i], NULL, get_file, (void*)&thread_data) != 0) {
      fprintf(stderr, "Failed to create thread %zu\n", i);
      pthread_mutex_destroy(&thread_data.directory_mutex);
      free(threads);
      return;
    }
  }

  // ler do FIFO de registo//////////////////////////////////////////////////////////////////////////////
  
  if (sem_init(&semEscritor, 0, MAX_SESSION_COUNT) == -1) {
    fprintf(stderr, "Failed to inicialize semaphore\n");
    return;
  }
  if (sem_init(&semLeitor, 0, 0) == -1) {
    fprintf(stderr, "Failed to inicialize semaphore\n");
    return;
  }
  
    /* remove pipe if it exists */
  if (unlink(reg_pipe_path) != 0 && errno != ENOENT) {
    perror("[ERR]: unlink(%s) failed");
    return;
  }

    /* create pipe */
  if (mkfifo(reg_pipe_path, 0640) != 0) {
    perror("[ERR]: mkfifo failed");
    return;
  }

  pthread_t host_thread;
  if (pthread_create(&host_thread, NULL, sessions_host, NULL) != 0) {
    fprintf(stderr, "Failed to create host thread\n");
    pthread_mutex_destroy(&thread_data.directory_mutex);
    return;
  }

  pthread_t* manager_threads = malloc(MAX_SESSION_COUNT * sizeof(pthread_t));
  for (size_t i = 0; i < MAX_SESSION_COUNT; i++) {
    if (pthread_create(&manager_threads[i], NULL, sessions_managers, NULL) != 0) {
        fprintf(stderr, "Failed to create thread %zu\n", i);
        free(manager_threads);
        return;
    }
}

  ///////////////////////////////////////////////////////////////////////////////////////////////////////
  for (unsigned int i = 0; i < max_threads; i++) {
    if (pthread_join(threads[i], NULL) != 0) {
      fprintf(stderr, "Failed to join thread %u\n", i);
      pthread_mutex_destroy(&thread_data.directory_mutex);
      free(threads);
      return;
    }
  }

  if (pthread_join(host_thread, NULL) != 0) {
    fprintf(stderr, "Failed to join host thread\n");
    pthread_mutex_destroy(&thread_data.directory_mutex);
    return;
  }

  for (unsigned int i = 0; i < MAX_SESSION_COUNT; i++) {
    if (pthread_join(manager_threads[i], NULL) != 0) {
      fprintf(stderr, "Failed to join thread %u\n", i);
      pthread_mutex_destroy(&thread_data.directory_mutex);
      free(manager_threads);
      return;
    }
  }

  if (pthread_mutex_destroy(&thread_data.directory_mutex) != 0) {
    fprintf(stderr, "Failed to destroy directory_mutex\n");
  }

  free(threads);
  free(manager_threads);
}


int main(int argc, char** argv) {
  if (argc < 5) {
    write_str(STDERR_FILENO, "Usage: ");
    write_str(STDERR_FILENO, argv[0]);
    write_str(STDERR_FILENO, " <jobs_dir>");
		write_str(STDERR_FILENO, " <max_threads>");
		write_str(STDERR_FILENO, " <max_backups> \n");
    write_str(STDERR_FILENO, " <reg_pipe_path> \n");/////////////
    return 1;
  }

  jobs_directory = argv[1];

  char* endptr;
  max_backups = strtoul(argv[3], &endptr, 10);

  if (*endptr != '\0') {
    fprintf(stderr, "Invalid max_proc value\n");
    return 1;
  }

  max_threads = strtoul(argv[2], &endptr, 10);

  if (*endptr != '\0') {
    fprintf(stderr, "Invalid max_threads value\n");
    return 1;
  }

	if (max_backups <= 0) {
		write_str(STDERR_FILENO, "Invalid number of backups\n");
		return 0;
	}

	if (max_threads <= 0) {
		write_str(STDERR_FILENO, "Invalid number of threads\n");
		return 0;
	}

  reg_pipe_path = argv[4];////

  setup_signal_handler();

  if (kvs_init()) {
    write_str(STDERR_FILENO, "Failed to initialize KVS\n");
    return 1;
  }

  

  DIR* dir = opendir(argv[1]);
  if (dir == NULL) {
    fprintf(stderr, "Failed to open directory: %s\n", argv[1]);
    return 0;
  }

  dispatch_threads(dir);

  if (closedir(dir) == -1) {
    fprintf(stderr, "Failed to close directory\n");
    return 0;
  }

  while (active_backups > 0) {
    wait(NULL);
    active_backups--;
  }

  kvs_terminate();

  return 0;
}
