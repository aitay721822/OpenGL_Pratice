#pragma once

#include <glm/glm.hpp>
#include "Object3D.h"
#include "ObjectType.h"
#include "shader_loader.h"
#include "Light.h"

namespace GameCore {
	class AmbientLight : public Light {
	public:
		AmbientLight(vec3 ambientColor, vec3 diffuseColor, vec3 specularColor, float intensity = 1.f) : Light(ambientColor, diffuseColor, specularColor, intensity) {
			this->type = ObjectType::AmbientLight;
		}

		AmbientLight(vec3 color = vec3(1.0f), float intensity = 1.f) : AmbientLight(color, color, color, intensity) {}
	};
}
