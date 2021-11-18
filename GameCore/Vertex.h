#pragma once

#include <glm/glm.hpp>

using namespace glm;

namespace GameCore {
	struct Vertex {
		vec3 position;
		vec3 normal;
		vector<vec2> texCoords;
		vector<vec4> colors;
		vec3 tangent;
		vec3 bitangent;

		Vertex(
			vec3 const& position,
			vec3 const& normal,
			vector<vec2> const& texCoords,
			vector<vec4> const& colors,
			vec3 const& tangent,
			vec3 const& bitangent) {
		
			this->position = vec3(position);
			this->normal = vec3(normal);
			this->texCoords = vector<vec2>(texCoords.begin(), texCoords.end());
			this->colors = vector<vec4>(colors.begin(), colors.end());
			this->tangent = vec3(tangent);
			this->bitangent = vec3(bitangent);
		}
	};
}