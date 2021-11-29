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

		// shininess of a phong-shaded material
		float shininess;

		Material(aiMaterial* mat) {
			this->id = MaterialGlobalId++;
			this->name = string(mat->GetName().C_Str());
			this->type = MaterialType::Material;

			loadMaterial(mat);
		}


		void loadMaterial(aiMaterial* mat) {
			// set colors and shininess
			aiColor3D color;
			mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
			this->ambientColor = vec3(color.r, color.g, color.b);
			mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
			this->diffuseColor = vec3(color.r, color.g, color.b);
			mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
			this->specularColor = vec3(color.r, color.g, color.b);
			float shininess;
			mat->Get(AI_MATKEY_SHININESS, shininess);
			this->shininess = shininess;

			// get textures
			loadTextures(mat, aiTextureType_AMBIENT, ambientMaps);
			loadTextures(mat, aiTextureType_DIFFUSE, diffuseMaps);
			loadTextures(mat, aiTextureType_SPECULAR, specularMaps);
			loadTextures(mat, aiTextureType_NORMALS, normalMaps);
			loadTextures(mat, aiTextureType_HEIGHT, heightMaps);
		}

		void loadTextures(aiMaterial* mat, aiTextureType type, vector<Texture*>& out) {
			vector<Texture*> loaded;
			for (GLuint i = 0; i < mat->GetTextureCount(type); i++) {
				aiString path;
				mat->GetTexture(type, i, &path);
				if (path.length > 0 && path.data[0] == '*') {
					logger.Info("Has embedded id: %s", path);
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
					Texture2D* texture = new Texture2D({ path.C_Str() });
					loaded.push_back(texture);
					out.push_back(texture);
				}
			}
		}
	};
}