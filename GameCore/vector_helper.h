#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

using namespace glm;

namespace GameCore {
	namespace Math {
		static vec3 applyMatrix4(vec3 const& v, mat4 const& m) {
			float w = 1 / (m[0][3] * v.x + m[1][3] * v.y + m[2][3] * v.z + m[3][3]);

			vec3 res(1.0f);
			res.x = (m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z + m[3][0]) * w;
			res.y = (m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z + m[3][1]) * w;
			res.z = (m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z + m[3][2]) * w;

			return res;
		}

		static vec3 applyQuaternion(vec3 const& v, quat const& q) {
			quat result = q * v;
			quat length = result * inverse(q);
			return vec3(length.x, length.y, length.z);
		}

		static vec3 matrixPosition(mat4 const& m) {
			return vec3(m[3][0], m[3][1], m[3][2]);
		}

		static mat4 extractRotation(mat4 const& m) {
			float sx = 1 / length(vec3(m[0])),
				sy = 1 / length(vec3(m[1])),
				sz = 1 / length(vec3(m[2]));

			mat4 result;
			result[0][0] = m[0][0] * sx;
			result[0][1] = m[0][1] * sx;
			result[0][2] = m[0][2] * sx;
			result[0][3] = 0;

			result[1][0] = m[1][0] * sy;
			result[1][1] = m[1][1] * sy;
			result[1][2] = m[1][2] * sy;
			result[1][3] = 0;

			result[2][0] = m[2][0] * sz;
			result[2][1] = m[2][1] * sz;
			result[2][2] = m[2][2] * sz;
			result[2][3] = 0;

			result[3][0] = 0;
			result[3][1] = 0;
			result[3][2] = 0;
			result[3][3] = 1;

			return result;
		}

	}
}