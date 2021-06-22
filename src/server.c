#include <locations.h>
#include <utils.h>

void usage(int argc, char **argv) {
  printf("usage: %s <v4|v6> <server port>\n", argv[0]);
  printf("example: %s v4 51511\n", argv[0]);
  exit(EXIT_FAILURE);
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

  while (1) {
    struct sockaddr_storage cstorage;
    struct sockaddr *caddr = (struct sockaddr *)(&cstorage);
    socklen_t caddr_size = sizeof(storage);

    int csock = accept(s, caddr, &caddr_size);
    if (csock == -1)
      logexit("accept");

    char caddrstr[BUFSIZE];
    addrtostr(caddr, caddrstr, BUFSIZE);
    printf("[log] connection from %s\n", caddrstr);

    char buf[BUFSIZE];
    memset(buf, 0, BUFSIZE);
    size_t count = recv(csock, buf, BUFSIZE, 0);
    printf("[msg] %s, %d bytes: %s\n", caddrstr, (int) count, buf);

    // TODO: implement client received message until character 
    /*unsigned total = 0;
    while(1) {
      count = recv(s, buf + total, BUFSIZE - total, 0);
      if (count == 0)
        break;
      total += count;
    }*/

    sprintf(buf, "remote endpoint: %s\n", caddrstr);
    count = send(csock, buf, strlen(buf)+1, 0);
    if (count != strlen(buf)+1) {
      logexit("send");
    }

    close(csock);
  }

  exit(EXIT_SUCCESS);

  return 0;
}
