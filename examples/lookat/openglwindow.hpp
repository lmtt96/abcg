#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <vector>

#include "abcg.hpp"
#include "fantasma.hpp"
#include "camera.hpp"
#include "wall.hpp"
#include "food.hpp"
#include "pactan.hpp"
#include "gamedata.hpp"
#include "model.hpp"
class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void handleEvent(SDL_Event& ev) override;
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;

 private:
  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};
  GLuint m_program{};

  int m_viewportWidth{};
  int m_viewportHeight{};

  Camera m_camera;
  Fantasma m_fantasma;
  Food m_food;
  float m_pontuacao{0};
  Wall m_wall;
  Pactan m_pactan;

  glm::vec4 m_lightDir{-1.0f, -1.0f, -1.0f, 0.0f};
  glm::vec4 m_Ia{1.0f, 1.0f, 1.0f, 1.0f};
  glm::vec4 m_Id{1.0f, 1.0f, 1.0f, 1.0f};
  glm::vec4 m_Is{1.0f, 1.0f, 1.0f, 1.0f};
  glm::vec3 skyColor{0.04f, 0.055f, 0.106f};

  void update();
  void handleEventUpPressed();
  void handleEventDownPressed();
  void handleEventLeftPressed();
  void handleEventRightPressed();
  void handleEventSpaceBarPressed();
  void handleEventUpReleased();
  void handleEventDownReleased();
  void handleEventLeftReleased();
  void handleEventRightReleased();
};

#endif
