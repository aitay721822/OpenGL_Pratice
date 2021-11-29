#pragma once

#include "Camera.h"
#include "shader_loader.h"

using namespace std;

namespace GameCore {

	// All object renderers need to implement this base class
	class Renderer {
	public:
		virtual void render(Shader* shader) = 0;
		virtual void update(Shader* shader, Camera* camera) = 0;
	};

}