#pragma once

#include "Logger.h"
#include "Object3D.h"

namespace GameCore {
	enum class CameraDirection
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	class Camera : public Object3D {
	public:
		mat4 matrixWorldInverse;
		mat4 projectionMatrix;
		mat4 projectionMatrixInverse;

		Camera() : Object3D() {
			this->type = ObjectType::BaseCamera;
			this->matrixWorldInverse = mat4(1.f);
			this->projectionMatrix = mat4(1.f);
			this->projectionMatrixInverse = mat4(1.f);
		}

		~Camera() {}

		vec3 getWorldDirection() {
			this->updateWorldMatrix(true, false);
			return normalize(vec3(-this->matrixWorld[2][0], -this->matrixWorld[2][1], -this->matrixWorld[2][2]));
		}

		void updateMatrixWorld(bool force) {
			Object3D::updateMatrixWorld(force);
			this->matrixWorldInverse = inverse(this->matrixWorld);
		}

		void updateWorldMatrix(bool updateParents, bool updateChildren) {
			Object3D::updateWorldMatrix(updateParents, updateChildren);
			this->matrixWorldInverse = inverse(this->matrixWorld);
		}
	};
}