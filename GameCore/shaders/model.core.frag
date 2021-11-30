#version 420

// const variable
const int MAX_AM_LIGHTS = 2;
const int MAX_DIR_LIGHTS = 2;
const int MAX_POINT_LIGHTS = 2;
const int MAX_SPOT_LIGHTS = 2;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
     
    sampler2D texture_ambient1;
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_normal1;
    sampler2D texture_height1;
};

struct AmbientLight {
    vec3 amColor;
    vec3 diColor;
    vec3 spColor;
    float intensity;
};

struct DirectionalLight {
    AmbientLight base;

    vec3 direction;
};

struct PointLight {
    AmbientLight base;

    vec3 position;
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    PointLight base;

    vec3 direction;
    float cutoff;
};

// in var
in vec3 Normal;
in vec3 Position;
in vec2 TexCoords;

// out var
out vec4 color;

// uniform (readonly)
uniform Material material;
uniform AmbientLight amLight[MAX_AM_LIGHTS];
uniform DirectionalLight dirLight[MAX_DIR_LIGHTS];
uniform PointLight ptLight[MAX_POINT_LIGHTS];
uniform SpotLight spLight[MAX_SPOT_LIGHTS];

uniform vec3 cameraPosition;

vec4 calcAmbientLight(AmbientLight light) {
    vec4 ambient = vec4(light.amColor, 1.0) * light.intensity;
    vec4 diffuse = vec4(light.diColor, 1.0) * light.intensity;
    vec4 specular = vec4(light.spColor, 1.0) * light.intensity;
    return ambient + diffuse + specular;
}

vec4 calcLightInternal(AmbientLight light, vec3 lightDir, vec3 normal) {
    vec4 ambientColor = vec4(light.amColor, 1.f) * light.intensity;
    float diffuseFactor = dot(normal, -lightDir);
    
    vec4 diffuseColor = vec4(0.f);
    vec4 specularColor = vec4(0.f);

    if (diffuseFactor > 0) {
        diffuseColor = vec4(light.diColor  * light.intensity * diffuseFactor, 1.f);
        
        vec3 viewDir = normalize(cameraPosition - Position);
        vec3 reflectDir = normalize(reflect(lightDir, normal));
        float specularFactor = dot(viewDir, reflectDir);
        if (specularFactor > 0) {
            specularFactor = pow(specularFactor, 35);
            specularColor = vec4(light.spColor * light.intensity * specularFactor, 1.f);
        }
    }

    return ambientColor + diffuseColor + specularColor;
}
 
vec4 calcDirectionalLight(DirectionalLight dirLight, vec3 normal) {
    return calcLightInternal(dirLight.base, normalize(dirLight.direction), normal);
}

vec4 calcPointLight(PointLight p, vec3 normal) {
    vec3 lightDir = Position - p.position;
    float dist = length(lightDir);
    lightDir = normalize(lightDir);

    vec4 color = calcLightInternal(p.base, lightDir, normal);
    float attenuation = p.constant / (1.f + p.linear * dist + p.quadratic * (dist * dist));

    return color * attenuation;
}

vec4 calcSpotLight(SpotLight s, vec3 normal) {
    vec3 lightDir = normalize(Position - s.base.position);
    float spotFactor = dot(lightDir, normalize(s.direction));
    if (spotFactor > s.cutoff) {
        vec4 color = calcPointLight(s.base, normal);
        return color * (1.0 - (1.0 - spotFactor) * 1.0 / (1.0 - s.cutoff));
    }  else {
        return vec4(0.f);
    }
}

void main(){ 
    vec3 normal = normalize(Normal);
    
    vec4 materialColor = vec4(material.ambient + material.diffuse + material.specular, 1.f);

    vec4 ambient = vec4(0.f);
    for (int i = 0; i < MAX_AM_LIGHTS; i++) {
        if (amLight[i].intensity > 0.f) {
            ambient += calcAmbientLight(amLight[i]);
        }
    }

    vec4 direction = vec4(0.f);
    for (int i = 0; i < MAX_DIR_LIGHTS; i++) {
        if (dirLight[i].base.intensity > 0.f) {
            direction += calcDirectionalLight(dirLight[i], normal);
        }
    }
    
    vec4 point = vec4(0.f);
    for (int i = 0; i < MAX_POINT_LIGHTS; i++) {
        if (ptLight[i].base.intensity > 0) {
            point += calcPointLight(ptLight[i], normal);
        }
    }

    vec4 spot = vec4(0.f);
    for (int i = 0; i < MAX_SPOT_LIGHTS; i++) {
        if (spLight[i].base.base.intensity > 0) {
            spot += calcSpotLight(spLight[i], normal);
        }
    }

    vec4 totalLight = ambient + direction + point + spot;

    color = texture(material.texture_diffuse1, TexCoords) * materialColor * totalLight;
}