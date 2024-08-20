#include "cumo.h"
#include "stack.h"
#include "types.h"
#include <pthread.h>
#include <stdio.h>
#include <string.h>

declare_template_stack_type(char, string);
define_template_stack_type(char, string);

typedef struct {
  u32 id;
  string path;
} metadata;

declare_template_stack_type(metadata, stack_metadata);
define_template_stack_type(metadata, stack_metadata);

typedef struct {
  u32 autoincrement;
  stack_metadata files;
} server_state;

server_state state = {};

void fread_metadata(metadata *md, FILE *db) {
  fread(&(md->id), sizeof(md->id), 1, db);
  fread(md->path.data, sizeof(*(md->path.data)), md->path.size, db);
}

void fread_server_state(FILE *db) {
  fread(&(state.autoincrement), sizeof(state.autoincrement), 1, db);
  fread_stack_metadata(&(state.files), fread_metadata, db);
}

void fwrite_metadata(metadata *md, FILE *db) {
  fwrite(&(md->id), sizeof(md->id), 1, db);
  fwrite(md->path.data, sizeof(*(md->path.data)), md->path.size, db);
}

void fwrite_server_state(FILE *db) {
  fread(&(state.autoincrement), sizeof(state.autoincrement), 1, db);
  fwrite_stack_metadata(&(state.files), fwrite_metadata, db);
}

void save_server_state() {
  FILE *db = fopen("database.bin", "wb");
  if (db == NULL) {
    goto exit;
  }

  fwrite_server_state(db);

exit:
  fclose(db);
}

void init_server_state() {
  FILE *db = fopen("database.bin", "rb");
  if (db == NULL) {
    goto exit;
  }

  fread_server_state(db);

exit:
  fclose(db);
}

u32 ip(u8 a, u8 b, u8 c, u8 d) { return d + (c << 8) + (b << 16) + (a << 24); }

//#define read_fn(type)                                                          \
//  type read_cumo_##type(socket_type *sock) {                                   \
//    type a = {};                                                               \
//    read_cumo(sock, &a, sizeof(a), 1);                                         \
//    return a;                                                                  \
//  }
//
// typedef enum {
//  c_create,
//  c_rename,
//  c_delete,
//  c_update,
//  c_dwload,
//} server_commands;
//
// read_fn(server_commands);
// read_fn(u32);
//
// u0 fn_create(socket_type *sock) {
//  metadata _a = {.id = state.autoincrement++, .path = new_stack};
//
//  push_stack_metadata(&(state.files), _a);
//  metadata *file = at_stack_metadata(&(state.files), -1);
//  string *path = &(file->path);
//  path->size = read_cumo_u32(sock);
//  resize_string(path, path->size);
//  read_cumo(sock, path->data, sizeof(*(path->data)), path->size);
//
//  char name[34] = {};
//  sprintf(name, "%032ud", file->id);
//  FILE *f = fopen(name, "wb");
//  if (f == NULL) {
//    perror("[ERR]:: non è possibile generare il file");
//    exit(4);
//  }
//  u64 size = read_cumo_u32(sock);
//  u64 bflen = 2048;
//  u8 bf[2048] = {};
//  for (u64 i = 0; i < size; i += bflen) {
//    read_cumo(sock, bf, sizeof(*bf), bflen);
//    fwrite(bf, sizeof(*bf), bflen, f);
//  }
//  fclose(f);
//}

char *c_str(string *str) {
  push_string(str, '\0');
  pop_string(str);
  return str->data;
}

//u0 read_cumo_string(socket_type *sock, string *str) {
//  read_cumo(sock, &(str->size), sizeof(str->size), 1);
//  resize_string(str, str->size);
//  read_cumo(sock, str->data, sizeof(*(str->data)), str->size);
//}

metadata *find_file(metadata *md) {
  for (u32 i = 0; i < state.files.size; i++) {
    metadata *data = at_stack_metadata(&(state.files), i);
    if (strcmp(c_str(&(md->path)), c_str(&(data->path))) == 0) {
      return data;
    }
  }
  return NULL;
}

u0 *server_main_main(socket_type *sock, u0 *args) {
  metadata a = {};
  while (1) {
    read_cumo_metadata(&a);
    metadata *b = find_file(&a);
    if (b == NULL) {
      fn_create(sock, &a);
    } else {
      if (b->stats.ctime < a.stats.ctime) {
        //rinomina il file
      }
      if (b->stats.mtime < a.stats.mtime) {
        //pulla dal client
      }else{
        //pusha sul client
      }
    }
    save_server_state();

    //switch (read_cumo_server_commands(sock)) {
    //c_create:
    //  break;
    //default:
    //  break;
    //}
  }
  return NULL;
}

u0 *server_main(socket_type *sock, u0 *args) {
  bind_cumo(sock, ip(127, 0, 0, 1), 8888);
  const u32 nconnections = 1;
  listen_cumo(sock, nconnections);
  accept_functor_cumo(sock, server_main_main, args);
  return NULL;
}
