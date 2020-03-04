#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED),
      mouseSensitivity(SENSITIVITY), zoom(ZOOM)
{
   this->position = position;
   this->worldUp = up;
   this->yaw = yaw;
   this->pitch = pitch;
   updateCameraVectors();
}

Camera::Camera(float pX, float pY, float pZ, float upX, float upY, float upZ,
               float yaw, float pitch)
    : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED),
      mouseSensitivity(SENSITIVITY), zoom(ZOOM)
{
   this->position = glm::vec3(pX, pY, pZ);
   this->worldUp = glm::vec3(upX, upY, upZ);
   this->yaw = yaw;
   this->pitch = pitch;
   updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix()
{
   return glm::lookAt(position, position + front, up);
}

void Camera::processKeyboard(Camera_Dir direction, float deltaTime)
{
   float velocity = movementSpeed * deltaTime;
   if (direction == Camera_Dir::Forward) { position += front * velocity; }
   if (direction == Camera_Dir::Backward) { position -= front * velocity; }
   if (direction == Camera_Dir::Left) { position -= right * velocity; }
   if (direction == Camera_Dir::Right) { position += right * velocity; }
}

void Camera::processMouseMovement(float xoffset, float yoffset,
                                  GLboolean constrainPitch)
{
   xoffset *= mouseSensitivity;
   yoffset *= mouseSensitivity;

   yaw += xoffset;
   pitch += yoffset;

   if (constrainPitch) {
      if (pitch > 89.0f) { pitch = 89.0f; }
      if (pitch < -89.0f) { pitch = -89.0f; }
   }

   updateCameraVectors();
}

void Camera::processMouseScroll(float yoffset)
{
   if (zoom >= 1.0f && zoom <= 45.0f) { zoom -= yoffset; }
   if (zoom <= 1.0f) { zoom = 1.0f; }
   if (zoom >= 45.0f) { zoom = 45.0f; }
}


void Camera::updateCameraVectors()
{
   glm::vec3 front;
   front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
   front.y = sin(glm::radians(pitch));
   front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
   this->front = glm::normalize(front);

   this->right = glm::normalize(glm::cross(this->front, this->worldUp));
   this->up = glm::normalize(glm::cross(this->right, this->front));
}
