
#include <stdio.h>
#include <pthread.h>
#include "LocalMemMgr.h"

static void *mp_memory = NULL;
static unsigned int mp_memory_size = 0;
static pthread_mutex_t oram_memory_mutex = PTHREAD_MUTEX_INITIALIZER;

int oram_memory_init (void *mp, unsigned int mp_size)
{
  int ret = 0;
  pthread_mutex_lock(&oram_memory_mutex);
  mp_memory_size = mp_size;
  mp_memory = mp;
  ret = Local_HeapInit (mp_memory, mp_memory_size);
  pthread_mutex_unlock(&oram_memory_mutex);
  return ret;
}

void oram_memory_deinit (void)
{
  pthread_mutex_lock(&oram_memory_mutex);
  mp_memory = NULL;
  mp_memory_size = 0;
  pthread_mutex_unlock(&oram_memory_mutex);
}
#if 1
void *oram_malloc (unsigned int size)
{
  pthread_mutex_lock(&oram_memory_mutex);
  void *ret = Local_Alloc (mp_memory, size);
  pthread_mutex_unlock(&oram_memory_mutex);
  return ret;
}

void oram_free (void *addr)
{
  if (addr != NULL) {
    pthread_mutex_lock(&oram_memory_mutex);
    Local_Dealloc (mp_memory, addr);
    pthread_mutex_unlock(&oram_memory_mutex);
  }
}

void *oram_calloc (unsigned int size, unsigned int n)
{
  pthread_mutex_lock(&oram_memory_mutex);
  void *ret = Local_Calloc(mp_memory, size, n);
  pthread_mutex_unlock(&oram_memory_mutex);
  return ret;
}

void *oram_realloc (void *addr, unsigned int size)
{
  pthread_mutex_lock(&oram_memory_mutex);
  void *ret = Local_Realloc (mp_memory, addr, size);
  pthread_mutex_unlock(&oram_memory_mutex);
  return ret;
}

void *oram_memalign (unsigned int align, unsigned int size)
{
  pthread_mutex_lock(&oram_memory_mutex);
  void *ret = Local_alignAlloc (mp_memory, align, size);
  pthread_mutex_unlock(&oram_memory_mutex);
  return ret;
}
#endif
