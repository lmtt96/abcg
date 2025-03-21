#ifndef MODEL_HPP_
#define MODEL_HPP_

#include <string_view>
#include <vector>

#include "abcg.hpp"

struct Vertex {
  glm::vec3 position{};
  glm::vec3 normal{};
  glm::vec2 texCoord{};

  bool operator==(const Vertex& other) const noexcept {
    static const auto epsilon{std::numeric_limits<float>::epsilon()};
    return glm::all(glm::epsilonEqual(position, other.position, epsilon)) &&
           glm::all(glm::epsilonEqual(normal, other.normal, epsilon)) &&
           glm::all(glm::epsilonEqual(texCoord, other.texCoord, epsilon));
  }
};

class OpenGLWindow;

class Model {
 public:
  Model() = default;
  virtual ~Model();

  Model(const Model&) = delete;
  Model(Model&&) = default;
  Model& operator=(const Model&) = delete;
  Model& operator=(Model&&) = default;

  void loadDiffuseTexture(std::string_view path);
  void loadFromFile(std::string_view path, bool standardize = false);
  void render();
  void setupVAO(GLuint program);

  glm::vec4 m_Ka{};
  glm::vec4 m_Kd{};
  glm::vec4 m_Ks{};
  float m_shininess{1.0};
  GLuint m_diffuseTexture{};

  [[nodiscard]] glm::vec4 getKa() const { return m_Ka; }
  [[nodiscard]] glm::vec4 getKd() const { return m_Kd; }
  [[nodiscard]] glm::vec4 getKs() const { return m_Ks; }
  [[nodiscard]] float getShininess() const { return m_shininess; }

  [[nodiscard]] bool isUVMapped() const { return m_hasTexCoords; }

 private:
  friend OpenGLWindow;

  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};

  std::vector<Vertex> m_vertices{};
  std::vector<GLuint> m_indices{};

  bool m_hasNormals{false};
  bool m_hasTexCoords{false};

  void createBuffers();
  void standardize();
  void computeNormals();
};

#endif
