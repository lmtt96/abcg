#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "gamedata.hpp"
class OpenGLWindow;
class Pactan;

class Camera {
 public:
  void computeViewMatrix();
  void computeProjectionMatrix(int width, int height);
  void initialize();

  void alterarDistancia(float mouseWhell);
  void pitch(float mouse);
  void angulo(float mouse);

 private:
  friend OpenGLWindow;
  friend Pactan;

  glm::mat4 m_viewMatrix;
  glm::mat4 m_projMatrix;

  glm::vec3 m_posicao{0, 5, 0};

  float m_pitch{10};
  float m_yaw{0};
  float m_roll{0};

  float m_distancia{6};
  float m_anguloTarget{0};

  void update(glm::vec3 targetPosition, glm::vec3 pacmanRotation);
  void updatePosition(glm::vec3 targetPosition, glm::vec3 pacmanRotation);
};

#endif
