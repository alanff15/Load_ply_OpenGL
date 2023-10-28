// static const std::string GLSL_STR = R"glsl(#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

out vec3 fcolor;

uniform mat4 u_mvp;

void main() {
  gl_Position = u_mvp * vec4(position, 1.0);
  fcolor = color;
}

#-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- - #

#shader fragment
#version 330 core

layout(location = 0) out vec4 fragColor;

in vec3 fcolor;

void main() {
  fragColor = vec4(fcolor, 1.0);
}

  // )glsl";