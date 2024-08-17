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
