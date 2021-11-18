#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <SOIL2/SOIL2.h>
// assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// glm
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// glew
#include <GL/glew.h>

// glfw
#include <GLFW/glfw3.h>
#include "Mesh.h"
#include "Logger.h"

#include <filesystem>
#include "Light.h"
#include "AmbientLight.h"
#include "DirectionalLight.h"

using namespace std;

namespace GameCore {
	class Model {
	public:
		Model(const GLchar* path) : Model(string(path)) {}

		Model(string path) {
			this->loadModel(path);
		}
	private:
		Logger logger = Logger("3D Model");
		vector<Mesh> meshes;
		vector<Light> lights;

		void loadModel(string path) {
			Assimp::Importer imp;
			const aiScene* scene = imp.ReadFile(path,
				aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

			if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
				logger.Err("Unable to load Model Error: %s", imp.GetErrorString());
				return;
			}
			// process node's
			// node is a small entities in the scene that have a place and orientation relative to thier parents.
			// and node can have multiple mesh 
			this->processNode(scene->mRootNode, scene);
			this->processLight(scene);
		}

		void processNode(aiNode* node, const aiScene* scene) {
			for (unsigned int i = 0; i < node->mNumMeshes; i++) {
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				meshes.push_back(Mesh(mesh, scene));
			}
			
			for (unsigned int i = 0; i < node->mNumChildren; i++) {
				this->processNode(node->mChildren[i], scene);
			}
		}
		
		void processLight(const aiScene* scene) {
			if (scene->HasLights()) {
				for (GLuint i = 0; i < scene->mNumLights; i++) {
					aiLight* source = scene->mLights[i];
					if (source->mType == aiLightSource_AMBIENT) {
						AmbientLight temp = AmbientLight();
						temp.ambientColor = vec3(source->mColorAmbient.r, source->mColorAmbient.g, source->mColorAmbient.b);
						temp.specularColor = vec3(source->mColorSpecular.r, source->mColorSpecular.g, source->mColorSpecular.b);
						temp.diffuseColor = vec3(source->mColorDiffuse.r, source->mColorDiffuse.g, source->mColorDiffuse.b);
						lights.push_back(temp);
					} else if (source->mType == aiLightSource_DIRECTIONAL) {
						DirectionalLight temp = DirectionalLight();
						temp.ambientColor = vec3(source->mColorAmbient.r, source->mColorAmbient.g, source->mColorAmbient.b);
						temp.specularColor = vec3(source->mColorSpecular.r, source->mColorSpecular.g, source->mColorSpecular.b);
						temp.diffuseColor = vec3(source->mColorDiffuse.r, source->mColorDiffuse.g, source->mColorDiffuse.b);
						temp.position = vec3(source->mPosition.x, source->mPosition.y, source->mPosition.z);
						temp.direction = vec3(source->mDirection.x, source->mDirection.y, source->mDirection.z);
						temp.updateMatrix();

					} else if (source->mType == aiLightSource_POINT) {
					
					} else if (source->mType == aiLightSource_SPOT) {

					}
				}
			}
		}
	};
}


//class Model {
//public:
//	Model(const GLchar* path) {
//		this->loadModel(path);
//	}
//
//	void Draw(Shader shader) {
//		shader.Use();
//		for (GLuint i = 0; i < this->meshes.size(); i++) {
//			this->meshes[i].Draw(shader);
//		}
//		shader.Unuse();
//	}
//private:
//	Logger* logger = new Logger("Model");
//	vector<Mesh> meshes;
//	string directory;
//	vector<Texture> texture_loaded;
//
//	vector<DirectionLight> dirlights;
//	vector<PointLight> pointlights;
//	vector<SpotLight> spotlights;
//
//	void loadModel(string path) {
//		Assimp::Importer imp;
//		const aiScene* scene = imp.ReadFile(path, 
//			aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
//
//		if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
//			logger->Err("Assimp Importer Error: %s", imp.GetErrorString());
//			return;
//		}
//		
//		this->directory = path.substr(0, path.find_last_of('/'));
//
//		this->processNode(scene->mRootNode, scene);
//		this->processLight(scene);
//	}
//
//	void processLight(const aiScene* scene) {
//		if (scene->HasLights()) {
//			for (GLuint i = 0; i < scene->mNumLights; i++) {
//				aiLight* source = scene->mLights[i];
//				if (source->mType == aiLightSourceType::aiLightSource_DIRECTIONAL) {
//					glm::vec3 direction(source->mDirection.x, source->mDirection.y, source->mDirection.z);
//					float intensity = source->mAttenuationConstant;
//					
//				}
//				
//
//			}
//		}
//	}
//
//	void processNode(aiNode* node, const aiScene* scene) {
//		for (GLuint i = 0; i < node->mNumMeshes; i++) {
//			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
//			this->meshes.push_back(this->processMesh(mesh, scene));
//		}
//
//		for (GLuint i = 0; i < node->mNumChildren; i++) {
//			this->processNode(node->mChildren[i], scene);
//		}
//	}
//
//	Mesh processMesh(aiMesh* mesh, const aiScene* scene) {
//		vector<Vertex> vertices;
//		vector<GLuint> indices;
//		vector<Texture> textures;
//
//		//vertices
//		for (GLuint i = 0; i < mesh->mNumVertices; i++) {
//			Vertex vtx;
//			vtx.Position = glm::vec3(
//				mesh->mVertices[i].x,
//				mesh->mVertices[i].y,
//				mesh->mVertices[i].z
//			);
//			if (mesh->mNormals) {
//				vtx.Normal = glm::vec3(
//					mesh->mNormals[i].x,
//					mesh->mNormals[i].y,
//					mesh->mNormals[i].z
//				);
//			}
//			if (mesh->mTextureCoords[0]) {
//				// texture coordinates
//				vtx.TexCoords = glm::vec2(
//					mesh->mTextureCoords[0][i].x,
//					mesh->mTextureCoords[0][i].y
//				);
//				// tangent
//				if (mesh->mTangents) {
//					vtx.Tangent = glm::vec3(
//						mesh->mTangents[i].x,
//						mesh->mTangents[i].y,
//						mesh->mTangents[i].z
//					);
//				}
//				// Bitangent
//				if (mesh->mBitangents) {
//					vtx.Bitangent = glm::vec3(
//						mesh->mBitangents[i].x,
//						mesh->mBitangents[i].y,
//						mesh->mBitangents[i].z
//					);
//				}
//			}
//			else {
//				vtx.TexCoords = glm::vec2(0.0f, 0.0f);
//			}
//			vertices.push_back(vtx);
//		}
//		// indices
//		for (GLuint i = 0; i < mesh->mNumFaces; i++) {
//			aiFace face = mesh->mFaces[i];
//			for (GLuint j = 0; j < face.mNumIndices; j++) {
//				indices.push_back(face.mIndices[j]);
//			}
//		}
//		// material
//		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
//		Material mat;
//		aiColor3D color;
//
//		// load mtl file 
//		material->Get(AI_MATKEY_COLOR_AMBIENT, color);
//		mat.Ka = glm::vec3(color.r, color.g, color.b);
//		material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
//		mat.Kd = glm::vec3(color.r, color.g, color.b);
//		material->Get(AI_MATKEY_COLOR_SPECULAR, color);
//		mat.Ks = glm::vec3(color.r, color.g, color.b);
//		float shininess;
//		material->Get(AI_MATKEY_SHININESS, shininess);
//		mat.Intensity = shininess;
//
//		vector<Texture> diffuseMaps = this->loadMaterialTextures(
//			material,
//			aiTextureType_DIFFUSE,
//			"texture_diffuse"
//		);
//		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
//
//		vector<Texture> specularMaps = this->loadMaterialTextures(
//			material,
//			aiTextureType_SPECULAR,
//			"texture_specular"
//		);
//		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
//
//		// because assimp aiTextureType_NORMAL doesn't load its normal maps but aiTextureType_HEIGHT does...
//		vector<Texture> normalMaps = this->loadMaterialTextures(
//			material,
//			aiTextureType_HEIGHT,
//			"texture_normal"
//		);
//		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
//
//		vector<Texture> heightMaps = this->loadMaterialTextures(
//			material,
//			aiTextureType_AMBIENT,
//			"texture_height"
//		);
//		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
//
//		return Mesh(vertices, indices, textures, mat);
//	}
//
//	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName){
//		vector<Texture> textures;
//
//		for (GLuint i = 0; i < mat->GetTextureCount(type); i++) {
//			aiString str;
//			mat->GetTexture(type, i, &str);
//
//			GLboolean skip = false;
//			for (GLuint j = 0; j < texture_loaded.size(); j++) {
//				if (strcmp(str.C_Str(), texture_loaded[j].Path.C_Str()) == 0) {
//					textures.push_back(texture_loaded[j]);
//					skip = true;
//					break;
//				}
//			}
//			
//			if (!skip) {
//				Texture texture;
//				texture.Id = TextureFromFile(str.C_Str(), this->directory);
//				texture.Type = typeName;
//				texture.Path = str;
//				textures.push_back(texture);
//
//				this->texture_loaded.push_back(texture);
//			}
//
//		}
//
//		return textures;
//	}
//};