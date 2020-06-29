#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#define MIN(A,B) (((A)<(B))?(A):(B))
#define MAX(A,B) (((A)>(B))?(A):(B))

typedef struct {
  int size;
  int atlas_x;
  int atlas_y;
  int oversample_x;
  int oversample_y;
  int first;
  int count;
} atlas_settings;

typedef struct {
  unsigned char* atlas;
  atlas_settings setting;
  stbtt_packedchar* chardata;
  float max_width;
  float max_height;
} font_data;

bool font_load(const char* fontname, atlas_settings setting, font_data* ret) {
  if(!ret) {
    log_debug("%s: given argument (ret) is NULL", __func__);
    return 0;
  }

  int fd = open(fontname, O_RDONLY);
  if(fd < 0) {
    log_error("Failed to open font");
    return 0;
  }

  int fsize = lseek(fd, 0, SEEK_END);
  lseek(fd, 0, SEEK_SET);
  char* fdata = malloc(fsize);
  if(read(fd, fdata, fsize) != fsize) {
    log_error("%s: Failed to read font. Font file size : %d", __func__, fsize);
    return 0;
  }

  ret->chardata = malloc(sizeof(stbtt_packedchar) * setting.count);
  ret->atlas = malloc(setting.atlas_x * setting.atlas_y);
  ret->setting = setting;

  // Do stb font jobs
  stbtt_pack_context context;
  stbtt_PackBegin(&context, ret->atlas, setting.atlas_x, setting.atlas_y, 0, 1, NULL);
  stbtt_PackSetOversampling(&context, 4,4);
  if(!stbtt_PackFontRange(&context, fdata, 0, setting.size, setting.first, setting.count, ret->chardata)) {
    log_error("%s: Failed to pack font", __func__);
    exit(EXIT_FAILURE);
  }
  stbtt_PackEnd(&context);


  // Find Max width and height
  stbtt_aligned_quad q;
  ret->max_width = 0;
  ret->max_height = 0;
  for(int c = 0; c < setting.count; c++) {
    float offx = 0, offy = 0;
    stbtt_GetPackedQuad(ret->chardata, 
        setting.atlas_x, 
        setting.atlas_y, 
        c, 
        &offx,
        &offy, &q, 0);
    ret->max_width = MAX(ret->max_width, q.x1 - q.x0);
    ret->max_height = MAX(ret->max_height, q.y1 - q.y0);
  }

  free(fdata);
  close(fd);
  return 1;
}

void terminal_pixel_size(font_data* f, unsigned int cols, unsigned int rows, unsigned int* x, unsigned int* y) {
  *x = f->max_width * cols;
  *y = f->max_height * rows;
}

void font_free(font_data* f) {
  free(f->atlas);
  free(f->chardata);
}
