#pragma once

#include "Mesh.h"

using namespace std;

// Geometric Primitive class
class Primitive {
private:
	vector<Vertex> vertices;
	vector<GLuint> indices;

public:
	/* Constructor/Deconstructor */
	Primitive() {}
	~Primitive() {}
	

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









