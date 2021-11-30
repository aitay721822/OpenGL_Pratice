#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Renderer.h"
#include "Mesh.h"

using namespace std;
using namespace glm;

namespace GameCore {

	class MeshRenderer : public Renderer {
	private:
		Mesh* ref;
		GLuint VAO, VBO, EBO;
	public:
		MeshRenderer(Mesh* ref) {
			this->ref = ref;
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);
	
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, ref->vertices.size() * sizeof(Vertex), &ref->vertices[0], GL_STATIC_DRAW);
				
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, ref->indices.size() * sizeof(unsigned int), &ref->indices[0], GL_STATIC_DRAW);
	
			// set the vertex attribute pointers
			// vertex positions
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
			// vertex normals
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
			// vertex texture coords
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
			// vertex color 
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, colors));
			// vertex tangent
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
			// vertex bitangent
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
			
			glBindVertexArray(0);
		}

		~MeshRenderer() {
			this->ref = nullptr;
		}

		void render(Shader* shader) {
			ref->onBeforeRender();
			
			Material* material = ref->material;
			// render textures ( ambient, diffuse, specular, normal, height)
			u32 num = 0;
			for (u32 i = 0; i < material->ambientMaps.size(); i++, num++) {
				glActiveTexture(GL_TEXTURE0 + num);
				string id = "material.texture_ambient" + to_string(i + 1);
				shader->set1i(id, num);
				glBindTexture(GL_TEXTURE_2D, material->ambientMaps[i]->textureId);
			}
			for (u32 i = 0; i < material->diffuseMaps.size(); i++, num++) {
				glActiveTexture(GL_TEXTURE0 + num);
				string id = "material.texture_diffuse" + to_string(i + 1);
				shader->set1i(id, num);
				glBindTexture(GL_TEXTURE_2D, material->diffuseMaps[i]->textureId);
			}
			for (u32 i = 0; i < material->specularMaps.size(); i++, num++) {
				glActiveTexture(GL_TEXTURE0 + num);
				string id = "material.texture_specular" + to_string(i + 1);
				shader->set1i(id, num);
				glBindTexture(GL_TEXTURE_2D, material->specularMaps[i]->textureId);
			}

			for (u32 i = 0; i < material->normalMaps.size(); i++, num++) {
				glActiveTexture(GL_TEXTURE0 + num);
				string id = "material.texture_normal" + to_string(i + 1);
				shader->set1i(id, num);
				glBindTexture(GL_TEXTURE_2D, material->normalMaps[i]->textureId);
			}
			for (u32 i = 0; i < material->heightMaps.size(); i++, num++) {
				glActiveTexture(GL_TEXTURE0 + num);
				string id = "material.texture_height" + to_string(i + 1);
				shader->set1i(id, num);
				glBindTexture(GL_TEXTURE_2D, material->heightMaps[i]->textureId);
			}

			// render colors
			shader->setVec3f("material.ambient", material->ambientColor);
			shader->setVec3f("material.diffuse", material->diffuseColor);
			shader->setVec3f("material.specular", material->specularColor);

			// render mesh
			shader->Use();
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, (GLsizei)ref->indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
			shader->Unuse();

			// unbind
			// texture unbind
			for (; num > 0; num--) {
				glActiveTexture(GL_TEXTURE0 + num);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			// set defaults
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);

			ref->onAfterRender();
		}

		void update(Shader* shader, Camera* camera) {
			shader->setMat4fv("model", ref->getWorldMatrix());
			shader->setMat4fv("view", camera->getWorldMatrixInverse());
			shader->setMat4fv("projection", camera->getProjectionMatrix());
			shader->setVec3f("cameraPosition", camera->getPosition());
		}
	};

}