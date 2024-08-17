#include "stack.h"
#include "types.h"
#include <server.h>
#include <stdio.h>

declare_template_stack_type(char, string);

typedef struct {
  u32 id;
  string path;
} metadata;

declare_template_stack_type(metadata, stack_metadata);

typedef struct {
  u32 autoincrement;
  stack_metadata files;
} server_state;

server_state state = {};

void fread_char(char *c, FILE *db) { fread(c, sizeof(*c), 1, db); }

void fread_metadata(metadata *md, FILE *db) {
  fread(&(md->id), sizeof(md->id), 1, db);
  fread_string(&(md->path), fread_char, db);
}

void fread_server_state(FILE *db) {
  fread(&(state.autoincrement), sizeof(state.autoincrement), 1, db);
  fread_stack_metadata(&(state.files), fread_metadata, db);
}
