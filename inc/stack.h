
#ifndef STACK_TEMPLATE
#define STACK_TEMPLATE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define new_stack                                                              \
  { .data = NULL, .size = 0, .capacity = 0 }

#define declare_template_stack_type(type, stack_type)                          \
  declare_template_stack_type_internal(                                        \
      type, stack_type, free_##stack_type, resize_##stack_type,                \
      push_##stack_type, pop_##stack_type, at_##stack_type,                    \
      fn_rw_##stack_type, fread_##stack_type, fwrite_##stack_type)

#define define_template_stack_type(type, stack_type)                           \
  define_template_stack_type_internal(type, stack_type, resize_##stack_type,   \
                                      push_##stack_type, pop_##stack_type,     \
                                      at_##stack_type, fn_rw_##stack_type,     \
                                      fread_##stack_type, fwrite_##stack_type)

#define declare_template_stack_type_internal(                                  \
    type, stack_type, free_stack, resize_stack, push_stack, pop_stack,         \
    at_stack, fn_rw_stack, fread_stack, fwrite_stack)                          \
  typedef struct stack_type {                                                  \
    type *data;                                                                \
    size_t capacity;                                                           \
    size_t size;                                                               \
  } stack_type;                                                                \
  typedef void (*fn_rw_stack)(type *, FILE *);                                 \
  void free_stack(stack_type *stack);                                          \
  void resize_stack(stack_type *, size_t);                                     \
  void push_stack(stack_type *, type);                                         \
  void pop_stack(stack_type *);                                                \
  type *at_stack(stack_type *, size_t);                                        \
  void fread_stack(stack_type *, fn_rw_stack, FILE *db);                       \
  void fwrite_stack(stack_type *, fn_rw_stack, FILE *db);

#define define_template_stack_type_internal(                                   \
    type, stack_type, resize_stack, push_stack, pop_stack, at_stack,           \
    fn_rw_stack, fread_stack, fwrite_stack)                                    \
                                                                               \
  void resize_stack(stack_type *stack, size_t size) {                          \
    stack->capacity = size;                                                    \
    stack->data = realloc(stack->data, stack->capacity * sizeof(type));        \
  }                                                                            \
                                                                               \
  void push_stack(stack_type *stack, type data) {                              \
    if (stack->size >= stack->capacity) {                                      \
      resize_stack(stack, stack->size * 1.5 + 1);                              \
    }                                                                          \
    memcpy(&(stack->data[stack->size++]), &data, sizeof(data));                \
  }                                                                            \
                                                                               \
  void pop_stack(stack_type *stack) {                                          \
    if (stack->size <= stack->capacity / 3) {                                  \
      resize_stack(stack, stack->capacity / 2);                                \
    }                                                                          \
    stack->size--;                                                             \
  }                                                                            \
                                                                               \
  void fread_stack(stack_type *stack, fn_rw_stack fn, FILE *db) {              \
    fread(&(stack->size), sizeof(stack->size), 1, db);                         \
    resize_stack(stack, stack->size);                                          \
    for (size_t i = 0; i < stack->size; i++) {                                 \
      fn(at_stack(stack, i), db);                                              \
    }                                                                          \
  }                                                                            \
                                                                               \
  void fwrite_stack(stack_type *stack, fn_rw_stack fn, FILE *db) {             \
    fwrite(&(stack->size), sizeof(stack->size), 1, db);                        \
    for (size_t i = 0; i < stack->size; i++) {                                 \
      fn(at_stack(stack, i), db);                                              \
    }                                                                          \
  }                                                                            \
                                                                               \
  type *at_stack(stack_type *stack, size_t i) {                                \
    return (i < stack->size)                                                   \
               ? &(stack->data[i])                                             \
               : ((i >= -(stack->size)) ? &(stack->data[i + stack->size])      \
                                        : NULL);                               \
  }

#endif // STACK_TEMPLATE
