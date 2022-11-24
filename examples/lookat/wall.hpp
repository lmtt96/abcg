#ifndef Wall_HPP_
#define Wall_HPP_

#include <vector>

#include "abcg.hpp"
#include "camera.hpp"
#include "gamedata.hpp"
#include "model.hpp"
class OpenGLWindow;
class Wall {
 public:
  ~Wall();

  void initializeGL(GLuint program, std::string assetsPath);
  void paintGL(GLuint program, glm::mat4 cameraViewMatrix);
  void terminateGL();
  void update(float deltaTime);

 private:
  friend OpenGLWindow;
  friend Camera;
  Model m_model;
  glm::mat4 m_modelMatrix{1.0f};

  glm::vec3 m_posicaoInicial{glm::vec3(0)};
  glm::vec3 m_posicaoFinal{glm::vec3(1, 0, 0)};
};

#endif
