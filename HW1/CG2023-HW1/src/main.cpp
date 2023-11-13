/*back render missing*/
/*fluent*/
/*bonus*/
/*report*/

#include <algorithm>
#include <memory>
#include <vector>
#include <iostream>

#include <GLFW/glfw3.h>
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#undef GLAD_GL_IMPLEMENTATION
#include <glm/glm.hpp>

#include "camera.h"
#include "opengl_context.h"
#include "utils.h"

#define ANGLE_TO_RADIAN(x) (float)((x)*M_PI / 180.0f) 
#define RADIAN_TO_ANGEL(x) (float)((x)*180.0f / M_PI) 

#define CIRCLE_SEGMENT 64

#define ROTATE_SPEED 1.0f
#define FLYING_SPEED ROTATE_SPEED / 20.f

#define RED 0.905f, 0.298f, 0.235f
#define BLUE 0.203f, 0.596f, 0.858f
#define GREEN 0.18f, 0.8f, 0.443f

/*for airplane control*/
bool spaceKeyPressed = false;
bool leftKeyPressed = false;
bool rightKeyPressed = false;
bool upKeyPressed = false;
bool downKeyPressed = false;
bool eKeyPressed = false;
bool rKeyPressed = false;
bool space_released = false;
float speed_frame = 0.05;

void resizeCallback(GLFWwindow* window, int width, int height) {
  OpenGLContext::framebufferResizeCallback(window, width, height);
  auto ptr = static_cast<Camera*>(glfwGetWindowUserPointer(window));
  if (ptr) {
    ptr->updateProjectionMatrix(OpenGLContext::getAspectRatio());
  }
}

void keyCallback(GLFWwindow* window, int key, int, int action, int) {
  // There are three actions: press, release, hold(repeat)
  /*if (action == GLFW_REPEAT) 
      return;
    */
  
  // Press ESC to close the window.
  if (key == GLFW_KEY_ESCAPE) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
    return;
  }
  if (key == GLFW_KEY_SPACE) {
    if (action == GLFW_PRESS) {
      spaceKeyPressed = true;
    } else if (action == GLFW_RELEASE) {
      spaceKeyPressed = false;
      space_released = true;
    }
  }

  /*yaw*/
  if (key == GLFW_KEY_LEFT) {
    if (action == GLFW_PRESS) {
      leftKeyPressed = true;
    } else if (action == GLFW_RELEASE) {
      leftKeyPressed = false;
    }
  }

  if (key == GLFW_KEY_RIGHT) {
    if (action == GLFW_PRESS) {
      rightKeyPressed = true;
    } else if (action == GLFW_RELEASE) {
      rightKeyPressed = false;
    }
  }
  
  /*pitch*/
  if (key == GLFW_KEY_UP) {
    if (action == GLFW_PRESS) {
      upKeyPressed = true;
    } else if (action == GLFW_RELEASE) {
      upKeyPressed = false;
    }
  }
  if (key == GLFW_KEY_DOWN) {
    if (action == GLFW_PRESS) {
      downKeyPressed = true;
    } else if (action == GLFW_RELEASE) {
      downKeyPressed = false;
    }
  }

  /*row*/
  if (key == GLFW_KEY_E) {
    if (action == GLFW_PRESS) {
      eKeyPressed = true;
    } else if (action == GLFW_RELEASE) {
      eKeyPressed = false;
    }
  }
  if (key == GLFW_KEY_R) {
    if (action == GLFW_PRESS) {
      rKeyPressed = true;
    } else if (action == GLFW_RELEASE) {
      rKeyPressed = false;
    }
  }
  /* TODO#4-1: Detect key-events, perform rotation or fly
   *       1. Use switch && case to find the key you want.
   *       2. Press "SPACE" for fly up, fly forward and wing rotate meanwhile. 
   *       3. Press "GLFW_KEY_LEFT" for turn left.
   *       4. Press "GLFW_KEY_RIGHT " for turn right.
   * Hint:
   *       glfw3's key list (https://www.glfw.org/docs/3.3/group__keys.html)
   *       glfw3's action codes (https://www.glfw.org/docs/3.3/group__input.html#gada11d965c4da13090ad336e030e4d11f)
   * Note:
   *       You should finish rendering your airplane first.
   *       Otherwise you will spend a lot of time debugging this with a black screen.
   */
}

void initOpenGL() {
  // Initialize OpenGL context, details are wrapped in class.
#ifdef __APPLE__
  // MacOS need explicit request legacy support
  OpenGLContext::createContext(21, GLFW_OPENGL_ANY_PROFILE);
#else
  //OpenGLContext::createContext(21, GLFW_OPENGL_ANY_PROFILE);
  OpenGLContext::createContext(43, GLFW_OPENGL_COMPAT_PROFILE);
#endif
  GLFWwindow* window = OpenGLContext::getWindow();
  /* TODO#0: Change window title to "HW1 - `your student id`"
   *        Ex. HW1 - 312550000 
   */
  glfwSetWindowTitle(window, "HW1 - 109550017");
  glfwSetKeyCallback(window, keyCallback);
  glfwSetFramebufferSizeCallback(window, resizeCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
#ifndef NDEBUG
  OpenGLContext::printSystemInfo();
  // This is useful if you want to debug your OpenGL API calls.
  OpenGLContext::enableDebugCallback();
#endif
}

/* TODO#2: Render body, wing, tail
 * Hint:
 *       glBegin/glEnd (https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glBegin.xml)
 *       glColor3f (https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glBegin.xml)
 *       glVertex3f (https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glVertex.xml)
 *       glNormal (https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glNormal.xml)
 *       glScalef (https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glScale.xml)
 * Note:
 *       You can refer to ppt "Draw Cylinder" page and `CIRCLE_SEGMENT`
 *       You can use color defined above
 */
void draw_body() {
    glBegin(GL_QUAD_STRIP);
    glColor3f(BLUE);

    float height = 4.0f;
    float radius = 0.5f;

    for (int i = 0; i <= CIRCLE_SEGMENT; i++) {
        float angle = 2.0f * M_PI * (static_cast<float>(i) / 64.0f);

        float x1 = radius * cos(angle);
        float y1 = radius * sin(angle);
        float z1 = height / 2.0f;

        float x2 = radius * cos(angle);
        float y2 = radius * sin(angle);
        float z2 = -height / 2.0f;
        
        y1 += radius;
        y2 += radius;
        
        float nx = x1;
        float ny = y1;
        float nz = 0.0f;

        glNormal3f(nx, ny, nz);

        glVertex3f(x1, y1, z1);
        glVertex3f(x2, y2, z2);
    }
    glEnd();

    /*front circle*/
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(BLUE);  
    for (int i = 0; i <= CIRCLE_SEGMENT; i++) {
      float angle = 2.0f * M_PI * (static_cast<float>(i) / CIRCLE_SEGMENT);

      float x = radius * cos(angle);
      float y = radius * sin(angle);
      float z = height / 2.0f;

        y += radius;

      float nx = 0.0f;
      float ny = 0.0f;
      float nz = 1.0f;

      glNormal3f(nx, ny, nz);

      glVertex3f(x, y, z);
    }
    glEnd();

    /*back circle*/
    /*it will disappear at first, but let i=circle_segment fix that*/
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(BLUE);  
    for (int i = CIRCLE_SEGMENT; i >= 0; i--) {
      float angle = 2.0f * M_PI * (static_cast<float>(i) / CIRCLE_SEGMENT);

      float x = radius * cos(angle);
      float y = radius * sin(angle);
      float z = -height / 2.0f;
      y += radius;

      float nx = 0.0f;
      float ny = 0.0f;
      float nz = -1.0f;

      glNormal3f(nx, ny, nz);

      glVertex3f(x, y, z);
    }
    glEnd();
}

void draw_right_wing() {
  float length = 4.0f;
  float width = 1.0f;
  float height = 0.5f;

  float cylinder_radius = 0.5f;

  float x = 0.0f;
  float y = cylinder_radius + height/2.0f;
  float z = width/2.0f;

  float normalX = 1.0f;
  float normalY = 1.0f;
  float normalZ = 1.0f;


  /*top*/
  glBegin(GL_QUADS);
  glColor3f(RED); 
  glNormal3f(0.0f, normalY, 0.0f); // position affects color...
  glVertex3f(x, y, z);
  glVertex3f(x + length, y, z);
  glVertex3f(x + length, y, z - width);
  glVertex3f(x, y, z - width);
  glEnd();
  
  /*bottom*/
  /*clock-wise*/
  glBegin(GL_QUADS);
  glColor3f(RED);
  glNormal3f(0.0f, -normalY, 0.0f);
  glVertex3f(x, y - height, z);
  glVertex3f(x, y - height, z - width);
  glVertex3f(x + length, y - height, z - width);
  glVertex3f(x + length, y - height, z);
  glEnd();
  
  /*front*/
  /*okay...so the rendering order is counter-clockwise*/
  glBegin(GL_QUADS);
  glColor3f(RED);  
  glNormal3f(0.0f, 0.0f, normalZ); 
  glVertex3f(x, y, z);
  glVertex3f(x, y - height, z);
  glVertex3f(x + length, y - height, z);
  glVertex3f(x + length, y, z);
  glEnd();
  

  /*back*/
  glBegin(GL_QUADS);
  glColor3f(RED);
  glNormal3f(0.0f, 0.0f, -normalZ);
  glVertex3f(x, y, z - width);
  glVertex3f(x + length, y, z - width);
  glVertex3f(x + length, y - height, z - width);
  glVertex3f(x, y - height, z - width);
  glEnd();
  

  
  /*right*/
  /*i don't know.. but here is clock-wise*/
  glBegin(GL_QUADS);
  glColor3f(RED);
  glNormal3f(normalX, 0.0f, 0.0f);
  glVertex3f(x + length, y, z);
  glVertex3f(x + length, y - height, z);
  glVertex3f(x + length, y - height, z - width);
  glVertex3f(x + length, y, z - width);
  
  glEnd();

  /*left*/
  /*i don't know.. but here is clock-wise*/
  glBegin(GL_QUADS);
  glColor3f(RED);
  glNormal3f(-normalX, 0.0f, 0.0f);
  glVertex3f(x, y, z);
  glVertex3f(x, y, z - width);
  glVertex3f(x, y - height, z - width);
  glVertex3f(x, y - height, z);
  glEnd();

}

void draw_left_wing() {
  float length = 4.0f;
  float width = 1.0f;
  float height = 0.5f;

  float cylinder_radius = 0.5f;

  float x = - length;
  float y = cylinder_radius + height / 2.0f;
  float z = width / 2.0f;

  float normalX = 1.0f;
  float normalY = 1.0f;
  float normalZ = 1.0f;

  /*top*/
  glBegin(GL_QUADS);
  glColor3f(RED);
  glNormal3f(0.0f, normalY, 0.0f);  // position affects color...
  glVertex3f(x, y, z);
  glVertex3f(x + length, y, z);
  glVertex3f(x + length, y, z - width);
  glVertex3f(x, y, z - width);
  glEnd();

  /*bottom*/
  /*clock-wise*/
  glBegin(GL_QUADS);
  glColor3f(RED);
  glNormal3f(0.0f, -normalY, 0.0f);  // position affects color...
  glVertex3f(x, y - height, z);
  glVertex3f(x, y - height, z - width);
  glVertex3f(x + length, y - height, z - width);
  glVertex3f(x + length, y - height, z);
  glEnd();

  /*front*/
  /*okay...so the rendering order is counter-clockwise*/
  glBegin(GL_QUADS);
  glColor3f(RED);
  glNormal3f(0.0f, 0.0f, normalZ);
  glVertex3f(x, y, z);
  glVertex3f(x, y - height, z);
  glVertex3f(x + length, y - height, z);
  glVertex3f(x + length, y, z);
  glEnd();

  /*back*/
  glBegin(GL_QUADS);
  glColor3f(RED);
  glNormal3f(0.0f, 0.0f, -normalZ);
  glVertex3f(x, y, z - width);
  glVertex3f(x + length, y, z - width);
  glVertex3f(x + length, y - height, z - width);
  glVertex3f(x, y - height, z - width);
  glEnd();

  /*right*/
  /*i don't know.. but here is clock-wise*/
  glBegin(GL_QUADS);
  glColor3f(RED);
  glNormal3f(normalX, 0.0f, 0.0f);
  glVertex3f(x + length, y, z);
  glVertex3f(x + length, y, z - width);
  glVertex3f(x + length, y - height, z - width);
  glVertex3f(x + length, y - height, z);
  glEnd();

  /*left*/
  /*i don't know.. but here is clock-wise*/
  glBegin(GL_QUADS);
  glColor3f(RED);
  glNormal3f(-normalX, 0.0f, 0.0f);
  glVertex3f(x, y, z);
  glVertex3f(x, y, z - width);
  glVertex3f(x, y - height, z - width);
  glVertex3f(x, y - height, z);
  glEnd();
}


void drawTail() {

    float cylinder_radius = 0.5f;

  float bottomEdge = 2.0f;
  float height1 = 1.0f;
  float height2 = 0.5f;

  float x = 0.0f;
  float y = cylinder_radius;
  float z = 2.0f;

  float normalX = 1.0f;
  float normalY = 1.0f;
  float normalZ = 1.0f;

  /*top*/
  /*okay.. counter-clockwise*/
  glBegin(GL_TRIANGLES);
  glColor3f(GREEN);  
  glNormal3f(0.0f, normalY, 0.0f);
  glVertex3f(x, y, z);
  glVertex3f(x - bottomEdge / 2, y, z + height1);
  glVertex3f(x + bottomEdge / 2.0f, y, z + height1);
  glEnd();

  
  /*back*/
  glBegin(GL_TRIANGLES);
  glColor3f(GREEN);
  glNormal3f(0.0f, 0.0f, normalZ);
  glVertex3f(x + bottomEdge / 2.0f, y, z + height1);
  glVertex3f(x - bottomEdge / 2, y, z + height1);
  glVertex3f(x, y - height2, z + height1);
  glEnd();
  
  /*right*/
  /*counter-clockwise*/
  glBegin(GL_TRIANGLES);
  glColor3f(GREEN);
  glm::vec3 vec1(0, -height2, height1);
  glm::vec3 vec2(bottomEdge / 2.0f, 0.0f, height1);
  glm::vec3 normal = glm::cross(vec1, vec2);
  glNormal3f(normal[0], normal[1], normal[2]);

  glVertex3f(x, y, z);
  glVertex3f(x + bottomEdge / 2.0f, y, z + height1);
  glVertex3f(x, y - height2, z + height1);
  glEnd();

  
  /*left*/
  glBegin(GL_TRIANGLES);
  glColor3f(GREEN);
  glm::vec3 vec3(0, -height2, height1);
  glm::vec3 vec4(- bottomEdge / 2.0f, 0.0f, height1);
  glm::vec3 normal1 = glm::cross(vec3, vec4);
  glNormal3f(normal1[0], normal1[1], normal1[2]);
  glVertex3f(x, y, z);
  glVertex3f(x, y - height2, z + height1);
  glVertex3f(x - bottomEdge / 2.0f, y, z + height1);
  glEnd();

}

void drawTail_save() {
  float cylinder_radius = 0.5f;

  float bottomEdge = 2.0f;
  float height1 = 1.0f;
  float height2 = 0.5f;

  float x = 0.0f;
  float y = cylinder_radius;
  float z = -2.0f;

  float normalX = 1.0f;
  float normalY = 1.0f;
  float normalZ = 1.0f;

  /*top*/
  /*okay.. counter-clockwise*/
  glBegin(GL_TRIANGLES);
  glColor3f(GREEN);
  glNormal3f(0.0f, normalY, 0.0f);
  glVertex3f(x, y, z);
  glVertex3f(x + bottomEdge / 2.0f, y, z - height1);
  glVertex3f(x - bottomEdge / 2, y, z - height1);
  glEnd();

  /*back*/
  glBegin(GL_TRIANGLES);
  glColor3f(GREEN);
  glNormal3f(0.0f, 0.0f, -normalZ);
  glVertex3f(x + bottomEdge / 2.0f, y, z - height1);
  glVertex3f(x - bottomEdge / 2, y, z - height1);
  glVertex3f(x, y - height2, z - height1);
  glEnd();

  /*right*/
  /*counter-clockwise*/
  glBegin(GL_TRIANGLES);
  glColor3f(GREEN);
  glm::vec3 vec1(0, -height2, -height1);
  glm::vec3 vec2(bottomEdge / 2.0f, 0.0f, -height1);
  glm::vec3 normal = glm::cross(vec1, vec2);
  glNormal3f(normal[0], normal[1], normal[2]);

  glVertex3f(x, y, z);
  glVertex3f(x, y - height2, z - height1);
  glVertex3f(x + bottomEdge / 2.0f, y, z - height1);
  glEnd();

  /*left*/
  glBegin(GL_TRIANGLES);
  glColor3f(GREEN);
  glm::vec3 vec3(0, -height2, -height1);
  glm::vec3 vec4(-bottomEdge / 2.0f, 0.0f, -height1);
  glm::vec3 normal1 = glm::cross(vec3, vec4);
  glNormal3f(normal1[0], normal1[1], normal1[2]);
  glVertex3f(x, y, z);
  glVertex3f(x - bottomEdge / 2.0f, y, z - height1);
  glVertex3f(x, y - height2, z - height1);
  glEnd();
}


float wing_angle = 0;
float wing_speed = 0.5;
void drawAirplaneComponents() {
  draw_body();

  glPushMatrix();
  if (spaceKeyPressed) {
    if (wing_angle > 20 || wing_angle < -20) {
      wing_angle -= wing_speed;
      wing_speed = -wing_speed;
      
    }
    glRotatef(wing_angle, 0.0f, 0.0f, 1.0f);
    wing_angle += wing_speed;
    
  }
  draw_right_wing();
  glPopMatrix();

  glPushMatrix();
  if (spaceKeyPressed) {
    if (wing_angle > 20 || wing_angle < -20) {
      wing_angle -= wing_speed;
      wing_speed = -wing_speed;
    }
    glRotatef(-wing_angle, 0.0f, 0.0f, 1.0f);
    wing_angle += wing_speed;
  }
  draw_left_wing();
  glPopMatrix();

  drawTail();
}


void light() {
  GLfloat light_specular[] = {0.6, 0.6, 0.6, 1};
  GLfloat light_diffuse[] = {0.6, 0.6, 0.6, 1};
  GLfloat light_ambient[] = {0.4, 0.4, 0.4, 1};
  GLfloat light_position[] = {50.0, 75.0, 80.0, 1.0};
  // z buffer enable
  glEnable(GL_DEPTH_TEST);
  // enable lighting
  glEnable(GL_LIGHTING);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_NORMALIZE);
  // set light property
  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
}

int main() {
  initOpenGL();
  GLFWwindow* window = OpenGLContext::getWindow();

  // Init Camera helper
  Camera camera(glm::vec3(0, 5, 10));
  camera.initialize(OpenGLContext::getAspectRatio());
  // Store camera as glfw global variable for callbasks use
  glfwSetWindowUserPointer(window, &camera);

  // Main rendering loop
  glm::vec3 plane_pos(0.0f, 0.0f, 0.0f);
  glm::vec3 plane_dir(0.0f, 1.0f, -1.0f);
  plane_dir = glm::normalize(plane_dir);
  plane_dir = plane_dir * 0.05f;
  float rotate_x = 0;
  float rotate_y = 0;
  float rotate_z = 0;
  float angle = 1.0f;
  while (!glfwWindowShouldClose(window)) {
    // Polling events.
    glfwPollEvents();
    // Update camera position and view
    camera.move(window);
    // GL_XXX_BIT can simply "OR" together to use.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /// TO DO Enable DepthTest
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    // Projection Matrix
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(camera.getProjectionMatrix());
    // ModelView Matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(camera.getViewMatrix());


#ifndef DISABLE_LIGHT   
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearDepth(1.0f);
    light();
#endif

    /* TODO#4-2: Update 
     *       You may update position and orientation of airplane here or not.
     *       Feel free to not follow TA's structure. However, don't violate the spec. 
     * 
     * Hint: 
     * Note:
     *       You can use `ROTATE_SPEED` and `FLYING_SPEED` as the speed constant. 
     *       If the rotate/flying speed is too slow or too fast, please change `ROTATE_SPEED` or `FLYING_SPEED` value.
     *       You should finish keyCallback first.
     */

    // Render a white board
    glPushMatrix();
    glScalef(3, 1, 3);
    glBegin(GL_TRIANGLE_STRIP);
    glColor3f(1.0f, 1.0f, 1.0f);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-5.0f, 0.0f, -5.0f);
    glVertex3f(-5.0f, 0.0f, 5.0f);
    glVertex3f(5.0f, 0.0f, -5.0f);
    glVertex3f(5.0f, 0.0f, 5.0f);
    glEnd();
    glPopMatrix();
    

    

    glPushMatrix();
    glTranslatef(plane_pos.x, plane_pos.y, plane_pos.z);
    if (spaceKeyPressed) {
      space_released = false;
      plane_pos += plane_dir;
    }
    if (space_released) {
      float cur_y = plane_pos.y - plane_dir.y;
      if (cur_y <= 0) {
        plane_pos.y = 0;
        space_released = false;
      } else {
        plane_pos.x += plane_dir.x;
        plane_pos.y -= plane_dir.y;
        plane_pos.z += plane_dir.z;
      }      
    }

    if (spaceKeyPressed || space_released) {
      if (leftKeyPressed) {
        glm::mat4 rotationMatrix_y = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
        plane_dir = glm::vec3(rotationMatrix_y * glm::vec4(plane_dir, 0.0f));
        rotate_y += angle;
      } 
      else if (rightKeyPressed) {
        glm::mat4 rotationMatrix_y = glm::rotate(glm::mat4(1.0f), glm::radians(-angle), glm::vec3(0.0f, 1.0f, 0.0f));
        plane_dir = glm::vec3(rotationMatrix_y * glm::vec4(plane_dir, 0.0f));
        rotate_y -= angle;
      } 
      else if (upKeyPressed) {
        glm::mat4 rotationMatrix_x = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
        plane_dir = glm::vec3(rotationMatrix_x * glm::vec4(plane_dir, 0.0f));
        rotate_x += angle;
      } 
      else if (downKeyPressed) {
        glm::mat4 rotationMatrix_x = glm::rotate(glm::mat4(1.0f), glm::radians(-angle), glm::vec3(1.0f, 0.0f, 0.0f));
        plane_dir = glm::vec3(rotationMatrix_x * glm::vec4(plane_dir, 0.0f));
        rotate_x -= angle;
      } 
      else if (eKeyPressed) {
        glm::mat4 rotationMatrix_z = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
        plane_dir = glm::vec3(rotationMatrix_z * glm::vec4(plane_dir, 0.0f));
        rotate_z += angle;
      } 
      else if (rKeyPressed) {          
        glm::mat4 rotationMatrix_z = glm::rotate(glm::mat4(1.0f), glm::radians(-angle), glm::vec3(0.0f, 0.0f, 1.0f));
        plane_dir = glm::vec3(rotationMatrix_z * glm::vec4(plane_dir, 0.0f));
        rotate_z -= angle;
      }
    }
    glRotatef(rotate_z, 0.0f, 0.0f, 1.0f);
    glRotatef(rotate_y, 0.0f, 1.0f, 0.0f);
    glRotatef(rotate_x, 1.0f, 0.0f, 0.0f);

    
    drawAirplaneComponents();
    glPopMatrix();
  
   
    /* TODO#3: Render the airplane    
     *       1. Render the body.
     *       2. Render the wings.(Don't forget to assure wings rotate at the center of body.)
     *       3. Render the tail.
     * Hint:
     *       glPushMatrix/glPopMatrix (https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glPushMatrix.xml)
     *       glRotatef (https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glRotate.xml)
     *       glTranslatef (https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glTranslate.xml) 
     *       glColor3f (https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glColor.xml)
     *       glScalef (https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glScale.xml)
     * Note:
     *       You may implement functions for drawing components of airplane first
     *       You should try and think carefully about changing the order of rotate and translate
     */
    

#ifdef __APPLE__
    // Some platform need explicit glFlush
    glFlush();
#endif
    glfwSwapBuffers(window);
  }
  return 0;
}
