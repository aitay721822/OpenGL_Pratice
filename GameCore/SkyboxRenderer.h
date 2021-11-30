#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Renderer.h"
#include "Skybox.h"

using namespace std;
using namespace glm;

namespace GameCore {
	
	class SkyboxRenderer : public Renderer {
	private:
		Skybox* skyboxRef;
		GLuint VAO, VBO, EBO;
	public:
		SkyboxRenderer(Skybox* source) {
			if (source == nullptr) {
				return;
			}
			this->skyboxRef = source;

			vector<Vertex> vertices = skyboxRef->getVertices();
			int numOfVertices = (int)skyboxRef->getVerticesSize();
			vector<GLuint> indices = skyboxRef->getIndices();
			int numOfIndices = (int)skyboxRef->getIndicesSize();

			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);
	
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, numOfVertices * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, numOfIndices * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
	
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

		~SkyboxRenderer() {
			this->skyboxRef = nullptr;
		}

		void render(Shader* shader) {
			skyboxRef->onBeforeRender();

			glDepthFunc(GL_LEQUAL);
			
			shader->Use();
			Texture* texture = this->skyboxRef->getTexture();
			if (texture != nullptr) 
				glBindTexture(GL_TEXTURE_CUBE_MAP, texture->textureId);
			
			glBindVertexArray(VAO);
			glActiveTexture(GL_TEXTURE0);
			glDrawElements(GL_TRIANGLES, (GLsizei)skyboxRef->getIndicesSize(), GL_UNSIGNED_INT, 0);		
			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			shader->Unuse();

			glDepthFunc(GL_LESS);

			skyboxRef->onAfterRender();
		}

		void update(Shader* shader, Camera* camera) {
			// remove translation
			mat4 view = mat4(mat3(camera->getWorldMatrixInverse()));
			
			shader->setMat4fv("view", view);
			shader->setMat4fv("projection", camera->getProjectionMatrix());
		}
	};


}