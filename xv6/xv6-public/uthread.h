#pragma once

#include "types.h"

typedef int tid_t;

#define MAX_THREADS 8
#define STACK_SIZE  4096

struct context {
  uint eip;
  uint ebp;
  uint ebx;
  uint esi;
  uint edi;
};

void  thread_init(void);
tid_t thread_create(void (*fn)(void*), void *arg);
void  thread_yield(void);
int   thread_join(tid_t tid);

void uswtch(struct context **old, struct context *new);
