#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GLFW/glfw3.h>


#define STB_TRUETYPE_IMPLEMENTATION
#include "./stb_truetype.h"

// Glfw Callbacks
static void error_callback(int error, const char* description) {
    fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if(action)printf("%d %d %d %d\n", key, scancode, action, mods);
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}
static void text_callback(GLFWwindow* window, unsigned int codepoint) {
  printf("%d\n", codepoint);
}
static void button_callback(GLFWwindow* window, int button, int action, int mods) {
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);
  printf("%f %f\n", xpos, ypos);
}

GLuint fontTexture = 0;
stbtt_packedchar* charInfo;
int atlas_x = 2048;
int atlas_y = 2048;

// Font
void load_font() {
  int fd = open("plex.otf", O_RDONLY);
  if(fd < 0) {
    fputs("Failed to load font", stderr);
    exit(EXIT_FAILURE);
  }

  int fsize = lseek(fd, 0, SEEK_END);
  lseek(fd, 0, SEEK_SET);
  char* fontdata = malloc(fsize);
  if(read(fd, fontdata, fsize) != fsize) {
    fputs("Failed to load font", stderr);
    exit(EXIT_FAILURE);
  }

  // Do stb font jobs
  stbtt_pack_context context;
  charInfo = malloc(('~' - ' ') * sizeof(stbtt_packedchar));
  uint8_t* atlas = malloc(atlas_x * atlas_y);
  stbtt_PackBegin(&context, atlas, atlas_x, atlas_y, 0, 1, NULL);
  stbtt_PackSetOversampling(&context, 4,4);
  if(!stbtt_PackFontRange(&context, fontdata, 0, STBTT_POINT_SIZE(15), ' ', '~' - ' ', charInfo)) {
    fputs("Failed to pack font", stderr);
    exit(EXIT_FAILURE);
  }
  stbtt_PackEnd(&context);
  close(fd);

  glGenTextures(1, &fontTexture);
  glBindTexture(GL_TEXTURE_2D, fontTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, atlas_x, atlas_y, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, atlas);
  glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
  glGenerateMipmap(GL_TEXTURE_2D);
}

void draw_character(char c, float x, float y) {
  stbtt_aligned_quad q;
  float offsetx = 1, offsety = 1;
  stbtt_GetPackedQuad(charInfo, atlas_x, atlas_y, 19, &offsetx, &offsety, &q, 1);

  glEnable(GL_TEXTURE_2D);
  glBegin(GL_QUADS);
    glTexCoord2f(q.s0,q.t1);  glVertex2f(0,0);
    glTexCoord2f(q.s1,q.t1);  glVertex2f(30,0);
    glTexCoord2f(q.s1,q.t0);  glVertex2f(30,30);
    glTexCoord2f(q.s0,q.t0);  glVertex2f(0,30);
  glEnd();
  glDisable(GL_TEXTURE_2D);
}

int main(void) {

    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    window = glfwCreateWindow(500, 500, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, button_callback);

    load_font();

    //glfwSetCharCallback(window, text_callback);
    double prevFrameTime = glfwGetTime();
    int frames;
    while (!glfwWindowShouldClose(window))
    {
      double currentTime = glfwGetTime();
      frames++;
        float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;
        glViewport(0, 0, width, height);
        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glOrtho(0, width, 0, height, 0, 1);
        glMatrixMode(GL_MODELVIEW);

        /*glBindTexture(GL_TEXTURE_2D, fontTexture);*/
        /*glActiveTexture(GL_TEXTURE0);*/
        /*draw_character('a', 0,0);*/

        /*glBindTexture(GL_TEXTURE_2D, fontTexture);*/
        /*glEnable(GL_TEXTURE_2D);*/
        /*glBegin(GL_QUADS);*/
        /*glTexCoord2i(0, 0); glVertex2i(0, height);*/
        /*glTexCoord2i(0, 1); glVertex2i(0, 0);*/
        /*glTexCoord2i(1, 1); glVertex2i(width, 0);*/
        /*glTexCoord2i(1, 0); glVertex2i(width, height);*/
        /*glEnd();*/
        /*glDisable(GL_TEXTURE_2D);*/


        glLoadIdentity();
        glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
        glBegin(GL_TRIANGLES);
          glColor3f(1.f, 0.f, 0.f);
          glVertex3f(-0.6f, -0.4f, 0.f);
          glColor3f(0.f, 1.f, 0.f);
          glVertex3f(0.6f, -0.4f, 0.f);
          glColor3f(0.f, 0.f, 1.f);
          glVertex3f(0.f, 0.6f, 0.f);
        glEnd();
        glfwSwapBuffers(window);
        glfwPollEvents();

        if(currentTime - prevFrameTime >= 1.0) {
          printf("%d\n", frames);
          frames = 0;
          prevFrameTime = currentTime;
        }
    }
    glDeleteTextures(1, &fontTexture);
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

