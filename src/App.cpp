#include "App.h"

#include <iostream>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Renderer/Renderer.h"
#include "Renderer/VertexBufferLayout.h"

#include "Model.h"

#include <memory>

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#ifndef M_PI
#define M_PI ((float)3.14159265358979323846)
#endif

static float aspectaxis(float screen_width, float screen_height) {
  float outputzoom = 1.0f;
  float aspectorigin = 16.0f / 9.0f;
  int aspectconstraint = 1;
  switch (aspectconstraint) {
    case 1:
      if ((screen_width / screen_height) < aspectorigin) {
        outputzoom *= (((float)screen_width / screen_height) / aspectorigin);
      } else {
        outputzoom *= ((float)aspectorigin / aspectorigin);
      }
      break;
    case 2:
      outputzoom *= (((float)screen_width / screen_height) / aspectorigin);
      break;
    default:
      outputzoom *= ((float)aspectorigin / aspectorigin);
  }
  return outputzoom;
}

static float recalculatefov(float screen_width, float screen_height) {
  return 2.0f * glm::atan(glm::tan(glm::radians(45.0f / 2.0f)) / aspectaxis(screen_width, screen_height));
}

namespace App {

Renderer renderer;

glm::vec3 eye = glm::vec3(0.0, 0.0, 10.0);
glm::vec3 center = glm::vec3(0.0, 0.0, 0.0);
glm::vec3 angles = glm::vec3(0.0, 0.0, 0.0);

bool mouse_b1 = false;
glm::vec3 angle_anchor;
glm::dvec2 mouse_anchor;

Model caixa;

void Setup(GLFWwindow* window) {
  // blend
  GLCall(glEnable(GL_BLEND));
  GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
  GLCall(glEnable(GL_DEPTH_TEST));

  caixa.load();
}

void Render(GLFWwindow* window) {
  int w, h;
  glfwGetWindowSize(window, &w, &h);

  glm::mat4 model = glm::rotate(glm::mat4(1.0f), angles.x, glm::vec3(1, 0, 0));
  model *= glm::rotate(glm::mat4(1.0f), angles.z, glm::vec3(0, 0, 1));
  model *= glm::rotate(glm::mat4(1.0f), angles.y, glm::vec3(0, 1, 0));
  glm::mat4 view = glm::lookAt(eye, center, glm::vec3(0.0, 1.0, 0.0));
  glm::mat4 projection = glm::perspective(recalculatefov((float)w, (float)h), 1.0f * (float)w / (float)h, 0.1f, 1000.0f);
  glm::mat4 mvp = projection * view * model;

  // set uniforms
  caixa.SetUniformMat4f("u_mvp", mvp);
  renderer.Draw(caixa.getVao(), caixa.getIb(), caixa.getShader());
}

void RenderInterface(GLFWwindow* window) {
  // ImGui::GetIO().IniFilename = NULL;
  ImGui::Begin("Degub");
  ImGui::Text("FPS: %3.1f", (ImGui::GetIO()).Framerate);
  ImGui::End();
}

void Shutdown(GLFWwindow* window) {
  caixa.~Model();
}

void CursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
  ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);  // imgui
  if (mouse_b1) {
    glm::dvec2 mouse_pos;
    glm::vec2 delta_pos;
    glfwGetCursorPos(window, &mouse_pos.x, &mouse_pos.y);
    delta_pos = glm::vec2(mouse_pos - mouse_anchor) * 0.01f;
    angles.x = angle_anchor.x + delta_pos.y;
    angles.y = angle_anchor.y + delta_pos.x;
  }
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
  ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);  // imgui
  switch (button) {
    case GLFW_MOUSE_BUTTON_1:
      mouse_b1 = (action == GLFW_PRESS);
      if (mouse_b1) {
        angle_anchor = angles;
        glfwGetCursorPos(window, &mouse_anchor.x, &mouse_anchor.y);
      }
      break;
  }
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
  ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);  // imgui
  eye *= 1.0f - yoffset / 10.0f;
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
  glViewport(0, 0, width, height);
}

}  // namespace App