#include "food.hpp"

#include <fmt/core.h>
#include <imgui.h>
#include <tiny_obj_loader.h>

#include <cppitertools/itertools.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/hash.hpp>
#include <random>
#include <unordered_map>

void Food::initializeGL(GLuint program, std::string assetsPath) {
  m_food = gerarFood();
  m_model.loadDiffuseTexture(assetsPath + "/maps/food.jpg");
  m_model.loadFromFile(assetsPath + "cube.obj");
  m_model.setupVAO(program);

  auto seed{std::chrono::steady_clock::now().time_since_epoch().count()};
  m_randomEngine.seed(seed);
}

void Food::update(float deltaTime, glm::vec3 pacmanPosition, float* score) {
  // rotaciona a comida e verifica se colidiu com o player
  m_rotacao.z += 30 * deltaTime;
  for (ulong i = 0; i < m_food.size(); i++) {
    if (m_food[i]->ativo) {
      if (checkColision(m_food[i]->posicao, pacmanPosition)) {
        m_food[i]->ativo = false;
        *score += m_food[i]->valor;
      }
    }
  }
}
bool Food::checkColision(glm::vec3 foodPosition,
                             glm::vec3 pacmanPosition) {
  glm::vec3 distance = foodPosition - pacmanPosition;
  return glm::length(distance) < 1.3;
}
void Food::computeModelMatrix(int positionIndex) {
  auto model = glm::mat4{1};

  m_modelMatrix = glm::translate(model, m_food[positionIndex]->posicao);

  m_modelMatrix =
      glm::rotate(m_modelMatrix, glm::radians(m_rotacao.x), glm::vec3(1, 0, 0));
  m_modelMatrix =
      glm::rotate(m_modelMatrix, glm::radians(m_rotacao.y), glm::vec3(0, 1, 0));
  m_modelMatrix =
      glm::rotate(m_modelMatrix, glm::radians(m_rotacao.z), glm::vec3(0, 0, 1));
  m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(m_escala));
}
void Food::paintGL(GLuint program, glm::mat4 cameraViewMatrix) {
  // se a comida foi capturada pelo player, nao sera mais renderizada.
  GLint modelMatrixLoc{glGetUniformLocation(program, "modelMatrix")};
  GLint normalMatrixLoc{glGetUniformLocation(program, "normalMatrix")};
  GLint shininessLoc{glGetUniformLocation(program, "shininess")};
  GLint KaLoc{glGetUniformLocation(program, "Ka")};
  GLint KdLoc{glGetUniformLocation(program, "Kd")};
  GLint KsLoc{glGetUniformLocation(program, "Ks")};
  GLint mappingModeLoc{glGetUniformLocation(program, "mappingMode")};

  for (ulong i = 0; i < m_food.size(); i++) {
    if (m_food[i]->ativo) {
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
}

std::vector<FoodUnidade*> Food::gerarFood() {
  std::uniform_real_distribution<float> rd(-5.0f, 5.0f);

  std::vector<FoodUnidade*> foods{};
  for (int i = 0; i < 40; i++) {
    for (int j = 0; j < 40; j++) {
      float randX = rd(m_randomEngine) + ((float)(i + j) * 8);
      float randZ = (rd(m_randomEngine) + i) * (float(j * 2));
      FoodUnidade* food =
          criaFoodUnidade(100, glm::vec3(randX, 1, randZ));
      foods.push_back(food);
    }
  }
  return foods;
}

FoodUnidade* Food::criaFoodUnidade(float valor, glm::vec3 posicao) {
  auto food = (FoodUnidade*)malloc(sizeof(FoodUnidade));
  food->ativo = true;
  food->posicao = posicao;
  food->valor = valor;
  return food;
}

void Food::terminateGL() {}

Food::~Food() {}
