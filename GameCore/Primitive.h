#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "ObjectType.h"
#include "Material.h"
#include "Vertex.h"

#include "Object3D.h"

using namespace std;
using namespace glm;

namespace GameCore {
	// Geometric Primitive class
	class Primitive : public Object3D {
	private:
		vector<Vertex> vertices;
		vector<GLuint> indices;
		Texture* texture = nullptr;

	public:
		/* Constructor/Deconstructor */
		Primitive() {
			this->type = ObjectType::Primitive;
		}
		~Primitive() {
			delete texture;
		}

		Texture* getTexture() {
			return this->texture;
		}

		void SetTexture(Texture* tex) {
			this->texture = tex;
		}

		void Set(const vector<Vertex> v,
			const vector<GLuint> i) {
			this->vertices.assign(v.begin(), v.end());
			this->indices.assign(i.begin(), i.end());
		}

		inline vector<Vertex> getVertices() { return this->vertices; }
		inline vector<GLuint> getIndices() { return this->indices; }
		inline Vertex* getVerticesArray() { return this->vertices.data(); }
		inline GLuint* getIndicesArray() { return this->indices.data(); }
		inline size_t getVerticesSize() { return this->vertices.size(); }
		inline size_t getIndicesSize() { return this->indices.size(); }
	};
}