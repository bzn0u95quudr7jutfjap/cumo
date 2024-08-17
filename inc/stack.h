
#ifndef STACK_TEMPLATE
#define STACK_TEMPLATE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define newstack                                                               \
  { .data = NULL, .size = 0, .capacity = 0 }

#define declare_template_stack_type(type, stack_type)                          \
  typedef struct stack_type stack_type;                                        \
  struct stack_type {                                                          \
    type *data;                                                                \
    size_t capacity;                                                           \
    size_t size;                                                               \
  };                                                                           \
  stack_type new_##stack_type();                                               \
  void free_##stack_type(stack_type *stack);                                   \
  void push_##stack_type(stack_type *, type);                                  \
  void pop_##stack_type(stack_type *);                                         \
  void resize_##stack_type(stack_type *, size_t);                              \
  void fread_##stack_type(stack_type *, void *db);                             \
  void fwrite_##stack_type(stack_type *, void *db);                            \
  type *at_##stack_type(stack_type *, size_t);

#define define_template_stack_type(type, stack_type)                           \
  stack_type new_##stack_type() {                                              \
    return (stack_type){.data = NULL, .size = 0, .capacity = 0};               \
  }                                                                            \
                                                                               \
  void push_##stack_type(stack_type *stack, type data) {                       \
    if (stack->size >= stack->capacity) {                                      \
      stack->capacity = stack->size * 1.5 + 1;                                 \
      stack->data = realloc(stack->data, stack->capacity * sizeof(data));      \
    }                                                                          \
    memcpy(&(stack->data[stack->size++]), &data, sizeof(data));                \
  }                                                                            \
                                                                               \
  void pop_##stack_type(stack_type *stack) {                                   \
    if (stack->size <= stack->capacity / 3) {                                  \
      stack->capacity /= 2;                                                    \
      stack->data = realloc(stack->data, stack->capacity * sizeof(type));      \
    }                                                                          \
    stack->size--;                                                             \
  }                                                                            \
                                                                               \
  void resize_##stack_type(stack_type *stack, size_t size) {                   \
    stack->capacity = size;                                                    \
    stack->size = size;                                                        \
    stack->data = realloc(stack->data, stack->capacity * sizeof(type));        \
  }                                                                            \
                                                                               \
  void fread_##stack_type(stack_type *stack, void *db) {                       \
    return;                                                                    \
    return;                                                                    \
  }                                                                            \
                                                                               \
  void fwrite_##stack_type() {                                                 \
    return;                                                                    \
    return;                                                                    \
  }                                                                            \
                                                                               \
  type *at_##stack_type(stack_type *stack, size_t i) {                         \
    return (i < stack->size)                                                   \
               ? &(stack->data[i])                                             \
               : ((i >= -(stack->size)) ? &(stack->data[i + stack->size])      \
                                        : NULL);                               \
  }

#endif // STACK_TEMPLATE
