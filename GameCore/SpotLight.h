#pragma once

#include "PointLight.h"

using namespace std;
using namespace glm;

// »E¥ú·½
namespace GameCore {
	class SpotLight : public PointLight {
	public:
		glm::vec3 direction;
		float cutoff;

		SpotLight(
			vec3 ambientColor,
			vec3 diffuseColor,
			vec3 specularColor,
			float intensity = 1.f,
			float constant = 1.f,
			float linear = 0.09f,
			float quadratic = 0.032f,
			float cutoff = 0.5f) : PointLight(ambientColor, diffuseColor, specularColor, intensity, constant, linear, quadratic) {

			this->type = ObjectType::SpotLight;

			this->position = vec3(DefaultUp);
			this->updateMatrix();

			this->direction = vec3(0.f, 1.f, 0.f);
			this->cutoff = cutoff;
		}

		SpotLight(
			glm::vec3 color = vec3(1.f),
			float intensity = 1.f,
			float constant = 1.f,
			float linear = 0.09f,
			float quadratic = 0.032f,
			float cutoff = 0.5f
		) : SpotLight(color, color, color, intensity, constant, linear, quadratic) {}


		~SpotLight() {}

		void setDirection(vec3 direction) {
			this->direction = normalize(direction);
		}
	};
}