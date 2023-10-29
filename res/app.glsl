// static const std::string GLSL_STR = R"glsl(#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec2 ftexCoord;
out vec3 fnormal;

uniform mat4 u_mvp;

void main() {
  gl_Position = u_mvp * vec4(position, 1.0);
  fnormal = mat3(u_mvp) * normal;
  ftexCoord = texCoord;
}

#-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- - #

#shader fragment
#version 330 core

layout(location = 0) out vec4 fragColor;

uniform sampler2D u_texture;

in vec2 ftexCoord;
in vec3 fnormal;

void main() {
  vec3 luz = normalize(vec3(0.0, 0.0, -1.0));
  vec4 cor = texture2D(u_texture, ftexCoord);
  float ganho = 0.5 + 0.5 * dot(luz, fnormal);
  fragColor = pow(vec4(ganho * cor.xyz, cor.w), vec4(0.9));
}

  // )glsl";