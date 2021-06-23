#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <inttypes.h>
#include <unistd.h>

#define FALSE 0
#define TRUE 1
#define BUFSIZE 500
#define PENDING_CONNECTIONS 10

int addrparse(const char *addrstr, const char *portstr, struct sockaddr_storage *storage);
int server_sockaddr_init(const char *proto, const char* portstr, struct sockaddr_storage *storage);
void logexit(const char *msg);
void addrtostr(const struct sockaddr *addr, char *str, size_t strsize); 
int invalidInput(const char * buf, int len);
