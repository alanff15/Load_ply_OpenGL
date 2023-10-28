#include <happly.h>

#include "Model.h"

// #include "../../res/app.glsl"

Model::Model(std::string path) {
  this->path = path;
  meanPosition = glm::vec3(0);
}

Model::~Model() {
  vao.release();
  vb.release();
  ib.release();
  shader.release();
  texture.release();
}

void Model::load() {
  happly::PLYData model(path + "/model.ply");
  std::vector<float> vertex_x = model.getElement("vertex").getProperty<float>("x");
  std::vector<float> vertex_y = model.getElement("vertex").getProperty<float>("y");
  std::vector<float> vertex_z = model.getElement("vertex").getProperty<float>("z");
  std::vector<float> vertex_nx = model.getElement("vertex").getProperty<float>("nx");
  std::vector<float> vertex_ny = model.getElement("vertex").getProperty<float>("ny");
  std::vector<float> vertex_nz = model.getElement("vertex").getProperty<float>("nz");
  std::vector<float> vertex_s = model.getElement("vertex").getProperty<float>("texture_u");
  std::vector<float> vertex_t = model.getElement("vertex").getProperty<float>("texture_v");
  std::vector<std::vector<size_t>> faces = model.getFaceIndices();
  // vertices
  std::vector<float> data(8 * vertex_x.size());
  meanPosition = glm::vec3(0);
  for (int i = 0; i < vertex_x.size(); i++) {
    data[8 * i + 0] = vertex_x[i];
    data[8 * i + 1] = vertex_y[i];
    data[8 * i + 2] = vertex_z[i];
    data[8 * i + 3] = vertex_nx[i];
    data[8 * i + 4] = vertex_ny[i];
    data[8 * i + 5] = vertex_nz[i];
    data[8 * i + 6] = vertex_s[i];
    data[8 * i + 7] = vertex_t[i];
    meanPosition += glm::vec3(vertex_x[i], vertex_y[i], vertex_z[i]);
  }
  meanPosition /= vertex_x.size();
  // indices
  std::vector<uint32_t> indices(3 * faces.size());
  for (int i = 0; i < faces.size(); i++) {
    indices[3 * i + 0] = (uint32_t)faces[i][0];
    indices[3 * i + 1] = (uint32_t)faces[i][1];
    indices[3 * i + 2] = (uint32_t)faces[i][2];
  }
  // texture
  texture = std::make_unique<Texture>(path + "/textures/texture.jpg");
  vao = std::make_unique<VertexArray>();
  vb = std::make_unique<VertexBuffer>(&data[0], (uint32_t)(data.size() * sizeof(float)));
  VertexBufferLayout vbl;
  vbl.Push<float>(3);  // shader 0: position
  vbl.Push<float>(3);  // shader 1: normal
  vbl.Push<float>(2);  // shader 2: texCoord
  vao->AddBuffer(*vb, vbl);
  ib = std::make_unique<IndexBuffer>(&indices[0], (uint32_t)indices.size());
  // shader
  shader = std::make_unique<Shader>("../../res/app.glsl", StringType::FILEPATH);
  // shader = std::make_unique<Shader>(GLSL_STR, StringType::PROGRAM);
}

void Model::SetUniformMat4f(const std::string& name, const glm::mat4& matrix) {
  shader->Bind();
  shader->SetUniformMat4f(name, matrix);
}

void Model::SetUniform1i(const std::string& name, int value) {
  texture->Bind(0);
  shader->SetUniform1i(name, value);
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

glm::vec3 Model::getMeanPosition() {
  return meanPosition;
}
