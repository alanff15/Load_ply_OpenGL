#pragma once

#include <memory>

#include "Renderer/Renderer.h"
#include "Renderer/VertexBufferLayout.h"

class Model {
public:
  Model();
  ~Model();

  void load();

  void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

  const VertexArray& getVao();
  const IndexBuffer& getIb();
  const Shader& getShader();

private:
  std::unique_ptr<VertexArray> vao;
  std::unique_ptr<VertexBuffer> vb;
  std::unique_ptr<IndexBuffer> ib;
  std::unique_ptr<Shader> shader;
};