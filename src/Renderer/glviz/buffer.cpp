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

#include "buffer.hpp"

#include <cassert>

namespace GLviz {

glUniformBuffer::glUniformBuffer() {
  GLCall(glGenBuffers(1, &m_uniform_buffer_obj));
}

glUniformBuffer::glUniformBuffer(GLsizeiptr size) : glUniformBuffer() {
  GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_uniform_buffer_obj));
  GLCall(glBufferData(GL_UNIFORM_BUFFER, size, reinterpret_cast<GLfloat*>(0), GL_DYNAMIC_DRAW));
  GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

glUniformBuffer::~glUniformBuffer() {
  GLCall(glDeleteBuffers(1, &m_uniform_buffer_obj));
}

void glUniformBuffer::bind_buffer_base(GLuint index) {
  GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, index, m_uniform_buffer_obj));
}

void glUniformBuffer::bind() {
  GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_uniform_buffer_obj));
}

void glUniformBuffer::unbind() {
  GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

glVertexArray::glVertexArray() {
  GLCall(glGenVertexArrays(1, &m_vertex_array_obj));
}

glVertexArray::~glVertexArray() {
  GLCall(glDeleteVertexArrays(1, &m_vertex_array_obj));
}

void glVertexArray::bind() {
  GLCall(glBindVertexArray(m_vertex_array_obj));
}

void glVertexArray::unbind() {
  GLCall(glBindVertexArray(0));
}

glArrayBuffer::glArrayBuffer() {
  GLCall(glGenBuffers(1, &m_array_buffer_obj));
}

glArrayBuffer::~glArrayBuffer() {
  GLCall(glDeleteBuffers(1, &m_array_buffer_obj));
}

void glArrayBuffer::bind() {
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_array_buffer_obj));
}

void glArrayBuffer::unbind() {
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void glArrayBuffer::set_buffer_data(GLsizeiptr size, GLvoid const* ptr) {
  bind();

  GLCall(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STREAM_DRAW));
  GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, ptr));

  unbind();
}

glElementArrayBuffer::glElementArrayBuffer() {
  GLCall(glGenBuffers(1, &m_element_array_buffer_obj));
}

glElementArrayBuffer::~glElementArrayBuffer() {
  GLCall(glDeleteBuffers(1, &m_element_array_buffer_obj));
}

void glElementArrayBuffer::bind() {
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_element_array_buffer_obj));
}

void glElementArrayBuffer::unbind() {
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void glElementArrayBuffer::set_buffer_data(GLsizeiptr size, GLvoid const* ptr) {
  bind();

  GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_STREAM_DRAW));
  GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, ptr));

  unbind();
}

}  // namespace GLviz
