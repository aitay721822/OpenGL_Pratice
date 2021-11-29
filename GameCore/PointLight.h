#pragma once

#include "Light.h"

using namespace std;
using namespace glm;

// ÂI¥ú·½
namespace GameCore {
	class PointLight : public Light {
	public:

		float constant, linear, quadratic;

		PointLight(
			vec3 ambientColor,
			vec3 diffuseColor, 
			vec3 specularColor,
			float intensity = 1.f,
			float constant = 1.f,
			float linear = 0.09f,
			float quadratic = 0.032f) : Light(ambientColor, diffuseColor, specularColor, intensity) {

			this->type = ObjectType::PointLight;
			this->constant = constant;
			this->linear = linear;
			this->quadratic = quadratic;
		}

		PointLight(vec3 color = vec3(1.0f),
			float intensity = 1.f,
			float constant = 1.f,
			float linear = 0.09f,
			float quadratic = 0.032f) : PointLight(color, color, color, intensity, constant, linear, quadratic) {}


		~PointLight() {}
	};
}