#include "App.h"

#include <iostream>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Renderer/GLDebug.h"
#include "Renderer/splat_renderer.hpp"
#include "Renderer/happly.h"

#include <memory>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/quaternion.hpp>

#ifndef M_PI
#define M_PI ((float)3.14159265358979323846)
#endif

namespace App {
bool mouse_b1 = false, mouse_b2 = false;
glm::dvec2 mouse_anchor;
float cameraHeight = 80;

glm::vec3 center(-25, 4, 0);
glm::vec3 eye(-25, 4, cameraHeight);
glm::vec3 up(0, 1, 0);
glm::vec3 lastCenter = center;
glm::vec3 lastEye = eye;
glm::vec3 localRefZ, localRefY, localRefX;

GLviz::Camera g_camera;
std::unique_ptr<SplatRenderer> viz;
std::vector<Surfel> g_surfels;

void load_points() {
  std::vector<Eigen::Vector3f> vertices, normals;
  std::vector<std::array<unsigned int, 3>> faces;

  happly::PLYData mesh("../../res/points.ply");
  std::vector<std::array<double, 3>> vPos = mesh.getVertexPositions();
  std::vector<std::array<double, 3>> vNormals = mesh.getVertexNormals();
  std::vector<std::array<unsigned char, 3>> vColor = mesh.getVertexColors();

  g_surfels.resize(vPos.size());
  for (int i = 0; i < vPos.size(); i++) {
    Eigen::Vector3f dz = {(float)vNormals[i][0], (float)vNormals[i][1], (float)vNormals[i][2]};
    dz.normalize();
    Eigen::Vector3f dx = (dz.cross(Eigen::Vector3f::UnitZ())).normalized();
    Eigen::Vector3f dy = dz.cross(dx);
    g_surfels[i].c = Eigen::Vector3f((float)vPos[i][0], (float)vPos[i][1], (float)vPos[i][2]);
    g_surfels[i].u = 0.20f * dx;
    g_surfels[i].v = 0.20f * dy;
    g_surfels[i].p = Eigen::Vector3f::Zero();
    g_surfels[i].rgba = vColor[i][0] | (vColor[i][1] << 8) | (vColor[i][2] << 16) | (0xFF << 24);
  }
}

void resize(int width, int height) {
  const float aspect = static_cast<float>(width) / static_cast<float>(height);
  GLCall(glViewport(0, 0, width, height));
  g_camera.set_perspective(60.0f, aspect, 5.0f, 500.0f);
}

void Setup(GLFWwindow* window) {
  int w, h;
  glfwGetWindowSize(window, &w, &h);
  resize(w, h);

  glm::mat4x4 lookat_glm = glm::lookAt(eye, center, up);
  g_camera.set_modelview_matrix(*((Eigen::Matrix4f*)&lookat_glm));

  load_points();
  viz = std::unique_ptr<SplatRenderer>(new SplatRenderer(g_camera));
  viz->set_color_material(0);
  viz->set_soft_zbuffer_epsilon(1.0f);
  viz->set_backface_culling(true);
}

void Render(GLFWwindow* window) {
  viz->render_frame(g_surfels);
}

void Shutdown(GLFWwindow* window) {
  viz = nullptr;
}

void RenderInterface(GLFWwindow* window) {
  ImGui::Begin("Degub");
  ImGui::Text("FPS: %3.1f", (ImGui::GetIO()).Framerate);
  ImGui::End();
}

void CursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
  ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);  // imgui
  if (mouse_b1 || mouse_b2) {
    glm::dvec2 mouse_pos;
    glm::vec2 mouseDelta;
    glfwGetCursorPos(window, &mouse_pos.x, &mouse_pos.y);
    mouseDelta = glm::vec2(mouse_pos - mouse_anchor);
    if (mouse_b1) {
      auto rot = glm::angleAxis(mouseDelta.y / 400.f, localRefX) * glm::angleAxis(-mouseDelta.x / 400.f, localRefY);
      eye = center - cameraHeight * glm::normalize(rot * localRefZ);
      up = rot * localRefY;
    } else {
      glm::vec3 delta = (mouseDelta.x / 10.f) * localRefX + (mouseDelta.y / 10.f) * localRefY;
      center = lastCenter + delta;
      eye = lastEye + delta;
    }
    glm::mat4x4 lookat_glm = glm::lookAt(eye, center, up);
    g_camera.set_modelview_matrix(*((Eigen::Matrix4f*)&lookat_glm));
  }
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
  ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);  // imgui
  glfwGetCursorPos(window, &mouse_anchor.x, &mouse_anchor.y);
  localRefZ = glm::normalize(center - eye);
  localRefY = glm::normalize(up);
  localRefX = glm::cross(localRefY, localRefZ);
  lastEye = eye;
  lastCenter = center;
  switch (button) {
    case GLFW_MOUSE_BUTTON_1:
      mouse_b1 = (action == GLFW_PRESS);
      break;
    case GLFW_MOUSE_BUTTON_2:
      mouse_b2 = (action == GLFW_PRESS);
      break;
  }
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
  ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);  // imgui
  //
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);  // imgui
  if (action == GLFW_PRESS || action == GLFW_RELEASE) {
    switch (key) {
      case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        break;
    }
  }
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
  resize(width, height);
}

}  // namespace App