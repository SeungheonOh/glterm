#include <stdlib.h>
#include <string.h>

typedef struct {
  char* buf;
  char* p;
  size_t alloc_size;
} bytebuffer;

bytebuffer* bytebuffer_init(size_t s) {
  bytebuffer* b = malloc(sizeof(bytebuffer));
  b->alloc_size = s;
  b->buf = malloc(b->alloc_size);
  b->p = b->buf;
  return b;
}

void bytebuffer_append(bytebuffer* b, char* c, size_t s) {
  if((b->p - b->buf)+s >= b->alloc_size) {
    b->buf = realloc(b->buf, b->alloc_size * 2);
    b->alloc_size *= 2;
  }
  memcpy(b->p, c, s);
}

void bytebuffer_clean(bytebuffer* b) {
  b->buf = b->p;
}
