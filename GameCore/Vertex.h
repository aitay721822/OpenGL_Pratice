#pragma once

#include <glm/glm.hpp>

using namespace glm;

namespace GameCore {
	struct Vertex {
		vec3 position;
		vec3 normal;
		vec2 texCoords;
		vec4 colors;
		vec3 tangent;
		vec3 bitangent;

		Vertex(
			vec3 const& position,
			vec3 const& normal,
			vec2 const& texCoords,
		    vec4 const& colors,
			vec3 const& tangent,
			vec3 const& bitangent) {
		
			this->position = vec3(position);
			this->normal = vec3(normal);
			this->texCoords = vec2(texCoords);
			this->colors = vec4(colors);
			this->tangent = vec3(tangent);
			this->bitangent = vec3(bitangent);
		}
	};
}