#pragma once

#include "Camera.h"

using namespace std;
using namespace glm;

namespace GameCore {
	/* http://www.bobatkins.com/photography/technical/field_of_view.html */
	class PerspectiveCamera : public Camera
	{
	public:
		float fov;
		float aspect;
		float near;
		float far;

		PerspectiveCamera(
			float fov = 50.f,
			float aspect = 1.f,
			float near = 0.1f,
			float far = 2000.f) : Camera() {

			this->type = ObjectType::PerspectiveCamera;

			this->fov = fov;
			this->aspect = aspect;
			this->near = near;
			this->far = far;

			this->updateProjectionMatrix();
		}
	private:
		Logger logger = Logger("PerspectiveCamera");

		void updateProjectionMatrix() {
			this->projectionMatrix = glm::perspective(fov, aspect, near, far);
			this->projectionMatrixInverse = inverse(this->projectionMatrix);
		}
	};
}