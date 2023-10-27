#include "Model.h"

// #include "../../res/app.glsl"

Model::Model() {}

Model::~Model() {
  vao.release();
  vb.release();
  ib.release();
  shader.release();
}

void Model::load() {
  // vertices
  float data[] = {
    -1.0, -1.0, 1.0,  0.0, 0.0, 1.0,  // pos(x,y,z) cor(r,g,b)
    1.0,  -1.0, 1.0,  1.0, 0.0, 1.0,  //
    1.0,  1.0,  1.0,  1.0, 1.0, 1.0,  //
    -1.0, 1.0,  1.0,  0.0, 1.0, 1.0,  //
    -1.0, -1.0, -1.0, 0.0, 0.0, 0.0,  //
    1.0,  -1.0, -1.0, 1.0, 0.0, 0.0,  //
    1.0,  1.0,  -1.0, 1.0, 1.0, 0.0,  //
    -1.0, 1.0,  -1.0, 0.0, 1.0, 0.0   //
  };
  // indices
  uint32_t indices[] = {
    0, 1, 2,  //
    2, 3, 0,  //
    1, 5, 6,  //
    6, 2, 1,  //
    7, 6, 5,  //
    5, 4, 7,  //
    4, 0, 3,  //
    3, 7, 4,  //
    4, 5, 1,  //
    1, 0, 4,  //
    3, 2, 6,  //
    6, 7, 3   //
  };
  vao = std::make_unique<VertexArray>();
  vb = std::make_unique<VertexBuffer>(data, 8 * (3 + 3) * sizeof(float));
  VertexBufferLayout vbl;
  vbl.Push<float>(3);  // shader 0: position
  vbl.Push<float>(3);  // shader 1: color
  vao->AddBuffer(*vb, vbl);
  ib = std::make_unique<IndexBuffer>(indices, 12 * 3);
  // shader
  shader = std::make_unique<Shader>("../../res/app.glsl", StringType::FILEPATH);
  // shader = std::make_unique<Shader>(GLSL_STR, StringType::PROGRAM);
}

void Model::SetUniformMat4f(const std::string& name, const glm::mat4& matrix) {
  shader->Bind();
  shader->SetUniformMat4f(name, matrix);
}

const VertexArray& Model::getVao() {
  return *vao;
}

const IndexBuffer& Model::getIb() {
  return *ib;
}

const Shader& Model::getShader() {
  return *shader;
}
