#include "mathify/arena.h"

#include <stdlib.h>
#include <string.h>
#include "src/io.h"

bool mathify_arena_create(arena_buf_t *buf, size_t size) {
  buf->buf = (char*)malloc(size);
  if (buf->buf == NULL) {
    ERR("Memory Allocate Failure");
    return true;
  }
  buf->bufsize = size;
  buf->bufindex = 0;
  return false;
}

void mathify_arena_delete(arena_buf_t *buf) {
  if (buf->buf != NULL) {
    free(buf->buf);
    buf->buf = NULL;
    buf->bufsize = 0;
    buf->bufindex = 0;
  }
}

bool mathify_arena_recreate(arena_buf_t *buf, size_t size) {
  buf->buf = (char*)realloc(buf->buf, size);
  if (buf->buf == NULL) {
    ERR("Memory ReAllocate Failure");
    return true;
  }
  buf->bufsize = size;
  return false;
}

bool mathify_arena_push(arena_buf_t *buf, void *data, size_t data_size) {
  if (buf->bufindex + data_size > buf->bufsize) {
    if (mathify_arena_recreate(buf, buf->bufsize * 2)) { return true; }
  }
  char *cdata = (char*)data;
  for (size_t i = 0; i < data_size; ++i) {
    buf->buf[buf->bufindex + i] = cdata[i];
  }
  buf->bufindex += data_size;
  return false;
}