#ifndef _OPENGL_DEBUG_H_
#define _OPENGL_DEBUG_H_

#define GLCheck(x)                                                                                  \
  {                                                                                                 \
    while (GLCall->glGetError() != GL_NO_ERROR);                                                    \
    x;                                                                                              \
    bool stop = false;                                                                              \
    while (GLenum error = GLCall->glGetError()) {                                                   \
      qDebug() << "[OpenGL Error] (" << error << "): " << #x << " " << __FILE__ << ":" << __LINE__; \
      stop = true;                                                                                  \
    }                                                                                               \
    if (stop) __debugbreak();                                                                       \
  }

#endif  // _OPENGL_DEBUG_H_