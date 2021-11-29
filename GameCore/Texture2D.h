#pragma once

#include <filesystem>

#include <SOIL2/SOIL2.h>

#include "Texture.h"

using namespace std;
using namespace glm;

namespace GameCore {

	class Texture2D : public Texture {
	private:
		Logger logger = Logger("Texture2D");
	public:
		Texture2D(vector<string> imagePaths,
			GLuint wrapS = GL_REPEAT,
			GLuint wrapT = GL_REPEAT,
			GLuint magFilter = GL_LINEAR,
			GLuint minFilter = GL_LINEAR_MIPMAP_LINEAR,
			GLboolean generateMipmaps = GL_TRUE) : Texture(imagePaths, wrapS, wrapT, magFilter, minFilter, generateMipmaps) {
		
			this->type = TextureType::Texture2D;
			this->load();
		}

		void load() override {
			if (imagePaths.size() == 0) {
				logger.Err("image path is empty!");
				return;
			}

			// check filepath is exists, texture2D only take first path
			filesystem::path path(imagePaths[0]);
			if (!filesystem::exists(path)) {
				logger.Err("image not found: %s", path.c_str());
			}
			// get full path
			filesystem::path fp = filesystem::absolute(path);
			string fullpath = fp.string();
			// generate new texture id
			GLuint tId;
			glGenTextures(1, &tId);
			// load image using SOIL2
			int w, h, channels;
			unsigned char* image = SOIL_load_image(fullpath.c_str(), &w, &h, &channels, SOIL_LOAD_RGBA);
			if (image) {
				GLenum format = GL_RGBA;
				glBindTexture(GL_TEXTURE_2D, tId);
				glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0,
					format, GL_UNSIGNED_BYTE, image);
				
				if (generateMipmaps) glGenerateMipmap(GL_TEXTURE_2D);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrapS);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrapT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->minFilter);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->magFilter);

				glBindTexture(GL_TEXTURE_2D, 0);
			}
			SOIL_free_image_data(image);

			this->textureId = tId;
		}
	};
}