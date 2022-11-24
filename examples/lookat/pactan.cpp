#include "pactan.hpp"

#include <fmt/core.h>
#include <imgui.h>
#include <tiny_obj_loader.h>

#include <cppitertools/itertools.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/hash.hpp>
#include <unordered_map>

void Pactan::initializeGL(GLuint program, std::string assetsPath) {
  m_model.loadDiffuseTexture(assetsPath + "maps/pactan.jpg");
  m_model.loadFromFile(assetsPath + "pactan.obj");
  m_model.setupVAO(program);

  computeModelMatrix();
}

void Pactan::paintGL(GLuint program, glm::mat4 cameraViewMatrix) {
  GLint modelMatrixLoc{glGetUniformLocation(program, "modelMatrix")};
  GLint normalMatrixLoc{glGetUniformLocation(program, "normalMatrix")};
  GLint shininessLoc{glGetUniformLocation(program, "shininess")};
  GLint KaLoc{glGetUniformLocation(program, "Ka")};
  GLint KdLoc{glGetUniformLocation(program, "Kd")};
  GLint KsLoc{glGetUniformLocation(program, "Ks")};
  GLint mappingModeLoc{glGetUniformLocation(program, "mappingMode")};
  glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &m_modelMatrix[0][0]);

  auto modelViewMatrix{glm::mat4(cameraViewMatrix * m_modelMatrix)};
  glm::mat3 normalMatrix{glm::inverseTranspose(modelViewMatrix)};
  glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrix[0][0]);

  glUniform1f(shininessLoc, m_model.m_shininess);
  glUniform4fv(KaLoc, 1, &m_model.m_Ka.x);
  glUniform4fv(KdLoc, 1, &m_model.m_Kd.x);
  glUniform4fv(KsLoc, 1, &m_model.m_Ks.x);

  glUniform1i(mappingModeLoc, 3);
  m_model.render();
}
void Pactan::computeGravity(float deltaTime) {
  m_velocidadePulo = m_velocidadePulo + Pactan::GRAVIDADE * deltaTime;
  m_posicao.y = m_posicao.y + m_velocidadePulo * deltaTime;

  if (m_posicao.y < Pactan::NIVEL_CHAO) {
    m_velocidadePulo = 0;
    m_posicao.y = Pactan::NIVEL_CHAO;
  }
}

void Pactan::pular() {
  
  if (m_posicao.y == Pactan::NIVEL_CHAO) {
    m_velocidadePulo = Pactan::JUMP_POWER;
  }
}

void Pactan::update(float deltaTime) {
  float rotacao = m_velocidadeRotacao * deltaTime;
  float distancia = m_velocidadeDeslocamento * deltaTime;

  float dx = distancia * glm::sin(glm::radians(m_rotacao.y));

  float dz = distancia * glm::cos(glm::radians(m_rotacao.y));

  m_rotacao.y += rotacao;
  m_posicao += glm::vec3(dx, 0, dz);

  computeGravity(deltaTime);

  computeModelMatrix();
}

void Pactan::computeModelMatrix() {
  auto model = glm::mat4{1};

  m_modelMatrix = glm::translate(model, m_posicao);

  m_modelMatrix =
      glm::rotate(m_modelMatrix, glm::radians(m_rotacao.x), glm::vec3(1, 0, 0));
  m_modelMatrix =
      glm::rotate(m_modelMatrix, glm::radians(m_rotacao.y), glm::vec3(0, 1, 0));
  m_modelMatrix =
      glm::rotate(m_modelMatrix, glm::radians(m_rotacao.z), glm::vec3(0, 0, 1));
  m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(m_escala));
}

Pactan::~Pactan() {}
