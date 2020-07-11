/*
 * TODO
 * handle ioctl size change
 */
#include <string.h>
#include <signal.h>

#include "bytebuffer.h"
#include "log.h"

#include "colors.h"
#include "font.h"
#include "terminal.h"
#include "parse.h"
#include "pseudo.h"
#include "render.h"

atlas_settings atlas = (atlas_settings){
  .size = STBTT_POINT_SIZE(19),
  .atlas_x = 2048,
  .atlas_y = 2048,
  .oversample_x = 1,
  .oversample_y = 1,
  .first = 0,
  .count = 128 
};

int main(int argc, char** argv) {
  set_loglevel(LOG_ALL);
  log_debug("Program started");
  GLFWwindow* win;
  font_data font;
  font_load(argv[1], atlas, &font);

  struct terminal term;
  terminal_init(&term, 80, 30, &font);
  pseudo_init(&term, 80, 30);
  test_setcells(&term);

  win = window_create("win", &term);
  GLuint ftexture = make_font_texture(&font);

  // Framerate
  int frames = 0;
  double prevFrameTime = glfwGetTime();

  while(!glfwWindowShouldClose(win)) {
    double currentTime = glfwGetTime();

    char buffer[100] = "\0";
    int n;
    if((n = pseudo_listen(&term, buffer, 100)) > 0 || currentTime - prevFrameTime >= 1.0) {
      //log_debug("terminal: %s", buffer);
      //terminal_write(&term, buffer, n);
      parse(&term, buffer, n);

    }
    float ratio;
    int width, height;

    glfwGetFramebufferSize(win, &width, &height);
    glViewport(0, 0, width, height);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glfw_render(win, &term, ftexture);
    glfwSwapBuffers(win);
    glfwPollEvents();

    if(currentTime - prevFrameTime >= 1.0) {
      //log_notice("FPS: %d", frames);
      frames = 0;
      prevFrameTime = currentTime;
    }
    frames++;
  }

  window_destroy(win);
  font_free(&font);
  pseudo_kill(&term);
}
