#include <string.h>
#include <signal.h>

#include "bytebuffer.h"
#include "log.h"

#include "colors.h"
#include "font.h"
#include "terminal.h"
#include "pseudo.h"
#include "render.h"

atlas_settings atlas = (atlas_settings){
  .size = STBTT_POINT_SIZE(19),
  .atlas_x = 2048,
  .atlas_y = 2048,
  .oversample_x = 8,
  .oversample_y = 8,
  .first = 0,
  .count = 128 
};

int main(int argc, char** argv) {
  set_loglevel(LOG_ALL);
  log_debug("Program started");
  GLFWwindow* win;
  font_data font;
  load_font(argv[1], atlas, &font);

  struct terminal term;
  init_terminal(&term, 80, 24, &font);
  init_pseudo(&term, 80, 24);
  test_setcells(&term);

  win = create_window("win", &term);
  GLuint ftexture = make_font_texture(&font);

  pseudo_write(&term, "echo helloworld\r", strlen("echo helloworld\r"));

  while(!glfwWindowShouldClose(win)) {
    char buffer[100] = "\0";
    if(pseudo_listen(&term, buffer, 100) > 0) log_debug("terminal: %s", buffer);

    float ratio;
    int width, height;
    glfwGetFramebufferSize(win, &width, &height);
    glViewport(0, 0, width, height);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.f, 1.f, 1.f);
    glfw_render(win, &term, ftexture);

    //glfw_render_rect_meme(win);

    glfwSwapBuffers(win);
    glfwPollEvents();
  }

  destroy_window(win);
  free_font(&font);

  kill(term.pid, SIGHUP);
}
