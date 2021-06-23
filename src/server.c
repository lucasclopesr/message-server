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
      printf("[log] %d bytes\n", (int) count);

      if (!invalidInput(buf, BUFSIZE)) {
        buf[strcspn(buf, "\n")] = 0;
        printf("[msg] %s, %d len: %s\n", caddrstr, (int) strlen(buf), buf);

        if (0 == strcmp(buf, "kill")) {
          close(csock);
          exit(EXIT_SUCCESS);
        }

        char* command = strtok(buf, " "); // Split message by spaces
        if (0 == strcmp(command, "add")) {
          // ADD X Y
          char* char_x = strtok(NULL, " ");
          int x = atoi(char_x);
          char* char_y = strtok(NULL, " ");
          int y = atoi(char_y);

          printf("[log] x=%d y=%d\n", x, y);
          int r = add(&locs, x, y);

          if (r == ADDED) {
            sprintf(buf, "%d %d added\n", x, y);
          } else if (r == ALREADY_EXISTS) {
            sprintf(buf, "%d %d already exists\n", x, y);
          } else if (r == LIMIT_EXCEEDED){
            sprintf(buf, "maximum number of localities reached");
          }else {
            sprintf(buf, "could not add %d %d\n", x, y);
          }
        } else if (0 == strcmp(command, "rm")) {
          // REMOVE X Y
          char* char_x = strtok(NULL, " ");
          int x = atoi(char_x);
          char* char_y = strtok(NULL, " ");
          int y = atoi(char_y);

          printf("[log] x=%d y=%d\n", x, y);
          int r = remove_loc(&locs, x, y);

          if (r == REMOVED) {
            sprintf(buf, "%d %d removed\n", x, y);
          } else if (r == DOES_NOT_EXIST) {
            sprintf(buf, "%d %d does not exist\n", x, y);
          } else {
            sprintf(buf, "could not add %d %d\n", x, y);
          }
        } else if (0 == strcmp(command, "list")) {
          // LIST
          const char* list_locations = list(locs);
          printf("[log] list: %s\n", list_locations);
          sprintf(buf, "%s", list_locations);
        } else if (0 == strcmp(command, "query")) {
          // QUERY X Y
          char* char_x = strtok(NULL, " ");
          int x = atoi(char_x);
          char* char_y = strtok(NULL, " ");
          int y = atoi(char_y);
          Loc q = query(locs, x, y);

          if (q.x != -1 && q.y != -1) {
            sprintf(buf, "%d %d\n", q.x, q.y);
          } else {
            sprintf(buf, "Not found");
          }

        } else {
          // Closes connection if the message format is incorrect
          close(csock);
          break;
        }

        count = send(csock, buf, strlen(buf)+1, 0);
        if (count != strlen(buf)+1) {
          logexit("send");
        }
      }
    }

    close(csock);
  }

  exit(EXIT_SUCCESS);

  return 0;
}
