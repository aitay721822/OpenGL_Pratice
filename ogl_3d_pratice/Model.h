#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
// use stb instead
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
//#include <SOIL2/SOIL2.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Constants.h"
#include "Logger.h"

using namespace std;

GLuint TextureFromFile(const char* path, string directory);

class Model {
public:
	Model(const GLchar* path) {
		this->loadModel(path);
	}

	void Draw(Shader shader) {
		for (GLuint i = 0; i < this->meshes.size(); i++) {
			this->meshes[i].Draw(shader);
		}
	}

	//std::vector<glm::vec3> get_vertices();
	//std::vector<unsigned int> get_indices();
private:
	Logger* logger = new Logger("Model", config_debug);
	vector<Mesh> meshes;
	string directory;
	vector<Texture> texture_loaded;

	void loadModel(string path) {
		Assimp::Importer imp;
		const aiScene* scene = imp.ReadFile(path, 
			aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			logger->Err("Assimp Importer Error: %s", imp.GetErrorString());
			return;
		}
		
		this->directory = path.substr(0, path.find_last_of('/'));

		this->processNode(scene->mRootNode, scene);
	}

	void processNode(aiNode* node, const aiScene* scene) {
		for (GLuint i = 0; i < node->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			this->meshes.push_back(this->processMesh(mesh, scene));
		}

		for (GLuint i = 0; i < node->mNumChildren; i++) {
			this->processNode(node->mChildren[i], scene);
		}
	}

	Mesh processMesh(aiMesh* mesh, const aiScene* scene) {
		vector<Vertex> vertices;
		vector<GLuint> indices;
		vector<Texture> textures;
		//vertices
		for (GLuint i = 0; i < mesh->mNumVertices; i++) {
			Vertex vtx;
			vtx.Position = glm::vec3(
				mesh->mVertices[i].x,
				mesh->mVertices[i].y,
				mesh->mVertices[i].z
			);
			if (mesh->mNormals) {
				vtx.Normal = glm::vec3(
					mesh->mNormals[i].x,
					mesh->mNormals[i].y,
					mesh->mNormals[i].z
				);
			}
			if (mesh->mTextureCoords[0]) {
				vtx.TexCoords = glm::vec2(
					mesh->mTextureCoords[0][i].x,
					mesh->mTextureCoords[0][i].y
				);
			}
			else {
				vtx.TexCoords = glm::vec2(0.0f, 0.0f);
			}

			// tangent
			if (mesh->mTangents) {
				vtx.Tangent = glm::vec3(
					mesh->mTangents[i].x,
					mesh->mTangents[i].y,
					mesh->mTangents[i].z
				);
			}

			// Bitangent
			if (mesh->mBitangents) {
				vtx.Bitangent = glm::vec3(
					mesh->mBitangents[i].x,
					mesh->mBitangents[i].y, 
					mesh->mBitangents[i].z
				);
			}
			vertices.push_back(vtx);
		}
		// indices
		for (GLuint i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			for (GLuint j = 0; j < face.mNumIndices; j++) {
				indices.push_back(face.mIndices[j]);
			}
		}
		// material
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		Material mat;
		aiColor3D color;

		// load mtl file 
		material->Get(AI_MATKEY_COLOR_AMBIENT, color);
		mat.Ka = glm::vec4(color.r, color.g, color.b, 1.0);
		material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		mat.Kd = glm::vec4(color.r, color.g, color.b, 1.0);
		material->Get(AI_MATKEY_COLOR_SPECULAR, color);
		mat.Ks = glm::vec4(color.r, color.g, color.b, 1.0);

		vector<Texture> diffuseMaps = this->loadMaterialTextures(
			material,
			aiTextureType_DIFFUSE,
			"texture_diffuse"
		);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		vector<Texture> specularMaps = this->loadMaterialTextures(
			material,
			aiTextureType_SPECULAR,
			"texture_specular"
		);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		// because assimp aiTextureType_NORMAL doesn't load its normal maps while aiTextureType_HEIGHT does...
		vector<Texture> normalMaps = this->loadMaterialTextures(
			material,
			aiTextureType_HEIGHT,
			"texture_normal"
		);
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

		vector<Texture> heightMaps = this->loadMaterialTextures(
			material,
			aiTextureType_AMBIENT,
			"texture_height"
		);
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

		return Mesh(vertices, indices, textures, mat);
	}

	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName){
		vector<Texture> textures;

		for (GLuint i = 0; i < mat->GetTextureCount(type); i++) {
			aiString str;
			mat->GetTexture(type, i, &str);

			GLboolean skip = false;
			for (GLuint j = 0; j < texture_loaded.size(); j++) {
				if (strcmp(str.C_Str(), texture_loaded[j].Path.C_Str()) == 0) {
					textures.push_back(texture_loaded[j]);
					skip = true;
					break;
				}
			}
			
			if (!skip) {
				Texture texture;
				texture.Id = TextureFromFile(str.C_Str(), this->directory);
				texture.Type = typeName;
				texture.Path = str;
				textures.push_back(texture);

				this->texture_loaded.push_back(texture);
			}

		}

		return textures;
	}
};

GLuint TextureFromFile(const char* path, string directory) {
	string fullname = string(path);
	fullname = directory + '/' + fullname;
	// gen texture ID
	GLuint textureId;
	glGenTextures(1, &textureId);

	int w, h, nrComponents;
	unsigned char* image = stbi_load(
		fullname.c_str(),
		&w, &h, &nrComponents, 0
	);
	if (image) {
		GLenum format = GL_RGB;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0,
			format, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	stbi_image_free(image);
	return textureId;
}




#endif 
