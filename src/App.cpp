#include "App.h"

#include <iostream>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Renderer/GLDebug.h"

#include <memory>

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#ifndef M_PI
#define M_PI ((float)3.14159265358979323846)
#endif

namespace App {

bool mouse_b1 = false;
glm::dvec2 mouse_anchor;

void Setup(GLFWwindow* window) {
  // blend
  GLCall(glEnable(GL_BLEND));
  GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
  GLCall(glEnable(GL_DEPTH_TEST));
}

void Render(GLFWwindow* window) {
  int width, height;
  glfwGetWindowSize(window, &width, &height);
}

void Shutdown(GLFWwindow* window) {}

void RenderInterface(GLFWwindow* window) {
  ImGui::Begin("Degub");
  ImGui::Text("FPS: %3.1f", (ImGui::GetIO()).Framerate);
  ImGui::End();
}

void CursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
  ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);  // imgui
  if (mouse_b1) {
    glm::dvec2 mouse_pos;
    glm::vec2 delta_pos;
    glfwGetCursorPos(window, &mouse_pos.x, &mouse_pos.y);
    delta_pos = glm::vec2(mouse_pos - mouse_anchor);
    std::cout << "mouse: (" << delta_pos.x << "," << delta_pos.y << ")" << std::endl;
  }
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
  ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);  // imgui
  switch (button) {
    case GLFW_MOUSE_BUTTON_1:
      mouse_b1 = (action == GLFW_PRESS);
      if (mouse_b1) {
        glfwGetCursorPos(window, &mouse_anchor.x, &mouse_anchor.y);
      }
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
  glViewport(0, 0, width, height);
}

}  // namespace App