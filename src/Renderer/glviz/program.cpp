// This file is part of GLviz.
//
// Copyright(c) 2014, 2015 Sebastian Lipponer
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

#include "program.hpp"

#include "camera.hpp"

#include <Eigen/Core>
#include <iostream>
#include <cstdlib>

using namespace Eigen;

namespace GLviz {

void UniformBufferWireframe::set_buffer_data(float const* color, int const* viewport) {
  bind();
  GLCheck(GLCall->glBufferSubData(GL_UNIFORM_BUFFER, 0, 4 * sizeof(GLfloat), color));
  GLCheck(GLCall->glBufferSubData(GL_UNIFORM_BUFFER, 4 * sizeof(GLfloat), 2 * sizeof(GLint), viewport));
  unbind();
}

UniformBufferCamera::UniformBufferCamera(QOpenGLFunctions_4_1_Core* GLCall) : glUniformBuffer(GLCall, 48 * sizeof(GLfloat)) {}

void UniformBufferCamera::set_buffer_data(Camera const& camera) {
  Matrix4f const& modelview_matrix = camera.get_modelview_matrix();
  Matrix4f modelview_matrix_it = modelview_matrix.inverse().transpose();
  Matrix4f const& projection_matrix = camera.get_projection_matrix();

  bind();
  GLCheck(GLCall->glBufferSubData(GL_UNIFORM_BUFFER, 0, 16 * sizeof(GLfloat), modelview_matrix.data()));
  GLCheck(GLCall->glBufferSubData(GL_UNIFORM_BUFFER, 16 * sizeof(GLfloat), 16 * sizeof(GLfloat), modelview_matrix_it.data()));
  GLCheck(GLCall->glBufferSubData(GL_UNIFORM_BUFFER, 32 * sizeof(GLfloat), 16 * sizeof(GLfloat), projection_matrix.data()));
  unbind();
}

UniformBufferMaterial::UniformBufferMaterial(QOpenGLFunctions_4_1_Core* GLCall) : glUniformBuffer(GLCall) {}

void UniformBufferMaterial::set_buffer_data(float const* mbuf) {
  bind();
  GLCheck(GLCall->glBufferData(GL_UNIFORM_BUFFER, 4 * sizeof(GLfloat), mbuf, GL_DYNAMIC_DRAW));
  unbind();
}

UniformBufferWireframe::UniformBufferWireframe(QOpenGLFunctions_4_1_Core* GLCall) : glUniformBuffer(GLCall, 4 * sizeof(GLfloat) + 2 * sizeof(GLint)) {}

UniformBufferSphere::UniformBufferSphere(QOpenGLFunctions_4_1_Core* GLCall) : glUniformBuffer(GLCall, 2 * sizeof(GLfloat)) {}

void UniformBufferSphere::set_buffer_data(float radius, float projection) {
  bind();
  GLCheck(GLCall->glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(GLfloat), &radius));
  GLCheck(GLCall->glBufferSubData(GL_UNIFORM_BUFFER, sizeof(GLfloat), sizeof(GLfloat), &projection));
  unbind();
}

}  // namespace GLviz
