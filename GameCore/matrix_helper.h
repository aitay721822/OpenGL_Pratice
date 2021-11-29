#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

using namespace std;
using namespace glm;

namespace GameCore {
	namespace Math {
		void decompose(mat4& matrix,
			vec3& position,
			quat& rotation,
			vec3& scale) {
			float sx = length(vec3(matrix[0])),
				sy = length(vec3(matrix[1])),
				sz = length(vec3(matrix[2]));

			// if determine is negative, we need to invert one scale
			float det = determinant(matrix);
			if (det < 0) { sx = -sx; }

			// position part
			position.x = matrix[3][0];
			position.y = matrix[3][1];
			position.z = matrix[3][2];

			// rotation part
			mat4 copyMat = mat4(matrix);
			copyMat = glm::scale(copyMat, vec3(1 / sx, 1 / sy, 1 / sz));
			rotation = toQuat(copyMat);

			// scale part
			scale.x = sx;
			scale.y = sy;
			scale.z = sz;
		}

		void compose(mat4& matrix,
			vec3& position,
			quat& rotation,
			vec3& scale) {
			// glm multiplication is applied backwards way
			mat4 scaleMat4 = glm::scale(mat4(1.0f), scale),
				rotationMat4 = toMat4(rotation),
				translateMat4 = glm::translate(mat4(1.0f), position);
			// scale first, then rotate, finally translate
			matrix = translateMat4 * rotationMat4 * scaleMat4;
		}
	}
}