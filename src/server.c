#include <server.h>
#include <stdio.h>
#include <stdlib.h>

u32 next_id;
u32 files_count;

typedef struct {
  u32 id;
  u32 path_len;
} metadata;

// TODO replace with a stack
metadata *files_metadata = NULL;
char **files_names = NULL;

u0 read_database() {
  FILE *db = fopen("database.cumo", "rb");
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
