#ifndef Fantasma_HPP_
#define Fantasma_HPP_

#include <random>
#include <vector>

#include "abcg.hpp"
#include "camera.hpp"
#include "gamedata.hpp"
#include "model.hpp"
class OpenGLWindow;
class Fantasma {
 public:
  ~Fantasma();

  void initializeGL(GLuint program, std::string assetsPath);
  void paintGL(GLuint program, glm::mat4 cameraViewMatrix);
  void terminateGL();
  void computeModelMatrix(int positionIndex);
  std::vector<glm::vec3> gerarPosicoes();

 private:
  friend OpenGLWindow;
  Model m_model;
  glm::mat4 m_modelMatrix{1.0f};
  std::default_random_engine m_randomEngine;

  std::vector<glm::vec3> m_posicoes{};
  glm::vec3 m_rotacao{-90, 0, 0};
  float m_escala{0.01};
};

#endif
