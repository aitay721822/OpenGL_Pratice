#version 420
layout ( location = 0 ) in vec3 position;
layout ( location = 1 ) in vec3 normal;
layout ( location = 2 ) in vec2 texCoords;
layout ( location = 3 ) in vec4 colors;
layout ( location = 4 ) in vec3 tangent;
layout ( location = 5 ) in vec3 bitangent;

out vec3 Normal;
out vec3 Position;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main( )
{
    gl_Position = projection * view * model * vec4( position, 1.f );
    Normal = mat3(transpose(inverse(model))) * normal;
    Position = vec3(model * vec4(position, 1.0f));
    TexCoords = texCoords;
}
