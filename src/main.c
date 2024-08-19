#include "cumo.h"
#include "stdio.h"
#include "string.h"
#include "types.h"

char *_validarguments = "modalità di utilizzo\n";
char *_clientmode = "--client <ip> <port>\n";
char *_servermode = "--server <ip> <port>\n";

int main(int argc, char *argv[]) {
  if (argc == 1) {
    printf("%s", _validarguments);
    printf("%s", _clientmode);
    printf("%s", _servermode);
    return 0;
  }

  if (strcmp(argv[1], "--server") == 0) {
    socket_functor_cumo_af_inet_sock_stream(server_main, NULL);
  }

  if (strcmp(argv[1], "--client") == 0) {
    socket_functor_cumo_af_inet_sock_stream(client_main, NULL);
  }
}
