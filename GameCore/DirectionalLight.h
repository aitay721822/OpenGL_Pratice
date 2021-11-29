#pragma once

#include "Light.h"

using namespace std;
using namespace glm;

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

		void setPosition(vec3 position) {
			this->position = position;
			updateMatrix();
		}
		
		void setDirection(vec3 direction) {
			this->direction = normalize(direction);
		}
	};
}
