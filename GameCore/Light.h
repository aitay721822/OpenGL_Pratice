#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "ObjectType.h"
#include "Object3D.h"

using namespace std;
using namespace glm;

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
