#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Defines several options for camera movement.
// Used as abstraction to stay away from window-system specific input.
enum class Camera_Dir {
   Forward,
   Backward,
   Left,
   Right,
};

// Default camera values
static const float YAW = -90.f;
static const float PITCH = 0.0f;
static const float SPEED = 2.5f;
static const float SENSITIVITY = 0.1f;
static const float ZOOM = 45.0f;

/* An abstract camera class that processes input and calculates the
   corresponding Euler Angles, Vectors, and Matrices for use. */
class Camera {
public:
   // Camera attributes
   glm::vec3 position;
   glm::vec3 front;
   glm::vec3 up;
   glm::vec3 right;
   glm::vec3 worldUp;
   // Euler Angles
   float yaw;
   float pitch;
   // Camera options
   float movementSpeed;
   float mouseSensitivity;
   float zoom;

   Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f),
          glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW,
          float pitch = PITCH);

   Camera(float pX, float pY, float pZ, float upX, float upY, float upZ,
          float yaw = YAW, float pitch = PITCH);

   glm::mat4 getViewMatrix();
   void processKeyboard(Camera_Dir direction, float deltaTime);
   void processMouseMovement(float xoffset, float yoffset,
                             GLboolean constrainPitch = true);
   void processMouseScroll(float yoffset);

private:
   void updateCameraVectors();
};

#endif
