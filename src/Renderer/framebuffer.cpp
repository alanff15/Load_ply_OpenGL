// This file is part of Surface Splatting.
//
// Copyright (C) 2010, 2015 by Sebastian Lipponer.
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

#include "framebuffer.hpp"

// #include <GLviz/glviz.hpp>
// #include <GLviz/utility.hpp>

#include <iostream>
#include <cstdlib>

struct Framebuffer::Impl {
  Framebuffer::Impl(QOpenGLFunctions_4_1_Core* GLCall) : GLCall(GLCall) {}
  virtual void framebuffer_texture_2d(GLenum target, GLenum attachment, GLuint texture, GLint level) = 0;
  virtual void renderbuffer_storage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height) = 0;
  virtual void allocate_depth_texture(GLuint texture, GLsizei width, GLsizei height) = 0;
  virtual void allocate_rgba_texture(GLuint texture, GLsizei width, GLsizei height) = 0;
  virtual void resize_rgba_texture(GLuint texture, GLsizei width, GLsizei height) = 0;
  virtual void resize_depth_texture(GLuint texture, GLsizei width, GLsizei height) = 0;
  virtual bool multisample() const = 0;
  QOpenGLFunctions_4_1_Core* GLCall;
};

struct Framebuffer::Default : public Framebuffer::Impl {
  Framebuffer::Default(QOpenGLFunctions_4_1_Core* GLCall) : Framebuffer::Impl(GLCall) {}
  void framebuffer_texture_2d(GLenum target, GLenum attachment, GLuint texture, GLint level) {
    GLCheck(GLCall->glFramebufferTexture2D(target, attachment, GL_TEXTURE_2D, texture, level));
  }

  void renderbuffer_storage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height) { GLCheck(GLCall->glRenderbufferStorage(target, internalformat, width, height)); }

  void allocate_depth_texture(GLuint texture, GLsizei width, GLsizei height) {
    GLCheck(GLCall->glBindTexture(GL_TEXTURE_2D, texture));
    GLCheck(GLCall->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GLCheck(GLCall->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCheck(GLCall->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE));
    GLCheck(GLCall->glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr));
    GLCheck(GLCall->glBindTexture(GL_TEXTURE_2D, 0));
  }

  void allocate_rgba_texture(GLuint texture, GLsizei width, GLsizei height) {
    GLCheck(GLCall->glBindTexture(GL_TEXTURE_2D, texture));
    GLCheck(GLCall->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GLCheck(GLCall->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCheck(GLCall->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr));
    GLCheck(GLCall->glBindTexture(GL_TEXTURE_2D, 0));
  }

  void resize_rgba_texture(GLuint texture, GLsizei width, GLsizei height) {
    GLCheck(GLCall->glBindTexture(GL_TEXTURE_2D, texture));
    GLCheck(GLCall->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr));
    GLCheck(GLCall->glBindTexture(GL_TEXTURE_2D, 0));
  }

  void resize_depth_texture(GLuint texture, GLsizei width, GLsizei height) {
    GLCheck(GLCall->glBindTexture(GL_TEXTURE_2D, texture));
    GLCheck(GLCall->glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr));
    GLCheck(GLCall->glBindTexture(GL_TEXTURE_2D, 0));
  }

  bool multisample() const { return false; }
};

struct Framebuffer::Multisample : public Framebuffer::Impl {
  Framebuffer::Multisample(QOpenGLFunctions_4_1_Core* GLCall) : Framebuffer::Impl(GLCall) {}
  void framebuffer_texture_2d(GLenum target, GLenum attachment, GLuint texture, GLint level) {
    GLCheck(GLCall->glFramebufferTexture2D(target, attachment, GL_TEXTURE_2D_MULTISAMPLE, texture, level));
  }

  void renderbuffer_storage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height) {
    GLCheck(GLCall->glRenderbufferStorageMultisample(target, 4, internalformat, width, height));
  }

  void allocate_depth_texture(GLuint texture, GLsizei width, GLsizei height) {
    GLCheck(GLCall->glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture));
    GLCheck(GLCall->glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_DEPTH_COMPONENT32F, width, height, GL_TRUE));
    GLCheck(GLCall->glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0));
  }

  void allocate_rgba_texture(GLuint texture, GLsizei width, GLsizei height) {
    GLCheck(GLCall->glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture));
    GLCheck(GLCall->glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA32F, width, height, GL_TRUE));
    GLCheck(GLCall->glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0));
  }

  void resize_rgba_texture(GLuint texture, GLsizei width, GLsizei height) {
    GLCheck(GLCall->glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture));
    GLint internal_format;
    GLCheck(GLCall->glGetTexLevelParameteriv(GL_TEXTURE_2D_MULTISAMPLE, 0, GL_TEXTURE_INTERNAL_FORMAT, &internal_format));

    GLCheck(GLCall->glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, internal_format, width, height, GL_TRUE));
    GLCheck(GLCall->glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0));
  }

  void resize_depth_texture(GLuint texture, GLsizei width, GLsizei height) { resize_rgba_texture(texture, width, height); }

  bool multisample() const { return true; }
};

Framebuffer::Framebuffer(QOpenGLFunctions_4_1_Core* GLCall) : GLCall(GLCall), m_fbo(0), m_color(0), m_normal(0), m_depth(0), m_pimpl(new Default(GLCall)) {
  // Create framebuffer object.
  GLCheck(GLCall->glGenFramebuffers(1, &m_fbo));

  // Initialize.
  bind();
  initialize();
  unbind();
}

Framebuffer::~Framebuffer() {
  bind();
  remove_and_delete_attachments();
  unbind();

  GLCheck(GLCall->glDeleteFramebuffers(1, &m_fbo));
}

GLuint Framebuffer::color_texture() {
  return m_color;
}

void Framebuffer::enable_depth_texture() {
  bind();

  GLCheck(GLCall->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0));
  GLCheck(GLCall->glDeleteRenderbuffers(1, &m_depth));

  GLint viewport[4];
  GLCheck(GLCall->glGetIntegerv(GL_VIEWPORT, viewport));

  GLCheck(GLCall->glGenTextures(1, &m_depth));
  m_pimpl->allocate_depth_texture(m_depth, viewport[2], viewport[3]);
  m_pimpl->framebuffer_texture_2d(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depth, 0);

  unbind();
}

void Framebuffer::disable_depth_texture() {
  bind();

  m_pimpl->framebuffer_texture_2d(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 0, 0);
  GLCheck(GLCall->glDeleteTextures(1, &m_depth));

  GLint viewport[4];
  GLCheck(GLCall->glGetIntegerv(GL_VIEWPORT, viewport));

  GLCheck(GLCall->glGenRenderbuffers(1, &m_depth));
  GLCheck(GLCall->glBindRenderbuffer(GL_RENDERBUFFER, m_depth));
  m_pimpl->renderbuffer_storage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32F, viewport[2], viewport[3]);
  GLCheck(GLCall->glBindRenderbuffer(GL_RENDERBUFFER, 0));

  GLCheck(GLCall->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depth));

  unbind();
}

GLuint Framebuffer::depth_texture() {
  return m_depth;
}

void Framebuffer::attach_normal_texture() {
  bind();

  GLint viewport[4];
  GLCheck(GLCall->glGetIntegerv(GL_VIEWPORT, viewport));

  GLCheck(GLCall->glGenTextures(1, &m_normal));
  m_pimpl->allocate_rgba_texture(m_normal, viewport[2], viewport[3]);
  m_pimpl->framebuffer_texture_2d(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, m_normal, 0);

  GLenum buffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
  GLCheck(GLCall->glDrawBuffers(2, buffers));

  unbind();
}

void Framebuffer::detach_normal_texture() {
  bind();

  m_pimpl->framebuffer_texture_2d(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, 0, 0);
  GLCheck(GLCall->glDeleteTextures(1, &m_normal));

  GLenum buffers[] = {GL_COLOR_ATTACHMENT0};
  GLCheck(GLCall->glDrawBuffers(1, buffers));

  unbind();
}

GLuint Framebuffer::normal_texture() {
  return m_normal;
}

void Framebuffer::set_multisample(bool enable) {
  if (m_pimpl->multisample() != enable) {
    bind();

    GLint type;
    GLCheck(GLCall->glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &type));

    remove_and_delete_attachments();

    if (m_pimpl->multisample()) {
      m_pimpl = std::unique_ptr<Framebuffer::Impl>(new Framebuffer::Default(GLCall));
    } else {
      m_pimpl = std::unique_ptr<Framebuffer::Impl>(new Framebuffer::Multisample(GLCall));
    }

    initialize();
    if (type == GL_TEXTURE) {
      attach_normal_texture();
      enable_depth_texture();
    }
    unbind();
  }
}

void Framebuffer::bind() {
  GLCheck(GLCall->glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));
}

void Framebuffer::unbind() {
  GLCheck(GLCall->glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void Framebuffer::reshape(GLint width, GLint height) {
  bind();

  GLenum attachment[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};

  for (unsigned int i(0); i < 2; ++i) {
    GLint type, name;
    GLCheck(GLCall->glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, attachment[i], GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &type));
    GLCheck(GLCall->glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, attachment[i], GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &name));

    if (type == GL_TEXTURE) {
      m_pimpl->resize_rgba_texture(name, width, height);
    }
  }

  {
    GLint type, name;
    GLCheck(GLCall->glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &type));
    GLCheck(GLCall->glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &name));

    switch (type) {
      case GL_TEXTURE:
        m_pimpl->resize_depth_texture(name, width, height);
        break;

      case GL_RENDERBUFFER:
        GLCheck(GLCall->glBindRenderbuffer(GL_RENDERBUFFER, name));
        m_pimpl->renderbuffer_storage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        GLCheck(GLCall->glBindRenderbuffer(GL_RENDERBUFFER, 0));
        break;

      case GL_NONE:
      default:
        break;
    }
  }
  unbind();
}

void Framebuffer::initialize() {
  GLint viewport[4];
  GLCheck(GLCall->glGetIntegerv(GL_VIEWPORT, viewport));

  // Attach color texture to framebuffer object.
  GLCheck(GLCall->glGenTextures(1, &m_color));
  m_pimpl->allocate_rgba_texture(m_color, viewport[2], viewport[3]);
  m_pimpl->framebuffer_texture_2d(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_color, 0);

  // Attach renderbuffer object to framebuffer object.
  GLCheck(GLCall->glGenRenderbuffers(1, &m_depth));
  GLCheck(GLCall->glBindRenderbuffer(GL_RENDERBUFFER, m_depth));
  m_pimpl->renderbuffer_storage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32F, viewport[2], viewport[3]);
  GLCheck(GLCall->glBindRenderbuffer(GL_RENDERBUFFER, 0));
  GLCheck(GLCall->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depth));

  GLenum buffers[] = {GL_COLOR_ATTACHMENT0};
  GLCheck(GLCall->glDrawBuffers(1, buffers));
}

void Framebuffer::remove_and_delete_attachments() {
  GLenum attachment[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_DEPTH_ATTACHMENT};

  for (unsigned int i(0); i < 3; ++i) {
    GLint type, name;
    GLCheck(GLCall->glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, attachment[i], GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &type));
    GLCheck(GLCall->glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, attachment[i], GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &name));

    switch (type) {
      case GL_TEXTURE:
        m_pimpl->framebuffer_texture_2d(GL_FRAMEBUFFER, attachment[i], 0, 0);
        GLCheck(GLCall->glDeleteTextures(1, reinterpret_cast<const GLuint*>(&name)));
        break;

      case GL_RENDERBUFFER:
        GLCheck(GLCall->glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment[i], GL_RENDERBUFFER, 0));
        GLCheck(GLCall->glDeleteRenderbuffers(1, reinterpret_cast<const GLuint*>(&name)));
        break;

      case GL_NONE:
      default:
        break;
    }
  }
}
