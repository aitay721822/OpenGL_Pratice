#pragma once

#include <glm/glm.hpp>
#include "Object3D.h"
#include "ObjectType.h"
#include "shader_loader.h"

namespace GameCore {
	class Light : public Object3D {
	public:
		float intensity;
		vec3 ambientColor;
		vec3 diffuseColor;
		vec3 specularColor;

		Light(vec3 color, float intensity = 1.f) : Light(color, color, color, intensity) {}

		Light(vec3 ambientColor, vec3 diffuseColor, vec3 specularColor,
			float intensity) : Object3D() {
			this->intensity = intensity;
			this->ambientColor = ambientColor;
			this->diffuseColor = diffuseColor;
			this->specularColor = specularColor;
		}
	};
}
