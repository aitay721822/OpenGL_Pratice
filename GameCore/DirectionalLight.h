#pragma once

#include <glm/glm.hpp>
#include "Object3D.h"
#include "ObjectType.h"
#include "shader_loader.h"
#include "Light.h"

namespace GameCore {
	class DirectionalLight : public Light {
	public:
		vec3 direction;

		DirectionalLight(vec3 ambientColor, vec3 diffuseColor, vec3 specularColor, float intensity = 1.f) : Light(ambientColor, diffuseColor, specularColor, intensity) {
			this->type = ObjectType::DirectionalLight;

			this->position = vec3(DefaultUp);
			this->updateMatrix();

			this->direction = vec3(0.f, 1.f, 0.f);
		}

		DirectionalLight(vec3 color = vec3(1.0f), float intensity = 1.f) : DirectionalLight(color, color, color, intensity) {}

		void setDirection(vec3 target) {
			vec3 dir = position - target;
			this->direction = normalize(dir);
		}
	};
}
