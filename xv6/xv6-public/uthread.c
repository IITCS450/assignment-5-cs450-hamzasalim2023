#include "types.h"
#include "stat.h"
#include "user.h"
#include "uthread.h"

enum { FREE, RUNNABLE, RUNNING, ZOMBIE };

struct thread {
  tid_t tid;
  int state;
  void *stack;
  struct context *context;
};

static struct thread threads[MAX_THREADS];
static struct thread *current;

static void
thread_stub(void (*fn)(void*), void *arg)
{
  fn(arg);
  current->state = ZOMBIE;
  thread_yield();
  exit();
}

static struct thread*
pick_next(void)
{
  int start = current->tid;

  for(int i = 1; i <= MAX_THREADS; i++){
    int idx = (start + i) % MAX_THREADS;
    if(threads[idx].state == RUNNABLE)
      return &threads[idx];
  }
  return current;
}

void
thread_init(void)
{
  for(int i = 0; i < MAX_THREADS; i++){
    threads[i].tid = i;
    threads[i].state = FREE;
    threads[i].stack = 0;
    threads[i].context = 0;
  }

  current = &threads[0];
  current->state = RUNNING;
}

tid_t
thread_create(void (*fn)(void*), void *arg)
{
  struct thread *t;
  uint *sp;

  for(int i = 0; i < MAX_THREADS; i++){
    if(threads[i].state == FREE){
      t = &threads[i];
      goto found;
    }
  }
  return -1;

found:
  t->stack = malloc(STACK_SIZE);
  if(t->stack == 0)
    return -1;

  sp = (uint*)((char*)t->stack + STACK_SIZE);

  // args for thread_stub
  *--sp = (uint)arg;
  *--sp = (uint)fn;
  *--sp = 0xffffffff;

  // saved registers
  *--sp = 0; // edi
  *--sp = 0; // esi
  *--sp = 0; // ebx
  *--sp = 0; // ebp

  t->context = (struct context*)sp;
  t->context->eip = (uint)thread_stub;

  t->state = RUNNABLE;
  return t->tid;
}

void
thread_yield(void)
{
  struct thread *prev = current;
  struct thread *next = pick_next();

  if(next == current)
    return;

  if(prev->state == RUNNING)
    prev->state = RUNNABLE;

  next->state = RUNNING;
  current = next;

  uswtch(&prev->context, next->context);
}

int
thread_join(tid_t tid)
{
  if(tid < 0 || tid >= MAX_THREADS || tid == current->tid)
    return -1;

  while(threads[tid].state != ZOMBIE)
    thread_yield();

  free(threads[tid].stack);
  threads[tid].state = FREE;

  return 0;
}
