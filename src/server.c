#include <server.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  u32 next_id;
  stack_metadata metadata;
  stack_string paths;
} server_state;

server_state state = {.next_id = 0, .metadata = new_stack, .paths = new_stack};

u32 next_id;
u32 files_count;

// TODO replace with a stack
metadata *files_metadata = NULL;
char **files_names = NULL;

u0 read_database(FILE *db) {
  fseek(db, 0, SEEK_SET);
  u32 next = 0, count = 0;
  fread(&next, sizeof(next), 1, db);
  fread(&count, sizeof(count), 1, db);

  state.next_id = next;
  resize_stack_metadata(&(state.metadata), count);
  resize_stack_string(&(state.paths), count);

  metadata *data = state.metadata.data;
  fread(data, sizeof(*data), count, db);

  for (u32 i = 0; i < count; i++) {
    u32 len = data[i].path_len;
    string *name = at_stack_string(&(state.paths), i);
    resize_string(name, len);
    fread(name->data, sizeof(*(name->data)), len, db);
  }
}

u0 write_database(FILE *db) {
  fseek(db, 0, SEEK_SET);
  u32 next = state.next_id, count = state.metadata.size;
  for (u32 i = 0; i < count; i++) {
    at_stack_metadata(&(state.metadata), i)->path_len =
        at_stack_string(&(state.paths), i)->size;
  }
  fwrite(&next, sizeof(next), 1, db);
  fwrite(&count, sizeof(count), 1, db);
  stack_metadata *data = &(state.metadata);
  fwrite(data->data, sizeof(*(data->data)), count, db);
  for (u32 i = 0; i < count; i++) {
    string *name = at_stack_string(&(state.paths), i);
    fwrite(name->data, sizeof(*(name->data)), name->size, db);
  }
}

u0 init_database() {
  FILE *db = fopen("database.cumo", "ab+");
  if (db == NULL) {
    goto exit;
  }
  fseek(db, 0, SEEK_END);
  if (ftell(db) == 0) {
    u32 bf[1] = {0};
    fwrite(bf, sizeof(*bf), 1, db);
  }

  read_database(db);

exit:
  fclose(db);
}
