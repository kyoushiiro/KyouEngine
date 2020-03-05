#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "Shader.h"
#include "Camera.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

/* Settings */
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

/* Camera */
Camera camera(glm::vec3(0.0f, 1.0f, 4.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

/* Light */
static glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

/* Timer */
float deltaTime = 0.0f;
float lastFrame = 0.0f;

float vertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.5f,  -0.5f, -0.5f,
    0.0f,  0.0f,  -1.0f, 0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f,
    0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, -0.5f, 0.5f,  -0.5f,
    0.0f,  0.0f,  -1.0f, -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f,

    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.5f,  -0.5f, 0.5f,
    0.0f,  0.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  -0.5f, 0.5f,  0.5f,
    0.0f,  0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,

    -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  -0.5f, 0.5f,  -0.5f,
    -1.0f, 0.0f,  0.0f,  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  -0.5f, -0.5f, 0.5f,
    -1.0f, 0.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  -0.5f,
    1.0f,  0.0f,  0.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
    0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.5f,  -0.5f, 0.5f,
    1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.5f,  -0.5f, -0.5f,
    0.0f,  -1.0f, 0.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,
    0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  -0.5f, -0.5f, 0.5f,
    0.0f,  -1.0f, 0.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,

    -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  -0.5f,
    0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  0.5f,
    0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f};

static const unsigned int indices[] = {
    0, 1, 3, 1, 2, 3,
};

static const glm::vec3 cubePositions[] = {
    glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
    glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

int main()
{
   glfwInit();
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
   // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

   GLFWwindow *window =
       glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
   if (window == NULL) {
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      return -1;
   }
   glfwMakeContextCurrent(window);
   glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
   glfwSetCursorPosCallback(window, mouse_callback);
   glfwSetScrollCallback(window, scroll_callback);

   glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::cout << "Failed to initialize GLAD" << std::endl;
      return -1;
   }

   Shader ourShader("4.6.shader.vs", "4.6.shader.fs");
   Shader lightShader("4.6.lightshader.vs", "4.6.lightshader.fs");

   /*
   unsigned int texture;
   glGenTextures(1, &texture);
   glBindTexture(GL_TEXTURE_2D, texture);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   int width, height, nrChannels;
   unsigned char *data =
       stbi_load("img/container.jpg", &width, &height, &nrChannels, 0);
   if (data) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                   GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
   } else {
      std::cout << "Failed to load image data!" << std::endl;
      return -1;
   }
   stbi_image_free(data);
   */

   unsigned int VAO;
   unsigned int VBO;
   glGenVertexArrays(1, &VAO);
   glGenBuffers(1, &VBO);

   glBindVertexArray(VAO);
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                         (void *)0);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                         (void *)(3 * sizeof(float)));
   glEnableVertexAttribArray(1);
   glBindVertexArray(0);

   unsigned int lightVAO;
   glGenVertexArrays(1, &lightVAO);
   glBindVertexArray(lightVAO);
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                         (void *)0);
   glEnableVertexAttribArray(0);
   glBindVertexArray(0);

   GLuint model_loc = glGetUniformLocation(ourShader.ID, "model");
   if (model_loc == -1) {
      std::cout << "Failed to find model uniform location." << std::endl;
      return -1;
   }
   GLuint view_loc = glGetUniformLocation(ourShader.ID, "view");
   if (view_loc == -1) {
      std::cout << "Failed to find view uniform location." << std::endl;
      return -1;
   }
   GLuint proj_loc = glGetUniformLocation(ourShader.ID, "proj");
   if (proj_loc == -1) {
      std::cout << "Failed to find proj uniform location." << std::endl;
      return -1;
   }
   GLuint objectColor_loc = glGetUniformLocation(ourShader.ID, "objectColor");
   if (objectColor_loc == -1) {
      std::cout << "Failed to find objectColor uniform location. " << std::endl;
      return -1;
   }
   GLuint lightColor_loc = glGetUniformLocation(ourShader.ID, "lightColor");
   if (lightColor_loc == -1) {
      std::cout << "Failed to find lightColor uniform location. " << std::endl;
      return -1;
   }
   GLuint lightPos_loc = glGetUniformLocation(ourShader.ID, "lightPos");
   if (lightPos_loc == -1) {
      std::cout << "Failed to find lightPos uniform location. " << std::endl;
      return -1;
   }
   GLuint viewPos_loc = glGetUniformLocation(ourShader.ID, "viewPos");
   if (viewPos_loc == -1) {
      std::cout << "Failed to find viewPos uniform location. " << std::endl;
      return -1;
   }

   lightShader.use();
   GLuint smodel_loc = glGetUniformLocation(lightShader.ID, "model");
   if (smodel_loc == -1) {
      std::cout << "Failed to find smodel uniform location." << std::endl;
      return -1;
   }
   GLuint sview_loc = glGetUniformLocation(lightShader.ID, "view");
   if (sview_loc == -1) {
      std::cout << "Failed to find sview uniform location." << std::endl;
      return -1;
   }
   GLuint sproj_loc = glGetUniformLocation(lightShader.ID, "proj");
   if (sproj_loc == -1) {
      std::cout << "Failed to find sproj uniform location." << std::endl;
      return -1;
   }

   glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
   glEnable(GL_DEPTH_TEST);

   while (!glfwWindowShouldClose(window)) {
      float currentFrame = glfwGetTime();
      deltaTime = currentFrame - lastFrame;
      lastFrame = currentFrame;

      processInput(window);

      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      ourShader.use();
      glm::mat4 view = camera.getViewMatrix();
      glm::mat4 proj =
          glm::perspective(glm::radians(camera.zoom),
                           (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

      glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));
      glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(proj));
      glUniform3f(objectColor_loc, 1.0f, 0.5f, 0.31f);
      glUniform3f(lightColor_loc, 1.0f, 1.0f, 1.0f);
      glUniform3f(lightPos_loc, lightPos.x, lightPos.y, lightPos.z);
      glUniform3f(viewPos_loc, camera.position.x, camera.position.y,
                  camera.position.z);

      glBindVertexArray(VAO);
      for (unsigned int i = 0; i < 1; i++) {
         glm::mat4 model = glm::mat4(1.0f);
         model = glm::translate(model, cubePositions[i]);
         float angle = 20.0f * i;
         if (i % 3 == 0) {
            model =
                glm::rotate(model, (float)glfwGetTime() * glm::radians(angle),
                            glm::vec3(1.0, 0.3f, 0.5f));
         } else {
            model = glm::rotate(model, glm::radians(angle),
                                glm::vec3(1.0f, 0.3f, 0.5f));
         }
         glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));

         glDrawArrays(GL_TRIANGLES, 0, 36);
      }

      lightShader.use();
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, lightPos);
      model = glm::scale(model, glm::vec3(0.2f));
      glUniformMatrix4fv(smodel_loc, 1, GL_FALSE, glm::value_ptr(model));
      glUniformMatrix4fv(sview_loc, 1, GL_FALSE, glm::value_ptr(view));
      glUniformMatrix4fv(sproj_loc, 1, GL_FALSE, glm::value_ptr(proj));
      glBindVertexArray(lightVAO);
      glDrawArrays(GL_TRIANGLES, 0, 36);
      // glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

      glfwSwapBuffers(window);
      glfwPollEvents();
   }

   glDeleteVertexArrays(1, &VAO);
   glDeleteBuffers(1, &VBO);

   glfwTerminate();
   return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
   glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
   if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
   }
   if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
      camera.processKeyboard(Camera_Dir::Forward, deltaTime);
   }
   if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
      camera.processKeyboard(Camera_Dir::Backward, deltaTime);
   }
   if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
      camera.processKeyboard(Camera_Dir::Left, deltaTime);
   }
   if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
      camera.processKeyboard(Camera_Dir::Right, deltaTime);
   }
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
   if (firstMouse) {
      lastX = xpos;
      lastY = ypos;
      firstMouse = false;
   }

   float xoffset = xpos - lastX;
   float yoffset = lastY - ypos;

   lastX = xpos;
   lastY = ypos;

   camera.processMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
   camera.processMouseScroll(yoffset);
}
