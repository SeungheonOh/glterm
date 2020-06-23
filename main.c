#include <string.h>

#include "bytebuffer.h"
#include "log.h"

#include "colors.h"
#include "font.h"
#include "terminal.h"
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
  GLFWwindow* win;
  font_data font;
  load_font(argv[1], atlas, &font);

  struct terminal term;
  init_terminal(&term, 80, 24, &font);
  test_setcells(&term);

  win = create_window("win", term.width, term.height);
  GLuint ftexture = make_font_texture(&font);

  while(!glfwWindowShouldClose(win)) {
    float ratio;
    int width, height;
    glfwGetFramebufferSize(win, &width, &height);
    glViewport(0, 0, width, height);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.f, 1.f, 1.f);
    glfw_render(win, &term, ftexture);

    glfw_render_rect_meme(win);

    glfwSwapBuffers(win);
    glfwPollEvents();
  }

  destroy_window(win);
  free_font(&font);
}
