#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>

#include "parser.h"
#include "src/client/api.h"
#include "src/common/constants.h"
#include "src/common/io.h"


static void* notif_handler(void* arguments) {
  int notif_pipe = *(int*) arguments;
  char notif_message_key[MAX_STRING_SIZE + 1] = {0};
  char notif_message_value[MAX_STRING_SIZE + 1] = {0};
  
  while (notif_pipe != -1) {
    if (read_all(notif_pipe, notif_message_key, MAX_STRING_SIZE + 1, NULL) < 0) {
      fprintf(stderr, "[ERROR]: Failed to read data\n");
      break;
    }

    if (read_all(notif_pipe, notif_message_value, MAX_STRING_SIZE + 1, NULL) < 0) {
      fprintf(stderr, "[ERROR]: Failed to read data\n");
      break;
    }
    fprintf(stdout, "(%s,%s)\n", notif_message_key, notif_message_value);
  }
  pthread_exit(NULL);
}


int main(int argc, char* argv[]) {
  if (argc < 3) {
    fprintf(stderr, "Usage: %s <client_unique_id> <server_pipe_path>\n", argv[0]);
    return 1;
  }

  char req_pipe_path[MAX_PIPE_PATH_LENGTH] = "/tmp/req";
  char resp_pipe_path[MAX_PIPE_PATH_LENGTH] = "/tmp/resp";
  char notif_pipe_path[MAX_PIPE_PATH_LENGTH] = "/tmp/notif";
  char server_pipe_path[MAX_PIPE_PATH_LENGTH];

  Connection* cli_connection = malloc(sizeof(Connection));
  char keys[MAX_NUMBER_SUB][MAX_STRING_SIZE] = {0};
  unsigned int delay_ms;
  size_t num;
  

  strncat(req_pipe_path, argv[1], strlen(argv[1]) * sizeof(char));
  strncat(resp_pipe_path, argv[1], strlen(argv[1]) * sizeof(char));
  strncat(notif_pipe_path, argv[1], strlen(argv[1]) * sizeof(char));
  strcpy(server_pipe_path, argv[2]);

  // TODO open pipes /////////////////////////////////////////////////////////////////////////////////////
  int i = kvs_connect(cli_connection, req_pipe_path, resp_pipe_path, server_pipe_path, notif_pipe_path);
  if (i != 0) {
    fprintf(stderr, "Failed to connect to the server\n");
    return 1;
  }

  pthread_t notif_thread;
  int* notifx = &cli_connection->notifx;
  if (pthread_create(&notif_thread, NULL, notif_handler, (void*)notifx) != 0) {
    fprintf(stderr, "Failed to create host thread\n");
    return 1;
  }

  ////////////////////////////////////////////////////////////////////////////////////////////////////////
  while (1) {
    switch (get_next(STDIN_FILENO)) {
      case CMD_DISCONNECT:
        if (kvs_disconnect(cli_connection, req_pipe_path, resp_pipe_path, notif_pipe_path) != 0) {
          fprintf(stderr, "Failed to disconnect to the server\n");
          return 1;
        }
        // end notifications thread
        if(pthread_cancel(notif_thread) != 0){
          fprintf(stderr, "Failed to create host thread\n");
          return 1;
        }
        
        if (pthread_join(notif_thread, NULL) != 0) {
          fprintf(stderr, "Failed to join notifications thread\n");
          return 1;
        }
        printf("Disconnected from server\n");
        return 0;

      case CMD_SUBSCRIBE:
        num = parse_list(STDIN_FILENO, keys, 1, MAX_STRING_SIZE);
        if (num == 0) {
          fprintf(stderr, "Invalid command. See HELP for usage\n");
          continue;
        }
        if (kvs_subscribe(cli_connection, keys[0])) {
            fprintf(stderr, "Command subscribe failed\n");
        }

        break;

      case CMD_UNSUBSCRIBE:
        num = parse_list(STDIN_FILENO, keys, 1, MAX_STRING_SIZE);
        if (num == 0) {
          fprintf(stderr, "Invalid command. See HELP for usage\n");
          continue;
        }
         
        if (kvs_unsubscribe(cli_connection, keys[0])) {
            fprintf(stderr, "Command subscribe failed\n");
        }

        break;

      case CMD_DELAY:
        if (parse_delay(STDIN_FILENO, &delay_ms) == -1) {
          fprintf(stderr, "Invalid command. See HELP for usage\n");
          continue;
        }

        if (delay_ms > 0) {
            printf("Waiting...\n");
            delay(delay_ms);
        }
        break;

      case CMD_INVALID:
        fprintf(stderr, "Invalid command. See HELP for usage\n");
        break;

      case CMD_EMPTY:
        break;

      case EOC:
        // input should end in a disconnect, or it will loop here forever
        break;
    }
  }

  if (pthread_join(notif_thread, NULL) != 0) {
    fprintf(stderr, "Failed to join notifications thread\n");
    return 1;
  }
  fprintf(stderr, "[INFO]: closing pipes\n");/////
  close(cli_connection->sessionx);/////////////////////////////////////
  close(cli_connection->reqx);///////////////////////////////////// 
  close(cli_connection->respx);///////////////////////////////////// 
  close(cli_connection->notifx);/////////////////////////////////////
  free(cli_connection); 
}
