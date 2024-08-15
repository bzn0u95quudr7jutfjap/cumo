#include <server.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  u32 next_id;
  stack_metadata metadata;
  stack_string filepaths;
} server_state;

server_state state = {
    .next_id = 0, .metadata = newstack, .filepaths = newstack};

u32 next_id;
u32 files_count;

// TODO replace with a stack
metadata *files_metadata = NULL;
char **files_names = NULL;

u0 read_database(FILE *db) {
  fseek(db, 0, SEEK_SET);
  fread(&next_id, sizeof(next_id), 1, db);
  fread(&files_count, sizeof(files_count), 1, db);
  files_metadata = malloc(sizeof(*files_metadata) * files_count);
  fread(&files_metadata, sizeof(*files_metadata), files_count, db);
  files_names = malloc(sizeof(*files_names) * files_count);
  for (u32 i = 0; i < files_count; i++) {
    u32 len = files_metadata[i].path_len;
    u32 sizeofchar = sizeof(*(files_names[i]));
    files_names[i] = malloc(sizeofchar * len);
    fread(files_names[i], sizeofchar, len, db);
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
