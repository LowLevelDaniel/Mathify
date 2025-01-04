#ifndef XPU_ARENA_IMPLEMENTED
#define XPU_ARENA_IMPLEMENTED

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h> // bool, true, false
#include <stddef.h> // size_t

struct arena_buf {
  char *buf;
  size_t bufsize;
  size_t bufindex;
};
typedef struct arena_buf arena_buf_t;

bool mathify_arena_create(arena_buf_t *buf, size_t size);
void mathify_arena_delete(arena_buf_t *buf);
bool mathify_arena_recreate(arena_buf_t *buf, size_t size);
bool mathify_arena_push(arena_buf_t *buf, void *data, size_t data_size);

#ifdef __cplusplus
}
#endif
#endif // XPU_ARENA_IMPLEMENTED