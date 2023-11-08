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

vec3 getLight(vec3 cam, vec3 L, vec3 N, vec3 color) {
  vec3 lightColor = vec3(1);
  vec3 V = -cam;
  vec3 R = reflect(-L, N);
  float ambient = 0.20;
  float diffuse = 0.80 * clamp(dot(L, N), 0.0, 1.0);
  float specular = 0.80 * pow(clamp(dot(R, V), 0.0, 1.0), 15.0);
  float fresnel = 0.20 * pow(1.0 + dot(cam, N), 3.0);
  return (ambient + fresnel + diffuse) * mix(color, lightColor, 0.02) + specular * lightColor;
}

void main() {
  vec3 luz = normalize(vec3(0.0, 0.0, -1.0));
  vec4 cor = texture2D(u_texture, ftexCoord);
  vec3 cam = vec3(0, 0, 1);
  fragColor = vec4(getLight(cam, luz, normalize(fnormal), cor.xyz), cor.w);
}

  // )glsl";