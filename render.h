#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GLFW/glfw3.h>

typedef struct {
  unsigned int x;
  unsigned int y;
} window_context;

static void glfw_error_callback(int error, const char* desc) {
  log_error("glfw error: %d %s", error, desc);
}

char buf[10000] = "\0";
static void glfw_text_callback(GLFWwindow* window, unsigned int codepoint) {
  buf[strlen(buf)] = codepoint;
}

static void glfw_keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if(action) log_info("keypress: %d(%c) %d %d %d", key, key, scancode, action, mods);
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GL_TRUE);
}

static void glfw_button_callback(GLFWwindow* window, int button, int action, int mods) {
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);
  log_info("mouse press at %f %f\n", xpos, ypos);
}

static void glfw_resize(GLFWwindow* window, int x, int y) {
  glViewport(0, 0, x, y);
}

void glfw_render(GLFWwindow* win, struct terminal* term, GLuint ftexture) {
  int width, height;
  glfwGetFramebufferSize(win, &width, &height);

  glLoadIdentity();
  glOrtho(0, width, height, 0, 0, 1);
    for(int l = 0; l < term->rows; l++) {
    float offsetx = 0, offsety = (term->font->max_height) * (l+1);
    for(int i = 0; i < term->cols; i++) {
      struct cell target_cell = term->screen[i + l * term->cols];
      if(!target_cell.c) continue;
      stbtt_aligned_quad q;
      offsetx = (term->font->max_width) * i;
      stbtt_GetPackedQuad(term->font->chardata, 
          term->font->setting.atlas_x, 
          term->font->setting.atlas_y, 
          target_cell.c, 
          &offsetx, &offsety, &q, 0);
      
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, ftexture);
      glActiveTexture(ftexture);
      glBegin(GL_QUADS);
      glColor3f((float)RED(target_cell.fg)/255, 
          (float)GREEN(target_cell.fg)/255, 
          (float)BLUE(target_cell.fg)/255);
      glTexCoord2f(q.s0,q.t0);  glVertex2f(q.x0, q.y0);
      glTexCoord2f(q.s1,q.t0);  glVertex2f(q.x1, q.y0);
      glTexCoord2f(q.s1,q.t1);  glVertex2f(q.x1, q.y1);
      glTexCoord2f(q.s0,q.t1);  glVertex2f(q.x0, q.y1);
      glEnd();
      glDisable(GL_TEXTURE_2D);

      /* BG need to figure out how to make texture transparent
      glBegin(GL_QUADS);
      glColor3f((float)RED(target_cell.bg)/255, 
          (float)GREEN(target_cell.bg)/255, 
          (float)BLUE(target_cell.bg)/255);
      glVertex2f(offsetx-term->font->max_width, offsety-term->font->max_height);
      glVertex2f(offsetx,                       offsety-term->font->max_height);
      glVertex2f(offsetx,                       offsety);
      glVertex2f(offsetx-term->font->max_width, offsety);
      glEnd();
      */
    }
  }
  glDisable(GL_TEXTURE_2D);
}

void glfw_render_rect_meme(GLFWwindow* win) {
  glClear(GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  glEnable(GL_DEPTH_TEST);
  glRotatef(sin((float) glfwGetTime()) * 100.f, 0.f, 0.f, 1.f);
  glRotatef(sin((float) glfwGetTime()) * 100, 1.0, 0.0, 0.0 );
  glRotatef(sin((float) glfwGetTime()) * 100, 0.0, 1.0, 0.0 );
  glBegin(GL_QUADS);
  glColor3f( 1.0, 1.0, 0.0 );     
  glVertex3f(  0.5, -0.5, -0.5 );
  glVertex3f(  0.5,  0.5, -0.5 );
  glVertex3f( -0.5,  0.5, -0.5 );
  glVertex3f( -0.5, -0.5, -0.5 );
  glColor3f(   1.0,  1.0, 1.0 );
  glVertex3f(  0.5, -0.5, 0.5 );
  glVertex3f(  0.5,  0.5, 0.5 );
  glVertex3f( -0.5,  0.5, 0.5 );
  glVertex3f( -0.5, -0.5, 0.5 );
  glColor3f(  1.0,  0.0,  1.0 );
  glVertex3f( 0.5, -0.5, -0.5 );
  glVertex3f( 0.5,  0.5, -0.5 );
  glVertex3f( 0.5,  0.5,  0.5 );
  glVertex3f( 0.5, -0.5,  0.5 );
  glColor3f(   0.0,  1.0,  0.0 );
  glVertex3f( -0.5, -0.5,  0.5 );
  glVertex3f( -0.5,  0.5,  0.5 );
  glVertex3f( -0.5,  0.5, -0.5 );
  glVertex3f( -0.5, -0.5, -0.5 );
  glColor3f(   0.0,  0.0,  1.0 );
  glVertex3f(  0.5,  0.5,  0.5 );
  glVertex3f(  0.5,  0.5, -0.5 );
  glVertex3f( -0.5,  0.5, -0.5 );
  glVertex3f( -0.5,  0.5,  0.5 );
  glColor3f(   1.0,  0.0,  0.0 );
  glVertex3f(  0.5, -0.5, -0.5 );
  glVertex3f(  0.5, -0.5,  0.5 );
  glVertex3f( -0.5, -0.5,  0.5 );
  glVertex3f( -0.5, -0.5, -0.5 );
  glEnd();
  glDisable(GL_DEPTH_TEST);
}

GLFWwindow* create_window(char* name, unsigned int x, unsigned int y) {
  glfwSetErrorCallback(glfw_error_callback);

  GLFWwindow* win;
  if (!glfwInit()) {
    log_error("%s: Failed to initialize glfw", __func__);
    exit(EXIT_FAILURE);
  }
  win = glfwCreateWindow(x, y, name, NULL, NULL);
  if (!win){
    glfwTerminate();
    log_error("%s: Failed to create window", __func__);
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(win);
  glfwSwapInterval(0); // 0 = no frame limit, 1 = limit frame
  
  // callbacks
  glfwSetKeyCallback(win, glfw_keyboard_callback);
  glfwSetMouseButtonCallback(win, glfw_button_callback);
  glfwSetCharCallback(win, glfw_text_callback);
  return win;
}

void destroy_window(GLFWwindow* win) {
  glfwDestroyWindow(win);
  glfwTerminate();
}

GLuint make_font_texture(font_data* font) {
  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, 
      font->setting.atlas_x, font->setting.atlas_y, 
      0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, font->atlas);
  glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
  return texture;
}
