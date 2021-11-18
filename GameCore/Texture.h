#pragma once
#include <glm/glm.hpp>
#include <gl/glew.h>
#include "Logger.h"
#include "TextureType.h"

using namespace glm;
using namespace std;

namespace GameCore {
	static u32 TextureGlobalId = 0;

	class Texture {
	private:
		Logger logger = Logger("Texture");
	public:
		// object properties
		u32 id;
		TextureType type;
		string name;

		// texture properties (openGL)
		GLuint wrapS;
		GLuint wrapT;
		GLuint wrapR;
		GLuint magFilter;
		GLuint minFilter;
		GLboolean generateMipmaps;

		// image properties
		vector<string> imagePaths;

		// textureId
		GLuint textureId;

		Texture(vector<string> imagePaths,
			GLuint wrapS = GL_REPEAT,
			GLuint wrapT = GL_REPEAT,
			GLuint wrapR = GL_REPEAT,
			GLuint magFilter = GL_LINEAR_MIPMAP_LINEAR,
			GLuint minFilter = GL_LINEAR,
			GLboolean generateMipmaps = GL_TRUE) {
			
			this->id = TextureGlobalId++;
			this->type = TextureType::Texture;
			this->name = "";

			this->wrapS = wrapS;
			this->wrapT = wrapT;
			this->wrapR = wrapR;
			this->magFilter = magFilter;
			this->minFilter = minFilter;
			this->generateMipmaps = generateMipmaps;

			this->textureId = 0;
			
			this->imagePaths = imagePaths;
			this->load();
		}

		virtual void load() = 0;
	};
}