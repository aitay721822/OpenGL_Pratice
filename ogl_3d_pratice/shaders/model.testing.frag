#version 420

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    sampler2D texture_diffuse;
};
 
in vec2 TexCoords;

out vec4 color;

uniform Material material;

void main( )
{
    color = vec4( texture( material.texture_diffuse, TexCoords ));
}