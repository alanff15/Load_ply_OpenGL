#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

// #define GLCall(x) x

#define GLCall(x)                                                                                                 \
  {                                                                                                               \
    while (glGetError() != GL_NO_ERROR);                                                                          \
    x;                                                                                                            \
    bool stop = false;                                                                                            \
    while (GLenum error = glGetError()) {                                                                         \
      std::cerr << "[OpenGL Error] (" << error << "): " << #x << " " << __FILE__ << ":" << __LINE__ << std::endl; \
      stop = true;                                                                                                \
    }                                                                                                             \
    if (stop) __debugbreak();                                                                                     \
  }