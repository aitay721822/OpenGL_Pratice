#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <assimp/Importer.hpp>
#include <GLFW/glfw3.h>
#include <GL/glew.h>

#include "shader_loader.h"
#include "Object3D.h"
#include "ObjectType.h"
#include "Vertex.h"
#include "Material.h"

using namespace std;

namespace GameCore {
	// TODO: implement geometry class
	class Mesh : public Object3D {
	private:
		Logger logger = Logger("Mesh");
	public:
		// 3d attributes
		vector<Vertex> vertices;
		vector<GLuint> indices;
		Material material;

		string name;
		ObjectType type;

		Mesh(const aiMesh* mesh, const aiScene* scene) {
			// process vertices
			for (GLuint i = 0; i < mesh->mNumVertices; i++) {
				vec3 position, normals, tangent, bitangent;
				vector<vec2> texCoords;
				vector<vec4> colors;

				if (mesh->mVertices) {
					position = vec3(mesh->mVertices[i].x,
						mesh->mVertices[i].y,
						mesh->mVertices[i].z);
				}
				if (mesh->mNormals) {
					normals = vec3(mesh->mNormals[i].x,
						mesh->mNormals[i].y,
						mesh->mNormals[i].z);
				}
				if (mesh->mTangents) {
					tangent = vec3(mesh->mTangents[i].x,
						mesh->mTangents[i].y,
						mesh->mTangents[i].z);
				}
				if (mesh->mBitangents) {
					bitangent = vec3(mesh->mBitangents[i].x,
						mesh->mBitangents[i].y,
						mesh->mBitangents[i].z);
				}

				for (unsigned int i = 0; i < mesh->GetNumUVChannels(); i++) {
					texCoords.push_back(vec2(
						mesh->mTextureCoords[i]->x, 
						mesh->mTextureCoords[i]->y));
				}

				for (unsigned int i = 0; i < mesh->GetNumColorChannels(); i++) {
					colors.push_back(vec4(
						mesh->mColors[i]->r,
						mesh->mColors[i]->g,
						mesh->mColors[i]->b,
						mesh->mColors[i]->a));
				}
				vertices.push_back(Vertex(position, normals, texCoords, colors, tangent, bitangent));
			}
			// process indices
			for (GLuint i = 0; i < mesh->mNumFaces; i++) {
				aiFace face = mesh->mFaces[i];
				for (GLuint j = 0; j < face.mNumIndices; j++) {
					indices.push_back(face.mIndices[j]);
				}
			}
			// material
			aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
			material = Material(mat);
		}
	};
}


//class Mesh {
//public:
//	vector<Vertex> vertices;
//	vector<GLuint> indices;
//	vector<Texture> textures;
//	Material mats;
//
//	Mesh(vector<Vertex> vertices, 
//		 vector<GLuint> indices, 
//		 vector<Texture> textures,
//		 Material mat
//	) {
//		this->vertices = vertices;
//		this->indices = indices;
//		this->textures = textures;
//		this->mats = mat;
//
//		this->Init();
//	}
//
//	void Draw(Shader shader) {
//		GLuint diffuseNr = 1;
//		GLuint specularNr = 1;
//		GLuint normalNr = 1;
//		GLuint heightNr = 1;
//
//		for (GLuint i = 0; i < this->textures.size(); i++) {
//			glActiveTexture(GL_TEXTURE0 + i);
//
//			stringstream ss;
//			string number;
//			string name = this->textures[i].Type;
//
//			if (name == "texture_diffuse") {
//				ss << diffuseNr++;
//			}
//			else if (name == "texture_specular") {
//				ss << specularNr++;
//			}
//			else if (name == "texture_normal") {
//				ss << normalNr++;
//			}
//			else if (name == "texture_height") {
//				ss << heightNr++;
//			}
//
//			number = ss.str();
//			string id = "material." + name + number;
//			shader.set1i(id, i, false);
//			glBindTexture(GL_TEXTURE_2D, this->textures[i].Id);
//		}
//		// lights
//		shader.setVec3f("material.ambient", mats.Ka, false);
//		shader.setVec3f("material.diffuse", mats.Kd, false);
//		shader.setVec3f("material.specular", mats.Ks, false);
//
//		// draw mesh
//		glBindVertexArray(this->VAO);
//		glBindBufferRange(GL_UNIFORM_BUFFER, 0, uniformBlockIndex, 0, sizeof(Material));
//		glDrawElements(GL_TRIANGLES, (GLsizei)this->indices.size(), GL_UNSIGNED_INT, 0);
//		glBindVertexArray(0);
//
//		// texture unbind
//		for (GLuint i = 0; i < this->textures.size(); i++) {
//			glActiveTexture(GL_TEXTURE0 + i);
//			glBindTexture(GL_TEXTURE_2D, 0);
//		}
//		// set defaults
//		glActiveTexture(GL_TEXTURE0);
//	}
//private:
//	GLuint VAO, VBO, EBO, uniformBlockIndex;
//	void Init() {
//		glGenVertexArrays(1, &VAO);
//		glGenBuffers(1, &VBO);
//		glGenBuffers(1, &EBO);
//		glGenBuffers(1, &uniformBlockIndex);
//
//		glBindVertexArray(VAO);
//		glBindBuffer(GL_ARRAY_BUFFER, VBO);
//		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex) + sizeof(mats), &vertices[0], GL_STATIC_DRAW);
//		
//		glBindBuffer(GL_UNIFORM_BUFFER, uniformBlockIndex);
//		glBufferData(GL_UNIFORM_BUFFER, sizeof(mats), (void*)(&mats), GL_STATIC_DRAW);
//
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
//
//		// set the vertex attribute pointers
//		// vertex Positions
//		glEnableVertexAttribArray(0);
//		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
//		// vertex normals
//		glEnableVertexAttribArray(1);
//		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
//		// vertex texture coords
//		glEnableVertexAttribArray(2);
//		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
//		// vertex tangent
//		glEnableVertexAttribArray(3);
//		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
//		// vertex bitangent
//		glEnableVertexAttribArray(4);
//		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
//
//		glBindVertexArray(0);
//	}
//};