#include <stdlib.h>
#include <string.h>

typedef struct {
  char* b;
  char* p;
  size_t alloc_size;
} bytebuffer;

bytebuffer* bytebuffer_init(size_t s) {
  bytebuffer* b = malloc(sizeof(bytebuffer));
  b->alloc_size = s;
  b->b = calloc(b->alloc_size, sizeof(char));
  b->p = b->b;
  return b;
}

size_t bytebuffer_size(bytebuffer* b) {
  return b->p - b->b;
}

/*
 * Check if there's enough space to write 's' bytes
 * if space is not enough allocate with 2x space
 */
void _bytebuffer_alloc(bytebuffer* b, size_t s) {
  if(bytebuffer_size(b) + s < b->alloc_size) return;
  if(b->alloc_size * 2 < 0) return;

  char* newptr = realloc(b->b, b->alloc_size * 2);
  b->alloc_size *= 2;
  b->p = newptr + bytebuffer_size(b);
  b->b = newptr;
}

void bytebuffer_append(bytebuffer* b, char* c, size_t s) {
  _bytebuffer_alloc(b, s);
  memcpy(b->p, c, s);
  b->p += s;
}

void bytebuffer_clear(bytebuffer* b) {
  b->p = b->b;
}

void bytebuffer_free(bytebuffer* b) {
  free(b->b);
}
