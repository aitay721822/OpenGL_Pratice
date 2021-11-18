#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Logger.h"
#include "Camera.h"

namespace GameCore {
	class OrthographicCamera : public Camera {
	public:
		float top, bottom, left, right;
		float near, far;

		OrthographicCamera(float left = -1,
			float right = 1,
			float top = 1,
			float bottom = -1,
			float near = 0.1f,
			float far = 2000.f) {
			
			this->type = ObjectType::OrthographicCamera;
			
			this->left = left;
			this->right = right;
			this->top = top;
			this->bottom = bottom;

			this->near = near;
			this->far = far;

			this->updateProjectionMatrix();
		}
	private:
		Logger logger = Logger("OrthographicCamera");

		void updateProjectionMatrix() {
			this->projectionMatrix = glm::ortho(left, right, bottom, top, near, far);
			this->projectionMatrixInverse = inverse(this->projectionMatrix);
		}
	};
}