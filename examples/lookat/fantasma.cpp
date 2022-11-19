#include "fantasma.hpp"

#include <fmt/core.h>
#include <imgui.h>
#include <tiny_obj_loader.h>

#include <cppitertools/itertools.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/hash.hpp>
#include <random>
#include <unordered_map>

void Fantasma::initializeGL(GLuint program, std::string assetsPath) {
  m_posicoes = gerarPosicoes();
  //m_model.loadDiffuseTexture(assetsPath +
                            // "maps/phantom.jpg");
  m_model.loadFromFile(assetsPath +
                       "phantom.obj");
  m_model.setupVAO(program);

  auto seed{std::chrono::steady_clock::now().time_since_epoch().count()};
  m_randomEngine.seed(seed);
}
void Fantasma::computeModelMatrix(int positionIndex) {
  auto model = glm::mat4{1};

  m_modelMatrix = glm::translate(model, m_posicoes[positionIndex]);

  m_modelMatrix =
      glm::rotate(m_modelMatrix, glm::radians(m_rotacao.x), glm::vec3(1, 0, 0));
  m_modelMatrix =
      glm::rotate(m_modelMatrix, glm::radians(m_rotacao.y), glm::vec3(0, 1, 0));
  m_modelMatrix =
      glm::rotate(m_modelMatrix, glm::radians(m_rotacao.z), glm::vec3(0, 0, 1));
  m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(m_escala));
}
void Fantasma::paintGL(GLuint program, glm::mat4 cameraViewMatrix) {
  GLint modelMatrixLoc{glGetUniformLocation(program, "modelMatrix")};
  GLint normalMatrixLoc{glGetUniformLocation(program, "normalMatrix")};
  GLint shininessLoc{glGetUniformLocation(program, "shininess")};
  GLint KaLoc{glGetUniformLocation(program, "Ka")};
  GLint KdLoc{glGetUniformLocation(program, "Kd")};
  GLint KsLoc{glGetUniformLocation(program, "Ks")};
  GLint mappingModeLoc{glGetUniformLocation(program, "mappingMode")};

  for (ulong i = 0; i < m_posicoes.size(); i++) {
    computeModelMatrix(i);
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
}

std::vector<glm::vec3> Fantasma::gerarPosicoes() {
  std::uniform_real_distribution<float> rd(-5.0f, 5.0f);

  std::vector<glm::vec3> posicoes{};
  for (int i = 0; i < 30; i++) {
    for (int j = 0; j < 30; j++) {
      float randX = rd(m_randomEngine) + ((float)(i + j) * 8);
      float randZ = (rd(m_randomEngine) + i) * (float(j * 5));
      posicoes.push_back(glm::vec3(randX, 0, randZ));
    }
  }
  return posicoes;
}

void Fantasma::terminateGL() {}
Fantasma::~Fantasma() {}
