#pragma once

#include "Primitive.h"
#include "TextureCube.h"

using namespace std;
using namespace glm;

namespace GameCore {
	class Skybox : public Primitive {
	private:
		void loadTexture(const vector<string> imgPaths) {
			this->SetTexture(new TextureCube(imgPaths));
		}
	public:
		Skybox(const vector<string> imgPaths) : Primitive() {
			this->type = ObjectType::Skybox;

			const vector<Vertex> vertices = {
				// position, normal, texCoords, color, tangent, bitangent
				Vertex(vec3(-1.0f,  1.0f,  1.0f), vec3(0.f, 0.f,  1.f), vec2(0.f, 1.f), vec4(1.f), vec3(0.f), vec3(0.f)), // front left top
				Vertex(vec3(-1.0f, -1.0f,  1.0f), vec3(0.f, 0.f,  1.f), vec2(0.f, 0.f), vec4(1.f), vec3(0.f), vec3(0.f)), // front left down
				Vertex(vec3( 1.0f, -1.0f,  1.0f), vec3(0.f, 0.f,  1.f), vec2(1.f, 0.f),	vec4(1.f), vec3(0.f), vec3(0.f)), // front right down
				Vertex(vec3( 1.0f,  1.0f,  1.0f), vec3(0.f, 0.f,  1.f), vec2(1.f, 1.f), vec4(1.f), vec3(0.f), vec3(0.f)), // front right top

				Vertex(vec3( 1.0f,  1.0f, -1.0f), vec3(0.f, 0.f, -1.f), vec2(0.f, 1.f), vec4(1.f), vec3(0.f), vec3(0.f)), // back right top
				Vertex(vec3( 1.0f, -1.0f, -1.0f), vec3(0.f, 0.f, -1.f), vec2(0.f, 0.f), vec4(1.f), vec3(0.f), vec3(0.f)), // back right down
				Vertex(vec3(-1.0f, -1.0f, -1.0f), vec3(0.f, 0.f, -1.f), vec2(1.f, 0.f), vec4(1.f), vec3(0.f), vec3(0.f)), // back left down
				Vertex(vec3(-1.0f,  1.0f, -1.0f), vec3(0.f, 0.f, -1.f), vec2(1.f, 1.f), vec4(1.f), vec3(0.f), vec3(0.f)), // back left top
			};

			const vector<GLuint> indices = {
				0, 1, 2,
				0, 2, 3,

				7, 6, 1,
				7, 1, 0,

				4, 5, 6,
				4, 6, 7,

				3, 2, 5,
				3, 5, 4,

				0, 3, 7,
				3, 4, 7,

				1, 2, 6,
				2, 5, 6
			};

			this->Set(vertices, indices);
			this->loadTexture(imgPaths);
		}
	};
}

//class Skybox : public Primitive {
//private:
//	GLuint textureID = 0, VAO, VBO, EBO;
//
//	void Init() {
//		vector<Vertex> vertices = this->getVertices();
//		int numOfVertices = (int)this->getVerticesSize();
//
//		vector<GLuint> indices = this->getIndices();
//		int numOfIndices = (int)this->getIndicesSize();
//
//		glGenVertexArrays(1, &VAO);
//		glGenBuffers(1, &VBO);
//		glGenBuffers(1, &EBO);
//
//		glBindVertexArray(VAO);
//		glBindBuffer(GL_ARRAY_BUFFER, VBO);
//		glBufferData(GL_ARRAY_BUFFER, numOfVertices * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
//
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numOfIndices * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
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
//	}
//
//public:
//	Skybox() : Primitive() {
//		const vector<Vertex> vertices = {
//			// front left top
//			Vertex{glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec3(0.f, 0.f,  1.f), glm::vec2(0.f, 1.f),},
//			// front left down
//			Vertex{glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(0.f, 0.f,  1.f), glm::vec2(0.f, 0.f),},
//			// front right down
//			Vertex{glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec3(0.f, 0.f,  1.f), glm::vec2(1.f, 0.f),},
//			// front right top
//			Vertex{glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec3(0.f, 0.f,  1.f), glm::vec2(1.f, 1.f),},
//
//			// back right top
//			Vertex{glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec3(0.f, 0.f, -1.f), glm::vec2(0.f, 1.f),},
//			// back right down
//			Vertex{glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec3(0.f, 0.f, -1.f), glm::vec2(0.f, 0.f),},
//			// back left down
//			Vertex{glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.f, 0.f, -1.f), glm::vec2(1.f, 0.f),},
//			// back left top
//			Vertex{glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(0.f, 0.f, -1.f), glm::vec2(1.f, 1.f),},
//		};
//		
//		const vector<GLuint> indices = {
//			0, 1, 2,
//			0, 2, 3,
//			 
//			7, 6, 1,
//			7, 1, 0,
//
//			4, 5, 6,
//			4, 6, 7,
//
//			3, 2, 5,
//			3, 5, 4,
//
//			0, 3, 7,
//			3, 4, 7,
//
//			1, 2, 6,
//			2, 5, 6
//		};
//		
//		this->Set(vertices, indices);
//		this->Init();
//	}
//
//	void loadCubeMap(vector<const GLchar*> faces) {
//		GLuint textureID;
//		glGenTextures(1, &textureID);
//
//		int width, height;
//		unsigned char* image;
//
//		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
//		for (GLuint i = 0; i < faces.size(); i++)
//		{
//			image = SOIL_load_image(faces[i], &width, &height, NULL, SOIL_LOAD_RGBA);
//			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
//			SOIL_free_image_data(image);
//		}
//		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
//		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
//		this->textureID = textureID;
//	}
//
//	void Draw(Shader shader) {
//		glDepthFunc(GL_LEQUAL);
//
//		shader.Use();
//		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
//		glBindVertexArray(VAO);
//		glActiveTexture(GL_TEXTURE0);
//		glDrawElements(GL_TRIANGLES, (GLsizei)this->getIndicesSize(), GL_UNSIGNED_INT, 0);		
//		glBindVertexArray(0);
//		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
//
//		glDepthFunc(GL_LESS);
//	}
//
//
//};