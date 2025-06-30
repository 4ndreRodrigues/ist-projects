#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "kvs.h"
#include "constants.h"

static struct HashTable* kvs_table = NULL;


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
  return 0;
}

int kvs_write(size_t num_pairs, char keys[][MAX_STRING_SIZE], char values[][MAX_STRING_SIZE]) {
  if (kvs_table == NULL) {
    fprintf(stderr, "KVS state must be initialized\n");
    return 1;
  }

  for (size_t i = 0; i < num_pairs; i++) {
    if (write_pair(kvs_table, keys[i], values[i]) != 0) {
      fprintf(stderr, "Failed to write keypair (%s,%s)\n", keys[i], values[i]);
    }
  }

  return 0;
}

int kvs_read(int fdo, size_t num_pairs, char keys[][MAX_STRING_SIZE]) {
  if (kvs_table == NULL) {
    fprintf(stderr, "KVS state must be initialized\n");
    return 1;
  }

  write(fdo, "[", 1);
  for (size_t i = 0; i < num_pairs; i++) {
    char buffer[512];
    char* result = read_pair(kvs_table, keys[i]);
    if (result == NULL) {
      snprintf(buffer, sizeof(buffer), "(%s,KVSERROR)", keys[i]);
    } else {
      snprintf(buffer, sizeof(buffer), "(%s,%s)", keys[i], result);
    }
    write(fdo, buffer, strlen(buffer));
    free(result);
  }
  write(fdo, "]\n", 2);
  return 0;
}

int kvs_delete(int fdo, size_t num_pairs, char keys[][MAX_STRING_SIZE]) {
  if (kvs_table == NULL) {
    fprintf(stderr, "KVS state must be initialized\n");
    return 1;
  }
  int aux = 0;

  for (size_t i = 0; i < num_pairs; i++) {
    char buffer[512];
    if (delete_pair(kvs_table, keys[i]) != 0) {
      if (!aux) {
        write(fdo, "[", 1);
        aux = 1;
      }
      snprintf(buffer, sizeof(buffer), "(%s,KVSMISSING)", keys[i]);
      write(fdo, buffer, strlen(buffer));
    }
  }
  if (aux) {
    write(fdo, "]\n", 2);
  }

  return 0;
}

void kvs_show(int fdo) {
  for (int i = 0; i < TABLE_SIZE; i++) {
    KeyNode *keyNode = kvs_table->table[i];
    while (keyNode != NULL) {
      char buffer[512];
      int len = snprintf(buffer, sizeof(buffer), "(%s, %s)\n", keyNode->key, keyNode->value);
      if (len > 0) {
          write(fdo, buffer, (size_t)len);
      }
      keyNode = keyNode->next; // Move to the next node
    }
  }
}

int kvs_backup(char *filepath, int backup_counter) {
  // Código para o processo filho

  char filepath_backup[MAX_JOB_FILE_NAME_SIZE];
  snprintf(filepath_backup, MAX_JOB_FILE_NAME_SIZE, "%s-%d.bck", filepath, backup_counter);

  int fdb = open(filepath_backup, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fdb < 0) {
    perror("open error");
    return 1;
  }

  kvs_show(fdb);

  close(fdb);    
  return 0;
}

void kvs_wait(unsigned int delay_ms) {
  struct timespec delay = delay_to_timespec(delay_ms);
  nanosleep(&delay, NULL);
}