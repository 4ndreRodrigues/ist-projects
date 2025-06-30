#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>


#include "api.h"
#include "src/common/io.h"
#include "src/common/constants.h"
#include "src/common/protocol.h"


int kvs_connect(Connection* cli_connection, char* req_pipe_path, char* resp_pipe_path, char* server_pipe_path,
                char* notif_pipe_path) { //tirei o 5º argumento (int* notif_pipe)
  // create pipes and connect
 
  // Open server pipe for writing
  // This waits for someone to open it for reading
  cli_connection->sessionx = open(server_pipe_path, O_WRONLY);
  if (cli_connection->sessionx == -1) {
      fprintf(stderr, "[ERR]: open failed: %s\n", strerror(errno));
      exit(EXIT_FAILURE);
  }

  adjust_string(req_pipe_path, MAX_PIPE_PATH_LENGTH);
  adjust_string(resp_pipe_path, MAX_PIPE_PATH_LENGTH);
  adjust_string(notif_pipe_path, MAX_PIPE_PATH_LENGTH);
  adjust_string(server_pipe_path, MAX_PIPE_PATH_LENGTH);

  char message[MAX_PIPE_PATH_LENGTH * 3 + 1] = {0};
  memset(message, 0, sizeof(message));
  // Construir a mensagem
  memcpy(&message[0], "1", 1); // OP_CODE
  memcpy(&message[1] , req_pipe_path, MAX_PIPE_PATH_LENGTH);
  memcpy(&message[1 + MAX_PIPE_PATH_LENGTH], resp_pipe_path, MAX_PIPE_PATH_LENGTH);
  memcpy(&message[1 + 2 * MAX_PIPE_PATH_LENGTH], notif_pipe_path, MAX_PIPE_PATH_LENGTH);
  
  if (write_all(cli_connection->sessionx, message, sizeof(message)) < 0) {
      fprintf(stderr, "[ERR]: write_all failed\n");
      close(cli_connection->sessionx);
      return -1;
  }

    /* remove pipes if it exists */
  if (unlink(req_pipe_path) != 0 && errno != ENOENT) {
    perror("[ERR]: unlink(%s) failed");
    return 1;
  }
  if (unlink(resp_pipe_path) != 0 && errno != ENOENT) {
    perror("[ERR]: unlink(%s) failed");
    return 1;
  }
  if (unlink(notif_pipe_path) != 0 && errno != ENOENT) {
    perror("[ERR]: unlink(%s) failed");
    return 1;
  }

    /* create communication pipes */
  if (mkfifo(req_pipe_path, 0640) != 0) {
    perror("[ERR]: mkfifo failed");
    return 1;
  }
  if (mkfifo(resp_pipe_path, 0640) != 0) {
    perror("[ERR]: mkfifo failed");
    return 1;
  }
  if (mkfifo(notif_pipe_path, 0640) != 0) {
    perror("[ERR]: mkfifo failed");
    return 1;
  }
    /**
   * open pipe for reading
   * this waits for someone to open it for writing
   */
  cli_connection->reqx = open(req_pipe_path, O_WRONLY);
  if (cli_connection->reqx == -1) {
    perror("[ERR]: open failed");
    return 1;
  }
  cli_connection->respx = open(resp_pipe_path, O_RDONLY);
  if (cli_connection->respx == -1) {
    perror("[ERR]: open failed");
    return 1;
  }
  cli_connection->notifx = open(notif_pipe_path, O_RDONLY);
  if (cli_connection->notifx == -1) {
    perror("[ERR]: open failed");
    return 1;
  }

  char resp_message[3] = {0};
  if (read_all(cli_connection->respx, resp_message, 3, NULL) < 0) {
    fprintf(stderr, "[ERROR]: Failed to read data\n");
    free(cli_connection);
    return 1;
  }
  fprintf(stdout, "Server returned %c for operation: connect\n", resp_message[1]);

  return 0;
}
 
int kvs_disconnect(Connection* cli_connection, char const* req_pipe_path, char const* resp_pipe_path,
                   char const* notif_pipe_path) {
  char op_code = '2';
  char message[MAX_STRING_SIZE + 1] = {0};
  message[0] = op_code; // OP_CODE
  adjust_string(message, MAX_STRING_SIZE + 1);

  if (write_all(cli_connection->reqx, message, sizeof(message)) < 0) {
      fprintf(stderr, "[ERR]: write_all failed\n");
      close(cli_connection->sessionx);
      return 1;
  }
  char resp_message[3] = {0};
  if (read_all(cli_connection->respx, resp_message, 3, NULL) < 0) {
    fprintf(stderr, "[ERROR]: Failed to read data\n");
    free(cli_connection);//rever
    return 1;
  }
  fprintf(stdout, "Server returned %c for operation: disconnect\n", resp_message[1]);

  // close pipes and unlink pipe files
  close(cli_connection->respx);
  close(cli_connection->reqx);
  close(cli_connection->respx);
  close(cli_connection->notifx);
  cli_connection->notifx = -1; //End notif_thread
  unlink(req_pipe_path);
  unlink(resp_pipe_path);
  unlink(notif_pipe_path);
  return 0;
}

int kvs_subscribe(Connection* cli_connection, char* key) {
  // send subscribe message to request pipe and wait for response in response pipe
  char op_code = '3';
  char message[MAX_STRING_SIZE + 1] = {0};

  adjust_string(key, MAX_STRING_SIZE);
  memset(message, 0, sizeof(message));
  // Construir a mensagem
  message[0] = op_code;
  memcpy(&message[1] , key, MAX_STRING_SIZE);

  if (write_all(cli_connection->reqx, message, sizeof(message)) < 0) {
      fprintf(stderr, "[ERR]: write_all failed\n");
      close(cli_connection->sessionx);//rever
      return 1;
  }
  char resp_message[3] = {0};
  if (read_all(cli_connection->respx, resp_message, 3, NULL) < 0) {
    fprintf(stderr, "[ERROR]: Failed to read data\n");
    return 1;
  }
  fprintf(stdout, "Server returned %c for operation: subscribe\n", resp_message[1]);

  return 0;
}

int kvs_unsubscribe(Connection* cli_connection, char* key) {
    // send unsubscribe message to request pipe and wait for response in response pipe
    char op_code = '4';
    char message[MAX_STRING_SIZE + 1] = {0};

    adjust_string(key, MAX_STRING_SIZE);
    memset(message, 0, sizeof(message));
    // Construir a mensagem
    message[0] = op_code;
    memcpy(&message[1] , key, MAX_STRING_SIZE);

    if (write_all(cli_connection->reqx, message, sizeof(message)) < 0) {
        fprintf(stderr, "[ERR]: write_all failed\n");
        close(cli_connection->sessionx);//rever
        return 1;
    }
    char resp_message[3] = {0};
    if (read_all(cli_connection->respx, resp_message, 3, NULL) < 0) {
      fprintf(stderr, "[ERROR]: Failed to read data\n");
      return 1;
    }
    fprintf(stdout, "Server returned %c for operation: unsubscribe\n", resp_message[1]);

  return 0;
}


