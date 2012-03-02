#ifndef CLIENT_H
#define CLIENT_H
#include "login.h"

typedef struct client client;

int client_new(int);
int client_handle(int);
struct login *client_get_login(struct client *);
#endif
