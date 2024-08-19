#include "types.h"
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> // read(), write(), close()

#include "cumo.h"

struct socket_type {
  i32 descriptor;
  union {
    struct sockaddr_in _addr;
    struct sockaddr addr;
  };
  u32 addr_len;
};

socket_type socket_cumo(u32 domain, u32 type, u32 protocol) {
  i32 sock = socket(domain, type, protocol);
  if (sock == -1) {
    exit(1);
  }
  return (socket_type){.descriptor = sock, ._addr = {.sin_family = domain}};
}

u0 bind_cumo(socket_type *sock, u32 addr, u32 port) {
  sock->_addr.sin_addr.s_addr = htonl(addr);
  sock->_addr.sin_port = htons(port);
  if (bind(sock->descriptor, &(sock->addr), sizeof(sock->addr)) != 0) {
    exit(2);
  }
}

u0 listen_cumo(socket_type *sock, u32 nconnection) {
  if (listen(sock->descriptor, nconnection) != 0) {
    exit(3);
  }
}

socket_type accept_cumo(socket_type *sock) {
  socket_type conn = {};
  conn.addr_len = sizeof(conn.addr);
  conn.descriptor = accept(sock->descriptor, &(conn.addr), &(conn.addr_len));
  return conn;
}

u0 socket_functor_cumo(u32 d, u32 t, u32 p, socket_fn fn, void *a) {
  socket_type sock = socket_cumo(d, t, p);
  (u0) fn(&sock, a);
  close(sock.descriptor);
}

u0 accept_functor_cumo(socket_type *sock, socket_fn fn, void *a) {
  socket_type conn = accept_cumo(sock);
  (u0) fn(&conn, a);
  close(conn.descriptor);
}

u0 socket_functor_cumo_af_inet_sock_stream(socket_fn fn, void *a) {
  socket_functor_cumo(AF_INET, SOCK_STREAM, 0, fn, a);
}
