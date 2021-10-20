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

struct SpotLight {
    PointLight baselight;

    vec3 direction;
    float cutoff;
};

in vec3 Normal;
in vec3 Position;
in vec2 TexCoords;

out vec4 color;

uniform DirectionalLight dirLight;
uniform Material material;
uniform PointLight pointlight;
uniform SpotLight spotlight;
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
vec4 calcPointLight(PointLight p, vec3 normal) {
    vec3 lightDir = Position - p.position;
    float dist = length(lightDir);
    lightDir = normalize(lightDir);

    vec4 color = calcLightInternal(p.baselight, lightDir, normal);
    float attenuation = p.constant / (1.f + p.linear * dist + p.quadratic * (dist * dist));

    return color * attenuation;
}

vec4 calcSpotLight(SpotLight s, vec3 normal) {
    vec3 lightDir = normalize(Position - s.baselight.position);
    float spotFactor = dot(lightDir, normalize(s.direction));
    if (spotFactor > s.cutoff) {
        vec4 color = calcPointLight(s.baselight, normal);
        return color * (1.0 - (1.0 - spotFactor) * 1.0 / (1.0 - s.cutoff));
    }  else {
        return vec4(0.f);
    }
}

void main(){ 
    vec3 normal = normalize(Normal);
    vec4 dirLight = calcDirectionalLight(normal);
    vec4 ptLight = calcPointLight(pointlight, normal);
    vec4 spotlight = calcSpotLight(spotlight, normal);
    //color = texture(material.texture_diffuse, TexCoords) * (spotlight);
    color = texture(material.texture_diffuse, TexCoords) * (dirLight + ptLight + spotlight);
}