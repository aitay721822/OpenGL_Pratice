#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "ObjectType.h"
#include "Object3D.h"

using namespace std;
using namespace glm;

namespace GameCore {
	enum class CameraDirection
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	class Camera : public Object3D {
	protected:
		mat4 matrixWorldInverse;
		mat4 projectionMatrix;
		mat4 projectionMatrixInverse;
	public:

		Camera() : Object3D() {
			this->type = ObjectType::BaseCamera;
			this->matrixWorldInverse = mat4(1.f);
			this->projectionMatrix = mat4(1.f);
			this->projectionMatrixInverse = mat4(1.f);
		}

		~Camera() {}

		void ProcessKeyboard(CameraDirection direction, float deltaTime) {
			// moveSpeed * deltaTime * 3
			float velocity = 10.f * deltaTime * 3;

			glm::quat qF = this->quaternion * glm::quat(0, 0, 0, -1) * conjugate(quaternion);
			glm::vec3 Front = { qF.x, qF.y, qF.z };
			glm::vec3 Right = glm::normalize(glm::cross(Front, glm::vec3(0, 1, 0)));

			if (direction == CameraDirection::FORWARD)
				this->position += Front * velocity;

			if (direction == CameraDirection::BACKWARD)
				this->position -= Front * velocity;

			if (direction == CameraDirection::LEFT)
				this->position -= Right * velocity;

			if (direction == CameraDirection::RIGHT)
				this->position += Right * velocity;

		}

		void ProcessMouseMovement(float xoffset, float yoffset)
		{
			xoffset *= 0.2f;
			yoffset *= 0.2f;

			this->rotateX(xoffset);
			this->rotateY(yoffset);
		}

		// !------ get
		mat4 getWorldMatrixInverse() { return this->matrixWorldInverse; }
		mat4 getProjectionMatrix() { return this->projectionMatrix; }
		mat4 getProjectionMatrixInverse() { return this->projectionMatrixInverse; }
		// !------ get

		vec3 getWorldDirection() {
			return normalize(vec3(-this->matrixWorld[2][0], -this->matrixWorld[2][1], -this->matrixWorld[2][2]));
		}

		void updateWorldMatrix(mat4 transformationMatrix = mat4(1.f)) {
			Object3D::updateWorldMatrix(transformationMatrix);
			this->matrixWorldInverse = inverse(this->matrixWorld);
		}

		virtual void updateProjectionMatrix() = 0;
	};
}