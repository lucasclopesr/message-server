#include <utils.h>

// Client
int addrparse(const char *addrstr, const char *portstr, struct sockaddr_storage *storage){

  if (addrstr == NULL || portstr == NULL)
    return -1;

  uint16_t port = (uint16_t) atoi(portstr);
  if (port == 0)
    return -1;

  port = htons(port); // Host to network short

  struct in_addr inaddr4; // 32-bit IP address
  if (inet_pton(AF_INET, addrstr, &inaddr4)) {
    struct sockaddr_in *addr4 = (struct sockaddr_in *) storage;
    addr4->sin_family = AF_INET;
    addr4->sin_port = port;
    addr4->sin_addr = inaddr4;
    return 0;
  }


  struct in6_addr inaddr6; // 128-bit IP address
  if (inet_pton(AF_INET6, addrstr, &inaddr6)) {
    struct sockaddr_in6 *addr6 = (struct sockaddr_in6 *) storage;
    addr6->sin6_family = AF_INET6;
    addr6->sin6_port = port;
    memcpy(&(addr6->sin6_addr), &inaddr6, sizeof(inaddr6));
    return 0;
  }

  return -1;
}

// Server
int server_sockaddr_init(const char *proto, const char* portstr, struct sockaddr_storage *storage){
  uint16_t port = (uint16_t) atoi(portstr);
  if (port == 0)
    return -1;

  port = htons(port); // Host to network short

  memset(storage, 0, sizeof(*storage));
  if (0 == strcmp(proto, "v4")) {
    struct sockaddr_in *addr4 = (struct sockaddr_in *) storage;
    addr4->sin_family = AF_INET;
    addr4->sin_port = port;
    addr4->sin_addr.s_addr = INADDR_ANY;
    return 0;
  } else if (0 == strcmp(proto, "v6")) {
    struct sockaddr_in6 *addr6 = (struct sockaddr_in6 *) storage;
    addr6->sin6_family = AF_INET;
    addr6->sin6_port = port;
    addr6->sin6_addr = in6addr_any;
    return 0;
  }

  return -1;
}

// Both client and server
void logexit(const char *msg){
  perror(msg);
  exit(EXIT_FAILURE);
}

void addrtostr(const struct sockaddr *addr, char *str, size_t strsize) {
  int version;
  char addrstr[INET_ADDRSTRLEN+1] = "";
  uint16_t port;

  if (addr->sa_family == AF_INET) {
    version = 4;
    struct sockaddr_in *addr4 = (struct sockaddr_in *) addr;
    if (!inet_ntop(AF_INET, &(addr4->sin_addr), addrstr, INET_ADDRSTRLEN+1))
      logexit("ntop");

    port = ntohs(addr4->sin_port); // Network to host short
  } else if (addr->sa_family == AF_INET6) {
    version = 6;
    struct sockaddr_in6 *addr6 = (struct sockaddr_in6 *) addr;
    if (!inet_ntop(AF_INET6, &(addr6->sin6_addr), addrstr, INET_ADDRSTRLEN+1))
      logexit("ntop");

    port = ntohs(addr6->sin6_port); // Network to host short
  } else {
    logexit("unknown protocol family");
  }

  if (str)
    snprintf(str, strsize, "IPv%d %s %hu", version, addrstr, port);
}

int invalidInput(const char * buf, int len) {
  int i;

  // printf("\n");
  for (i = 0; i < len; i++) {
    // printf("char[%d]: %c ", i, buf[i]);
    if (buf[i] < 48 && buf[i] != 32) 
      return FALSE;
    if (buf[i] > 57 && buf[i] < 65) 
      return FALSE;
    if (buf[i] > 90 && buf[i] < 97) 
      return FALSE;
    if (buf[i] > 122) 
      return FALSE;
  }
  // printf("\n");

  return TRUE;
}
