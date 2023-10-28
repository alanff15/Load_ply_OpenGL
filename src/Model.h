#pragma once

#include <memory>

#include "Renderer/Renderer.h"
#include "Renderer/VertexBufferLayout.h"
#include "Renderer/Texture.h"

class Model {
public:
  Model(std::string path);
  ~Model();

  void load();

  void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
  void SetUniform1i(const std::string& name, int value);

  const VertexArray& getVao();
  const IndexBuffer& getIb();
  const Shader& getShader();

  glm::vec3 getMeanPosition();

private:
  std::unique_ptr<VertexArray> vao;
  std::unique_ptr<VertexBuffer> vb;
  std::unique_ptr<IndexBuffer> ib;
  std::unique_ptr<Shader> shader;
  std::unique_ptr<Texture> texture;
  std::string path;
  glm::vec3 meanPosition;
};