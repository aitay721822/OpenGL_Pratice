#pragma once

#include <filesystem>
#include <glm/glm.hpp>
#include <gl/glew.h>
#include "Logger.h"
#include "TextureType.h"
#include "Texture.h"
#include <SOIL2/SOIL2.h>

using namespace glm;
using namespace std;

namespace GameCore {

	class TextureCube : public Texture {
	private:
		Logger logger = Logger("TextureCube");
	public:
		TextureCube(vector<string> imagePaths,
			GLuint wrapS = GL_CLAMP_TO_EDGE,
			GLuint wrapT = GL_CLAMP_TO_EDGE,
			GLuint wrapR = GL_CLAMP_TO_EDGE,
			GLuint magFilter = GL_LINEAR,
			GLuint minFilter = GL_LINEAR,
			GLboolean generateMipmaps = GL_TRUE) : Texture(imagePaths, wrapS, wrapT, wrapR, magFilter, minFilter, generateMipmaps) {

			this->type = TextureType::CubeTexture;
			this->load();
		}

		void load() override {
			if (imagePaths.size() == 0) {
				logger.Err("image path is empty!");
				return;
			}
			
			// check filepath is exists
			vector<string> fps;
			for (auto& imagePath : imagePaths) {
				filesystem::path path(imagePath);
				if (!filesystem::exists(path)) {
					logger.Err("image not found: %s", path.c_str());
				}
				filesystem::path fp = filesystem::absolute(path);
				fps.push_back(fp.string());
			}

			if (fps.size() == 0) {
				logger.Err("all cube face image path is not exists!");
				return;
			}

			// generate new texture id
			GLuint tId;
			glGenTextures(1, &tId);
			glBindTexture(GL_TEXTURE_CUBE_MAP, tId);

			// load image using SOIL2
			for (int i = 0; i < fps.size(); i++) {
				int w, h, channels;
				unsigned char* image = SOIL_load_image(fps[i].c_str(), &w, &h, &channels, SOIL_LOAD_RGBA);
				if (image) {
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
				}
				SOIL_free_image_data(image);
			}
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, this->magFilter);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, this->minFilter);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, this->wrapS);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, this->wrapT);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, this->wrapR);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			this->textureId = tId;
		}
	};
}