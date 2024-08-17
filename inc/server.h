#ifndef __CUMO_SERVER__
#define __CUMO_SERVER__

#include <stack.h>
#include <types.h>

typedef struct {
  u32 id;
  u32 path_len;
} metadata;

declare_template_stack_type(metadata, stack_metadata);
declare_template_stack_type(char, string);
declare_template_stack_type(string, stack_string);

#endif //__CUMO_SERVER__
