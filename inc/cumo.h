#ifndef __CUMO__
#define __CUMO__

#include "types.h"

typedef struct socket_type socket_type;
typedef void *(*socket_fn)(socket_type *, void *);

u0 socket_functor_cumo(u32 d, u32 t, u32 p, socket_fn fn, void *a);
u0 bind_cumo(socket_type *sock, u32 addr, u32 port);
u0 listen_cumo(socket_type *sock, u32 nconnection);
u0 accept_functor_cumo(socket_type *sock, socket_fn fn, void *a);

u32 main_server();
u32 main_client();

#endif //__CUMO__
