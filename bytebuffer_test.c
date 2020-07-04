#include <stdio.h>
#include "bytebuffer.h"

int main() {
  bytebuffer* b = bytebuffer_init(5);
  bytebuffer_append(b, "abcde", 5);
  printf("%d %d\n", bytebuffer_size(b), b->alloc_size);
  bytebuffer_append(b, "abcde", 3);
  printf("%d %d\n", bytebuffer_size(b), b->alloc_size);
  bytebuffer_append(b, "abcde", 3);
  for(int i = 0; i < 50000; i++) {
    bytebuffer_append(b, "a", 1);
    printf("%d %d\n", bytebuffer_size(b), b->alloc_size);
  }
}
