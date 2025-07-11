#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "constants.h"
#include "io.h"
#include "kvs.h"
#include "operations.h"
#include "src/common/io.h" //
#include "src/common/constants.h" //

static struct HashTable *kvs_table = NULL;

/// Calculates a timespec from a delay in milliseconds.
/// @param delay_ms Delay in milliseconds.
/// @return Timespec with the given delay.
static struct timespec delay_to_timespec(unsigned int delay_ms) {
  return (struct timespec){delay_ms / 1000, (delay_ms % 1000) * 1000000};
}

int kvs_init() {
  if (kvs_table != NULL) {
    fprintf(stderr, "KVS state has already been initialized\n");
    return 1;
  }

  kvs_table = create_hash_table();
  return kvs_table == NULL;
}

int kvs_terminate() {
  if (kvs_table == NULL) {
    fprintf(stderr, "KVS state must be initialized\n");
    return 1;
  }

  free_table(kvs_table);
  kvs_table = NULL;
  return 0;
}

int kvs_write(size_t num_pairs, char keys[][MAX_STRING_SIZE],
              char values[][MAX_STRING_SIZE]) {
  if (kvs_table == NULL) {
    fprintf(stderr, "KVS state must be initialized\n");
    return 1;
  }

  pthread_rwlock_wrlock(&kvs_table->tablelock);

  for (size_t i = 0; i < num_pairs; i++) {
    int notifxs[MAX_NUMBER_SUB] = {0};
    for (int k = 0; k < MAX_NUMBER_SUB; k++) {
      notifxs[k] = -1; // Inicializa com -1, por exemplo
    }
    if (write_pair(kvs_table, keys[i], values[i], notifxs) != 0) {
      fprintf(stderr, "Failed to write key pair (%s,%s)\n", keys[i], values[i]);
    }
    for (int j = 0; j < MAX_NUMBER_SUB; j++) {
      if (notifxs[j] != -1) {
        char notification_key[MAX_STRING_SIZE + 1] = {0};
        char notification_value[MAX_STRING_SIZE + 1] = {0};

        snprintf(notification_key, MAX_STRING_SIZE + 1, "<%s>", keys[i]);
        snprintf(notification_value, MAX_STRING_SIZE + 1, "<%s>", values[i]);

        adjust_string(notification_key, MAX_STRING_SIZE + 1);
        adjust_string(notification_value, MAX_STRING_SIZE + 1);

        if (write_all(notifxs[j], notification_key, sizeof(notification_key)) < 0) {
            fprintf(stderr, "[ERR]: write_all failed\n");
        }
        if (write_all(notifxs[j], notification_value, sizeof(notification_value)) < 0) {
            fprintf(stderr, "[ERR]: write_all failed\n");
        }
      }
    }  
  }

  pthread_rwlock_unlock(&kvs_table->tablelock);
  return 0;
}

int kvs_read(size_t num_pairs, char keys[][MAX_STRING_SIZE], int fd) {
  if (kvs_table == NULL) {
    fprintf(stderr, "KVS state must be initialized\n");
    return 1;
  }
  
  pthread_rwlock_rdlock(&kvs_table->tablelock);

  write_str(fd, "[");
  for (size_t i = 0; i < num_pairs; i++) {
    char *result = read_pair(kvs_table, keys[i]);
    char aux[MAX_STRING_SIZE];
    if (result == NULL) {
      snprintf(aux, MAX_STRING_SIZE, "(%s,KVSERROR)", keys[i]);
    } else {
      snprintf(aux, MAX_STRING_SIZE, "(%s,%s)", keys[i], result);
    }
    write_str(fd, aux);
    free(result);
  }
  write_str(fd, "]\n");
  
  pthread_rwlock_unlock(&kvs_table->tablelock);
  return 0;
}

int kvs_delete(size_t num_pairs, char keys[][MAX_STRING_SIZE], int fd) {
  if (kvs_table == NULL) {
    fprintf(stderr, "KVS state must be initialized\n");
    return 1;
  }

  pthread_rwlock_wrlock(&kvs_table->tablelock);

  int aux = 0;
  for (size_t i = 0; i < num_pairs; i++) {
    int notifxs[MAX_NUMBER_SUB] = {0};
    for (int k = 0; k < MAX_NUMBER_SUB; k++) {
      notifxs[k] = -1; // Inicializa com -1, por exemplo
    }
    if (delete_pair(kvs_table, keys[i], notifxs) != 0) {
      if (!aux) {
        write_str(fd, "[");
        aux = 1;
      }
      char str[MAX_STRING_SIZE];
      snprintf(str, MAX_STRING_SIZE, "(%s,KVSMISSING)", keys[i]);
      write_str(fd, str);
    }
    for (int j = 0; j < MAX_NUMBER_SUB; j++) {
      if (notifxs[j] != -1) {
        char notification_key[MAX_STRING_SIZE + 1] = {0};
        char notification_delete[MAX_STRING_SIZE + 1] = {0};
        snprintf(notification_key, MAX_STRING_SIZE + 1, "<%s>", keys[i]);
        snprintf(notification_delete, MAX_STRING_SIZE + 1, "DELETED");
        adjust_string(notification_key, MAX_STRING_SIZE + 1);
        adjust_string(notification_delete, MAX_STRING_SIZE + 1);
        if (write_all(notifxs[j], notification_key, sizeof(notification_key)) < 0) {
            fprintf(stderr, "[ERR]: write_all failed\n");
        }
        if (write_all(notifxs[j], notification_delete, sizeof(notification_delete)) < 0) {
            fprintf(stderr, "[ERR]: write_all failed\n");
        }
      }
    }    
  }
  if (aux) {
    write_str(fd, "]\n");
  }

  pthread_rwlock_unlock(&kvs_table->tablelock);
  return 0;
}

void kvs_show(int fd) {
  if (kvs_table == NULL) {
    fprintf(stderr, "KVS state must be initialized\n");
    return;
  }
  
  pthread_rwlock_rdlock(&kvs_table->tablelock);
  char aux[MAX_STRING_SIZE];
  
  for (int i = 0; i < TABLE_SIZE; i++) {
    KeyNode *keyNode = kvs_table->table[i]; // Get the next list head
    while (keyNode != NULL) {
      snprintf(aux, MAX_STRING_SIZE, "(%s, %s)\n", keyNode->key, keyNode->value);
      write_str(fd, aux);
      keyNode = keyNode->next; // Move to the next node of the list
    }
  }

  pthread_rwlock_unlock(&kvs_table->tablelock);
}

int kvs_backup(size_t num_backup,char* job_filename , char* directory) {
  pid_t pid;
  char bck_name[50];
  snprintf(bck_name, sizeof(bck_name), "%s/%s-%ld.bck", directory, strtok(job_filename, "."),
           num_backup);

  pthread_rwlock_rdlock(&kvs_table->tablelock);
  pid = fork();
  pthread_rwlock_unlock(&kvs_table->tablelock);
  if (pid == 0) {
    // functions used here have to be async signal safe, since this
    // fork happens in a multi thread context (see man fork)
    int fd = open(bck_name, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    for (int i = 0; i < TABLE_SIZE; i++) {
      KeyNode *keyNode = kvs_table->table[i]; // Get the next list head
      while (keyNode != NULL) {
        char aux[MAX_STRING_SIZE];
        aux[0] = '(';
        size_t num_bytes_copied = 1; // the "("
        // the - 1 are all to leave space for the '/0'
        num_bytes_copied += strn_memcpy(aux + num_bytes_copied,
                                        keyNode->key, MAX_STRING_SIZE - num_bytes_copied - 1);
        num_bytes_copied += strn_memcpy(aux + num_bytes_copied,
                                        ", ", MAX_STRING_SIZE - num_bytes_copied - 1);
        num_bytes_copied += strn_memcpy(aux + num_bytes_copied,
                                        keyNode->value, MAX_STRING_SIZE - num_bytes_copied - 1);
        num_bytes_copied += strn_memcpy(aux + num_bytes_copied,
                                        ")\n", MAX_STRING_SIZE - num_bytes_copied - 1);
        aux[num_bytes_copied] = '\0';
        write_str(fd, aux);
        keyNode = keyNode->next; // Move to the next node of the list
      }
    }
    exit(1);
  } else if (pid < 0) {
    return -1;
  }
  return 0;
}

void kvs_wait(unsigned int delay_ms) {
  struct timespec delay = delay_to_timespec(delay_ms);
  nanosleep(&delay, NULL);
}

int kvs_subscribe(char key[MAX_STRING_SIZE], int notifx) {
  if (kvs_table == NULL) {
    fprintf(stderr, "KVS state must be initialized\n");
    return 0;
  }

  pthread_rwlock_wrlock(&kvs_table->tablelock);

  if (add_subscriber(kvs_table, key, notifx) != 0) {
    pthread_rwlock_unlock(&kvs_table->tablelock);
    return 0;
  }
  pthread_rwlock_unlock(&kvs_table->tablelock);

  return 1;
}

int kvs_unsubscribe(char key[MAX_STRING_SIZE], int notifx) {
  if (kvs_table == NULL) {
    fprintf(stderr, "KVS state must be initialized\n");
    return 0;
  }
  
  pthread_rwlock_rdlock(&kvs_table->tablelock);

  
  char *result = read_pair(kvs_table, key); //rever
  if (result == NULL) {
    pthread_rwlock_unlock(&kvs_table->tablelock);
    return 0;
  }

  free(result);
  
  pthread_rwlock_unlock(&kvs_table->tablelock);

  pthread_rwlock_wrlock(&kvs_table->tablelock);
  rm_subscriber(kvs_table, key, notifx);
  pthread_rwlock_unlock(&kvs_table->tablelock);
  return 1;
}

int kvs_cli_disconnect(int notifx) {
  if (kvs_table == NULL) {
    fprintf(stderr, "KVS state must be initialized\n");
    return 0;
  }

  pthread_rwlock_wrlock(&kvs_table->tablelock);
  rm_cli_subscriptions(kvs_table, notifx);
  pthread_rwlock_unlock(&kvs_table->tablelock);
  return 1;
}

int kvs_rm_subscriptions() {
  if (kvs_table == NULL) {
    fprintf(stderr, "KVS state must be initialized\n");
    return 0;
  }

  pthread_rwlock_wrlock(&kvs_table->tablelock);
  rm_all_subscriptions(kvs_table);
  pthread_rwlock_unlock(&kvs_table->tablelock);
  return 1;
}