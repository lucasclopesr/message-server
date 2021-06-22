#include <locations.h>
#include <utils.h>

void usage(int argc, char **argv) {
  printf("usage: %s <v4|v6> <server port>\n", argv[0]);
  printf("example: %s v4 51511\n", argv[0]);
  exit(EXIT_FAILURE);
}

void stripInvalid(char *s, int len){
    int i;

  for (i = 0; i < len; i++) {
    if (s[i] == '\n') {
      s[i] = '\0';
      break;
    }
  }
}

int main(int argc, char **argv){
  if (argc < 3)
    usage(argc, argv);

  Locs locs;
  initialize(&locs);

  // Parse arguments to a socket object
  struct sockaddr_storage storage;
  if (0 != server_sockaddr_init(argv[1], argv[2], &storage)) {
    logexit("address");
  }

  // Create connection socket
  int s;
  s = socket(storage.ss_family, SOCK_STREAM, 0);
  if ( s == -1 )
    logexit("socket");

  struct sockaddr *addr = (struct sockaddr *)(&storage);

  if (0 != bind(s, addr, sizeof(storage)))
    logexit("bind");

  if (0 != listen(s, PENDING_CONNECTIONS))
    logexit("listen");

  char addrstr[BUFSIZE];
  addrtostr(addr, addrstr, BUFSIZE);
  printf("bound to %s, waiting connections\n", addrstr);

  struct sockaddr_storage cstorage;
  struct sockaddr *caddr = (struct sockaddr *)(&cstorage);
  socklen_t caddr_size = sizeof(storage);

  while (1) {
    int csock = accept(s, caddr, &caddr_size);
    if (csock == -1)
      logexit("accept");

    char caddrstr[BUFSIZE];
    addrtostr(caddr, caddrstr, BUFSIZE);
    printf("[log] connection from %s\n", caddrstr);

    char buf[BUFSIZE];
    memset(buf, 0, BUFSIZE);

    while (1) {
      // printf("[log] strcmp: %d", strcmp(buf, "EOF"));
      memset(buf, 0, BUFSIZE);
      size_t count = recv(csock, buf, BUFSIZE, 0);
      printf("[log] count: %d", count);

      buf[strcspn(buf, "\n")] = 0;
      printf("[msg] %s, %d bytes: %s\n", caddrstr, strlen(buf), buf);

      // Remove all invalid characters
      if (0 == strcmp(buf, "EOF")) {
        break;
      }

      // Check for message size
      // Close client if message == EOF
      

      sprintf(buf, "remote endpoint: %s\n", caddrstr);
      count = send(csock, buf, strlen(buf)+1, 0);
      if (count != strlen(buf)+1) {
        logexit("send");
      }
    }

    close(csock);
  }

  exit(EXIT_SUCCESS);

  return 0;
}
