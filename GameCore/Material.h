#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <assimp/Importer.hpp>
#include <assimp/material.h>

#include <GL/glew.h>
#include <gl/GL.h>

#include "Logger.h"
#include "MaterialType.h"
#include "Texture.h"
#include "Texture2D.h"

using namespace std;
using namespace glm;

namespace GameCore {

	static u32 MaterialGlobalId = 0;

	// https://assimp-docs.readthedocs.io/en/latest/usage/use_the_lib.html?#material-system
	//struct TextureStack {
	//	vec3 constant;
	//	
	//};

	class Material {
	private:
		Logger logger = Logger("Material");
	public:
		// object properties
		u32 id;
		string name;
		MaterialType type;

		// ambient
		vec3 ambientColor;
		vector<Texture*> ambientMaps;

		// diffuse
		vec3 diffuseColor;
		vector<Texture*> diffuseMaps;

		// specular
		vec3 specularColor;
		vector<Texture*> specularMaps;

		// normal map
		vector<Texture*> normalMaps;
		vector<Texture*> heightMaps;

		Material(aiMaterial* mat, string directory) {
			this->id = MaterialGlobalId++;
			this->name = string(mat->GetName().C_Str());
			this->type = MaterialType::Material;

			loadMaterial(mat, directory);
		}

		~Material() {
			for (auto& map : ambientMaps) {
				delete map;
			}
			for (auto& map : diffuseMaps) {
				delete map;
			}
			for (auto& map : specularMaps) {
				delete map;
			}
			for (auto& map : normalMaps) {
				delete map;
			}
			for (auto& map : heightMaps) {
				delete map;
			}
		}


		void loadMaterial(aiMaterial* mat, string directory) {
			// set colors and shininess
			aiColor3D color;
			mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
			this->ambientColor = vec3(color.r, color.g, color.b);
			mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
			this->diffuseColor = vec3(color.r, color.g, color.b);
			mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
			this->specularColor = vec3(color.r, color.g, color.b);

			// get textures
			loadTextures(directory, mat, aiTextureType_AMBIENT, ambientMaps);
			loadTextures(directory, mat, aiTextureType_DIFFUSE, diffuseMaps);
			loadTextures(directory, mat, aiTextureType_SPECULAR, specularMaps);
			loadTextures(directory, mat, aiTextureType_NORMALS, normalMaps);
			loadTextures(directory, mat, aiTextureType_HEIGHT, heightMaps);
		}

		void loadTextures(string directory, aiMaterial* mat, aiTextureType type, vector<Texture*>& out) {
			filesystem::path dir(directory);

			vector<Texture*> loaded;
			for (GLuint i = 0; i < mat->GetTextureCount(type); i++) {
				aiString path;
				mat->GetTexture(type, i, &path);
				if (path.length > 0 && path.data[0] == '*') {
					logger.Info("Material(%d).loadTextures: Has embedded id: %s", this->id, path);
				}
			
				GLboolean skip = false;
				for (GLuint j = 0; j < loaded.size(); j++) {
					// assume texture is Texture2D
					if (strcmp(path.C_Str(), loaded[j]->imagePaths[0].data()) == 0) {
						out.push_back(loaded[j]);
						skip = true;
						break;
					}
				}
						
				if (!skip) {
					filesystem::path dir = filesystem::path(directory);
					filesystem::path imgPath = filesystem::path(path.C_Str());
					imgPath = (imgPath.is_absolute()) ? imgPath : dir.append(imgPath.string());
					Texture2D* texture = new Texture2D({ imgPath.string() });
					loaded.push_back(texture);
					out.push_back(texture);
				}
			}
		}
	};
}