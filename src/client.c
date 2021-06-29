#include <utils.h>

void usage(int argc, char **argv) {
  printf("usage: %s <server IP> <server port>\n", argv[0]);
  printf("example: %s 127.0.0.1 51511\n", argv[0]);
  exit(EXIT_FAILURE);
}

int main(int argc, char **argv){
  if (argc < 3)
    usage(argc, argv);

  // Parse arguments to a socket object
  struct sockaddr_storage storage;
  if (0 != addrparse(argv[1], argv[2], &storage)) {
    logexit("address");
  }

  // Create connection socket
  int s;
  s = socket(storage.ss_family, SOCK_STREAM, 0);
  if ( s == -1 )
    logexit("socket");

  // START connection to server at addr
  struct sockaddr *addr = (struct sockaddr *)(&storage);
  if ( 0 != connect(s, addr, sizeof(storage)) )
    logexit("connect");
  // END connection to server at addr

  // Gets connection address string
  char addrstr[BUFSIZE];
  addrtostr(addr, addrstr, BUFSIZE);
  printf("connected to %s\n", addrstr);

  //------------- send data correctly
  
  // Send data to server
  char buf[BUFSIZE];

  while (1) {
    memset(buf, 0, BUFSIZE);
    printf("message> ");
    fgets(buf, BUFSIZE-1, stdin);
    int count = send(s, buf, strlen(buf), 0);

    if (count != strlen(buf)) {
      logexit("send");
    }

    // Waits for server answer
    memset(buf, 0, BUFSIZE);
    count = recv(s, buf, BUFSIZE, 0);
    if (count == 0)
      break;

    //printf("received %u bytes\n", count);
    puts(buf);
  }

  //------------- until connection is closed
  close(s);


  exit(EXIT_SUCCESS);

  return 0;
}
