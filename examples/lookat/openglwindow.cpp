#include "openglwindow.hpp"

#include <fmt/core.h>
#include <imgui.h>
#include <tiny_obj_loader.h>

#include <cppitertools/itertools.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/hash.hpp>
#include <unordered_map>

#include "gamedata.hpp"

void OpenGLWindow::handleEventUpPressed() {
  m_pactan.m_velocidadeDeslocamento = Pactan::VELOCIDADE_DESCOLAMENTO;
}
void OpenGLWindow::handleEventDownPressed() {
  m_pactan.m_velocidadeDeslocamento = -Pactan::VELOCIDADE_DESCOLAMENTO;
}
void OpenGLWindow::handleEventLeftPressed() {
  m_pactan.m_velocidadeRotacao = Pactan::VELOCIDADE_ROTACAO;
}
void OpenGLWindow::handleEventRightPressed() {
  m_pactan.m_velocidadeRotacao = -Pactan::VELOCIDADE_ROTACAO;
}
void OpenGLWindow::handleEventSpaceBarPressed() { m_pactan.pular(); }
void OpenGLWindow::handleEventUpReleased() {
  m_pactan.m_velocidadeDeslocamento = 0.0f;
}
void OpenGLWindow::handleEventDownReleased() {
  m_pactan.m_velocidadeDeslocamento = 0.0f;
}
void OpenGLWindow::handleEventLeftReleased() {
  m_pactan.m_velocidadeRotacao = 0.0f;
}
void OpenGLWindow::handleEventRightReleased() {
  m_pactan.m_velocidadeRotacao = 0.0f;
}
void OpenGLWindow::handleEvent(SDL_Event& ev) {
  const float deltaTime{static_cast<float>(getDeltaTime())};

  // ao utilizar o scroll do mouse, altera a distancia da camera e o pactan
  if (ev.type == SDL_MOUSEWHEEL) {
    m_camera.alterarDistancia(ev.wheel.y);
  }

  // ao movimentar o mouse, a camera altera o angulo em relacao ao pactan
  if (ev.type == SDL_MOUSEMOTION) {
    float deltaX = ev.motion.xrel * deltaTime;
    float deltaY = ev.motion.yrel * deltaTime;
    m_camera.angulo(deltaX);
    m_camera.pitch(deltaY);
  }
  // movimentos
  if (ev.type == SDL_KEYDOWN) {
    if (ev.key.keysym.sym == SDLK_UP || ev.key.keysym.sym == SDLK_w) {
      handleEventUpPressed();
    }
    if (ev.key.keysym.sym == SDLK_DOWN || ev.key.keysym.sym == SDLK_s) {
      handleEventDownPressed();
    }

    if (ev.key.keysym.sym == SDLK_LEFT || ev.key.keysym.sym == SDLK_a) {
      handleEventLeftPressed();
    }

    if (ev.key.keysym.sym == SDLK_RIGHT || ev.key.keysym.sym == SDLK_d) {
      handleEventRightPressed();
    }

    if (ev.key.keysym.sym == SDLK_SPACE) {
      handleEventSpaceBarPressed();
    }
  }
  if (ev.type == SDL_KEYUP) {
    if (ev.key.keysym.sym == SDLK_UP || ev.key.keysym.sym == SDLK_w) {
      handleEventUpReleased();
    }

    if (ev.key.keysym.sym == SDLK_DOWN || ev.key.keysym.sym == SDLK_s) {
      handleEventDownReleased();
    }

    if (ev.key.keysym.sym == SDLK_LEFT || ev.key.keysym.sym == SDLK_a) {
      handleEventLeftReleased();
    }
    if (ev.key.keysym.sym == SDLK_RIGHT || ev.key.keysym.sym == SDLK_d) {
      handleEventRightReleased();
    }
  }
}

void OpenGLWindow::initializeGL() {
  abcg::glClearColor(skyColor.x, skyColor.y, skyColor.z, 1);

  abcg::glEnable(GL_DEPTH_TEST);

  auto assetsPath = getAssetsPath();

  m_program =
      createProgramFromFile(assetsPath + "shaders/texture_neblina.vert",
                            assetsPath + "shaders/texture_neblina.frag");

  m_wall.initializeGL(m_program, assetsPath);

  m_pactan.initializeGL(m_program, assetsPath);

  m_camera.initialize();
  m_fantasma.initializeGL(m_program, assetsPath);
  m_food.initializeGL(m_program, assetsPath);

  resizeGL(getWindowSettings().width, getWindowSettings().height);
}

void OpenGLWindow::paintGL() {
  update();

  abcg::glEnable(GL_DEPTH_TEST);
  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  abcg::glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  abcg::glUseProgram(m_program);

  GLint viewMatrixLoc{glGetUniformLocation(m_program, "viewMatrix")};
  GLint projMatrixLoc{glGetUniformLocation(m_program, "projMatrix")};

  GLint lightDirLoc{glGetUniformLocation(m_program, "lightDirWorldSpace")};

  GLint skyColorLoc{glGetUniformLocation(m_program, "skyColor")};

  GLint IaLoc{glGetUniformLocation(m_program, "Ia")};
  GLint IdLoc{glGetUniformLocation(m_program, "Id")};
  GLint IsLoc{glGetUniformLocation(m_program, "Is")};

  GLint diffuseTexLoc{glGetUniformLocation(m_program, "diffuseTex")};

  glUniform4fv(IsLoc, 1, &m_Is.x);
  glUniform4fv(IdLoc, 1, &m_Id.x);
  glUniform4fv(IaLoc, 1, &m_Ia.x);
  glUniform1i(diffuseTexLoc, 0);
  glUniform4fv(lightDirLoc, 1, &m_lightDir.x);
  glUniform3fv(skyColorLoc, 1, &skyColor.x);

  glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &m_camera.m_viewMatrix[0][0]);
  glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &m_camera.m_projMatrix[0][0]);

  m_wall.paintGL(m_program, m_camera.m_viewMatrix);
  m_pactan.paintGL(m_program, m_camera.m_viewMatrix);

  m_fantasma.paintGL(m_program, m_camera.m_viewMatrix);
  m_food.paintGL(m_program, m_camera.m_viewMatrix);
  glUseProgram(0);
}

void OpenGLWindow::paintUI() {
  abcg::OpenGLWindow::paintUI();
  ImGuiStyle* style = &ImGui::GetStyle();

  style->WindowPadding = ImVec2(15, 15);
  style->WindowRounding = 5.0f;
  style->FramePadding = ImVec2(5, 5);
  style->FrameRounding = 4.0f;

  style->Colors[ImGuiCol_Text] = ImVec4(0.917f, 0.992f, 0.929f, 1.00f);
  style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
  style->Colors[ImGuiCol_WindowBg] = ImVec4(0.121f, 0.552f, 0.176f, 1.00f);
  style->Colors[ImGuiCol_Border] = ImVec4(0.0f, 1.0f, 0.06f, 0.88f);
  style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.917f, 0.992f, 0.929f, 1.00f);
  {
    ImGui::SetNextWindowPos(ImVec2(5, 5));
    ImGui::Begin(" ", nullptr, ImGuiWindowFlags_NoDecoration);

    ImGui::Text("Score: %d ", m_pontuacao);

    ImGui::End();
  }
}

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;

  m_camera.computeProjectionMatrix(width, height);
}

void OpenGLWindow::terminateGL() {
  abcg::glDeleteProgram(m_program);
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}

void OpenGLWindow::update() {
  m_lightDir = glm::vec4(glm::normalize(glm::vec3(-1, -1, -1)), 0);
  const float deltaTime{static_cast<float>(getDeltaTime())};
  m_pactan.update(deltaTime);
  m_food.update(deltaTime, m_pactan.m_posicao, &m_pontuacao);
  m_camera.update(m_pactan.m_posicao, m_pactan.m_rotacao);
}
