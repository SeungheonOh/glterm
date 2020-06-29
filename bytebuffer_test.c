#include "bytebuffer.h"
#include <stdio.h>

int main() {
  bytebuffer* b = init_bytebuffer(5);
  append_bytebuffer(b, "abcde", 5);
  printf("%d %d\n", size_bytebuffer(b), b->alloc_size);
  append_bytebuffer(b, "abcde", 3);
  printf("%d %d\n", size_bytebuffer(b), b->alloc_size);
  append_bytebuffer(b, "abcde", 3);
  printf("%d %d\n", size_bytebuffer(b), b->alloc_size);
  for(int i = 0; i < 50000; i++) {
    append_bytebuffer(b, "a", 1);
    printf("%d %d\n", size_bytebuffer(b), b->alloc_size);
  }
}
