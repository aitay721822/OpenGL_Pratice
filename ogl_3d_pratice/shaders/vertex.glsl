#version 420

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragmentPosition;
out vec3 outNormal;

void main() {
  gl_Position = projection * view * model * vec4(position, 1.0f);
  fragmentPosition = vec3(model * vec4(position, 1.0f));
  outNormal = mat3(transpose(inverse(model))) * normal;  
}