#ifndef Food_HPP_
#define Food_HPP_

#include <random>
#include <vector>

#include "abcg.hpp"
#include "camera.hpp"
#include "gamedata.hpp"
#include "model.hpp"

typedef struct FoodUnidade {
  bool ativo{0};
  int qtd{100};
  glm::vec3 posicao{0};
} FoodUnidade;

class OpenGLWindow;
class Food {
 public:
  ~Food();

  void initializeGL(GLuint program, std::string assetsPath);
  void paintGL(GLuint program, glm::mat4 cameraViewMatrix);
  void terminateGL();
  void computeModelMatrix(int positionIndex);
  std::vector<FoodUnidade*> gerarFood();
  void update(float deltaTime, glm::vec3 pacmanPosition, float* score);

 private:
  friend OpenGLWindow;
  Model m_model;
  glm::mat4 m_modelMatrix{0};
  std::default_random_engine m_randomEngine;
  FoodUnidade* criaFoodUnidade(float valor, glm::vec3 posicao);
  bool checkColision(glm::vec3 foodPosition, glm::vec3 pacmanPosition);
  std::vector<FoodUnidade*> m_food{};
  glm::vec3 m_rotacao{-90, 0, 0};
  float m_escala{0.005f};
};

#endif
