#ifndef CLIENT_API_H
#define CLIENT_API_H

#include <stddef.h>
#include "src/common/constants.h"

typedef struct {
  int sessionx;
  int reqx;
  int respx;
  int notifx;
} Connection;

/// Connects to a kvs server.
/// @param cli_connection
/// @param req_pipe_path Path to the name pipe to be created for requests.
/// @param resp_pipe_path Path to the name pipe to be created for responses.
/// @param server_pipe_path Path to the name pipe where the server is listening.
/// @return 0 if the connection was established successfully, 1 otherwise.
int kvs_connect(Connection* cli_connection, char* req_pipe_path, char* resp_pipe_path, char* server_pipe_path,
                char* notif_pipe_path); //tirei o 5º argumento (int* notif_pipe)
/// Disconnects from an KVS server.
/// @return 0 in case of success, 1 otherwise.
int kvs_disconnect(Connection* cli_connection, char const* req_pipe_path, char const* resp_pipe_path,
                   char const* notif_pipe_path);

/// Requests a subscription for a key
/// @param key Key to be subscribed
/// @return 1 if the key was subscribed successfully (key existing), 0 otherwise.

int kvs_subscribe(Connection* cli_connection, char* key);

/// Remove a subscription for a key
/// @param key Key to be unsubscribed
/// @return 0 if the key was unsubscribed successfully  (subscription existed and was removed), 1 otherwise.

int kvs_unsubscribe(Connection* cli_connection, char* key);
 
#endif  // CLIENT_API_H
