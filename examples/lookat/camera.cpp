#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "pactan.hpp"

void Camera::computeProjectionMatrix(int width, int height) {
  m_projMatrix = glm::mat4(1.0f);
  const auto aspect{static_cast<float>(width) / static_cast<float>(height)};
  m_projMatrix = glm::perspective(glm::radians(70.0f), aspect, 0.1f, 1000.0f);
}

void Camera::computeViewMatrix() {
  auto viewMatrix = glm::mat4{1};

  // rotaciona a camera no eixo X, realizando o  pitch
  viewMatrix =
      glm::rotate(viewMatrix, glm::radians(m_pitch), glm ::vec3(1, 0, 0));

  // rotaciona a camera no eixo y, realizando o yaw
  viewMatrix =
      glm::rotate(viewMatrix, glm::radians(m_yaw), glm ::vec3(0, 1, 0));

  // deslocamento da camera
  viewMatrix = glm::translate(viewMatrix, -m_posicao);

  m_viewMatrix = viewMatrix;
}

void Camera::update(glm::vec3 pacmanPosition, glm::vec3 pacmanRotation) {
  updatePosition(pacmanPosition, pacmanRotation);

  computeViewMatrix();
}

void Camera::updatePosition(glm::vec3 pacmanPosition,
                            glm::vec3 pacmanRotation) {
 
  // movimento do mouse atual
  float novaDistanciaX = m_distancia * glm::cos(glm::radians(m_pitch));
  float novaDistanciaY = m_distancia * glm::sin(glm::radians(m_pitch));

  float anguloCamera = pacmanRotation.y + m_anguloTarget;

  float deslocamentoX =
      static_cast<float>(novaDistanciaX * glm::sin(glm::radians(anguloCamera)));
  float deslocamentoZ =
      static_cast<float>(novaDistanciaX * glm::cos(glm::radians(anguloCamera)));

  m_posicao.x = pacmanPosition.x - deslocamentoX;
  m_posicao.z = pacmanPosition.z - deslocamentoZ;

  m_posicao.y = pacmanPosition.y + novaDistanciaY;

  m_yaw = 180 - (pacmanRotation.y + m_anguloTarget);
}

void Camera::initialize() {}

void Camera::alterarDistancia(float mouseWheel) {
  // altera distancia da camera ate o pactan, com base no scroll do mouse
  m_distancia += mouseWheel * 0.2f;
  if (m_distancia < 3.0f) {
    m_distancia = 3.0f;
    return;
  }
  if (m_distancia > 15.0f) {
    m_distancia = 15.0f;
    return;
  }
}

void Camera::pitch(float mouse) {
  // movimento de pitch, com base no movimento relativo do mouse
  float delta = mouse * 35.0f;
  m_pitch += delta;
  if (m_pitch < 10) {
    m_pitch = 10;
    return;
  }
  if (m_pitch > 95) {
    m_pitch = 95;
    return;
  }
}

void Camera::angulo(float mouse) {
  // gira a camera em torno do pactan;
  float delta = mouse * 35.0f;
  m_anguloTarget -= delta;
}
