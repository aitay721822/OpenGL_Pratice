#version 420

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
     
    sampler2D texture_diffuse;
    // sampler2D texture_specular;
};

// Base Light
struct BaseLight {
    vec3 color;
    float ambientIntensity;
    float diffuseIntensity;
    float specularIntensity;
};

// Directional Light
struct DirectionalLight {
    BaseLight baselight;

    vec3 direction;
};

struct PointLight {
    BaseLight baselight;

    vec3 position;
    float constant;
    float linear;
    float quadratic;
};

in vec3 Normal;
in vec3 Position;
in vec2 TexCoords;

out vec4 color;

uniform DirectionalLight dirLight;
uniform Material material;
uniform PointLight pointlight;
uniform vec3 cameraPosition;

vec4 calcLightInternal(BaseLight light, vec3 lightDir, vec3 normal) {
    vec4 ambientColor = vec4(light.color, 1.f) * light.ambientIntensity;
    float diffuseFactor = dot(normal, -lightDir);
    
    vec4 diffuseColor = vec4(0.f);
    vec4 specularColor = vec4(0.f);

    if (diffuseFactor > 0) {
        diffuseColor = vec4(light.color * light.diffuseIntensity * diffuseFactor, 1.0f);
        
        vec3 viewDir = normalize(cameraPosition - Position);
        vec3 reflectDir = normalize(reflect(lightDir, normal));
        float specularFactor = dot(viewDir, reflectDir);
        if (specularFactor > 0) {
            specularFactor = pow(specularFactor, 35);
            specularColor = vec4(light.color * light.specularIntensity * specularFactor, 1.0f);
        }
    }

    return ambientColor + diffuseColor + specularColor;
}
 
vec4 calcDirectionalLight(vec3 normal) {
    return calcLightInternal(dirLight.baselight, normalize(dirLight.direction), normal);
}

// TODO: Multiple Point Lights
vec4 calcPointLight(vec3 normal) {
    vec3 lightDir = Position - pointlight.position;
    float dist = length(lightDir);
    lightDir = normalize(lightDir);

    vec4 color = calcLightInternal(pointlight.baselight, lightDir, normal);
    float attenuation = pointlight.constant / (1.f + pointlight.linear * dist + pointlight.quadratic * (dist * dist));

    return color * attenuation;
}

void main(){
    vec3 normal = normalize(Normal);
    vec4 dirLight = calcDirectionalLight(normal);
    vec4 ptLight = calcPointLight(normal);
    color = texture(material.texture_diffuse, TexCoords) * (dirLight + ptLight);
}