#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <assimp/postprocess.h>

#include "Light.h"
#include "DirectionalLight.h"
#include "AmbientLight.h"
#include "PointLight.h"
#include "SpotLight.h"

#include "Object3DNode.h"

using namespace std;
using namespace glm;

namespace GameCore {
	class Model {
	public:
		Model(const char* path) : Model(string(path)) {}

		Model(std::string path) {
			this->path = tryGetAbsolutePath(path);
			this->loadModel(path);
		}

		~Model() {
			delete rootNode;
			for (auto& light : lights) {
				delete light;
			}
		}

		
		Object3DNode* getNode() { return this->rootNode; }
		vector<Light*> getLights() { return this->lights; }
	private:
		Logger logger = Logger("3D Model");

		string path;
		Object3DNode* rootNode;
		vector<Light*> lights;

		string tryGetAbsolutePath(string path) {
			// get fullpath
			filesystem::path fp = filesystem::absolute(path);
			// check exists
			if (!filesystem::exists(fp)) {
				logger.Err("Model.tryGetAbsolutePath: can't resolve path because file not exists");
				return path;
			}
			return fp.string();
		}

		void loadModel(std::string path) {
			Assimp::Importer imp;
			const aiScene* scene = imp.ReadFile(path,
				aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

			if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
				logger.Err("Model.loadModel: Unable to load Model Error: %s", imp.GetErrorString());
				return ;
			}
			// process node's
			// node is a small entities in the scene that have a place and orientation relative to thier parents.
			// and node can have multiple mesh
			rootNode = this->processNode(scene->mRootNode, scene);
			this->processLight(scene);
		}

		mat4 aiMatrix4x4toMat4(aiMatrix4x4 mat) {
			mat4 res = mat4(
				mat.a1, mat.a2, mat.a3, mat.a4,
				mat.b1, mat.b2, mat.b3, mat.b4,
				mat.c1, mat.c2, mat.c3, mat.c4,
				mat.d1, mat.d2, mat.d3, mat.d4
			);
			return res;
		}

		vec3 aiColor3DtoVec3(aiColor3D color) {
			return vec3(
				color.r,
				color.g,
				color.b
			);
		}

		vec3 aiVector3toVec3(aiVector3D v) {
			return vec3(
				v.x,
				v.y,
				v.z
			);
		}

		// process recursively
		Object3DNode* processNode(aiNode* node, const aiScene* scene) {
			// build a fresh node :)
			Object3DNode* objNode = new Object3DNode(string(node->mName.C_Str()));
			objNode->setWorldMatrix(aiMatrix4x4toMat4(node->mTransformation));
			// parse current node meshes
			for (u32 i = 0; i < node->mNumMeshes; i++) {
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				objNode->addMesh(new Mesh(mesh, scene, this->path));
			}
			// process children node
			for (unsigned int i = 0; i < node->mNumChildren; i++) {
				objNode->add(this->processNode(node->mChildren[i], scene));
			}
			// return node
			return objNode;
		}
		
		void processLight(const aiScene* scene) {
			if (scene->HasLights()) {
				for (GLuint i = 0; i < scene->mNumLights; i++) {
					aiLight* source = scene->mLights[i];
					if (source->mType == aiLightSource_AMBIENT) {
						lights.push_back(new AmbientLight(
							aiColor3DtoVec3(source->mColorAmbient),
							aiColor3DtoVec3(source->mColorDiffuse),
							aiColor3DtoVec3(source->mColorSpecular)
						));
					} else if (source->mType == aiLightSource_DIRECTIONAL) {
						DirectionalLight* light = new DirectionalLight(
							aiColor3DtoVec3(source->mColorAmbient),
							aiColor3DtoVec3(source->mColorDiffuse),
							aiColor3DtoVec3(source->mColorSpecular)
						);
						light->setPosition(aiVector3toVec3(source->mPosition));
						light->setDirection(aiVector3toVec3(source->mDirection));
						lights.push_back(light);
					} else if (source->mType == aiLightSource_POINT) {
						lights.push_back( new PointLight(
							aiColor3DtoVec3(source->mColorAmbient),
							aiColor3DtoVec3(source->mColorDiffuse),
							aiColor3DtoVec3(source->mColorSpecular)
						));
					} else if (source->mType == aiLightSource_SPOT) {
						lights.push_back(new SpotLight(
							aiColor3DtoVec3(source->mColorAmbient),
							aiColor3DtoVec3(source->mColorDiffuse),
							aiColor3DtoVec3(source->mColorSpecular)
						));
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