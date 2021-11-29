#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Renderer.h"
#include "AmbientLight.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

using namespace std;
using namespace glm;

namespace GameCore {

	class AmbientLightRenderer : public Renderer {
	private:
		u32 id;
		AmbientLight* ref;
	public:
		AmbientLightRenderer(AmbientLight* ref, u32 id = 0) {
			this->ref = ref;
			this->id = id;
		}
		
		void render(Shader* shader) {
			ref->onBeforeRender();
			shader->setVec3f("amLight[" + to_string(id) + "].amColor", ref->ambientColor);
			shader->setVec3f("amLight[" + to_string(id) + "].diColor", ref->diffuseColor);
			shader->setVec3f("amLight[" + to_string(id) + "].spColor", ref->specularColor);
			shader->set1f("amLight[" + to_string(id) + "].intensity", ref->intensity);
			ref->onAfterRender();
		}

		void update(Shader* shader, Camera* camera) {

		}
	};

	class DirectionalLightRenderer : public Renderer {
	private:
		u32 id;
		DirectionalLight* ref;
	public:
		DirectionalLightRenderer(DirectionalLight* ref, u32 id = 0) {
			this->ref = ref;
			this->id = id;
		}

		void render(Shader* shader) {
			ref->onBeforeRender();
			shader->setVec3f("dirLight[" + to_string(id) + "].direction", ref->direction);
			shader->setVec3f("dirLight[" + to_string(id) + "].base.amColor", ref->ambientColor);
			shader->setVec3f("dirLight[" + to_string(id) + "].base.diColor", ref->diffuseColor);
			shader->setVec3f("dirLight[" + to_string(id) + "].base.spColor", ref->specularColor);
			shader->set1f("dirLight[" + to_string(id) + "].base.intensity", ref->intensity);
			ref->onAfterRender();
		}

		void update(Shader* shader, Camera* camera) {

		}
	};

	class PointLightRenderer : public Renderer {
	private:
		u32 id;
		PointLight* ref;
	public:
		PointLightRenderer(PointLight* ref, u32 id = 0) {
			this->ref = ref;
			this->id = id;
		}

		void render(Shader* shader) {
			ref->onBeforeRender();
			shader->setVec3f("ptLight[" + to_string(id) + "].position", ref->getPosition());
			shader->setVec3f("ptLight[" + to_string(id) + "].base.amColor", ref->ambientColor);
			shader->setVec3f("ptLight[" + to_string(id) + "].base.diColor", ref->diffuseColor);
			shader->setVec3f("ptLight[" + to_string(id) + "].base.spColor", ref->specularColor);
			shader->set1f("ptLight[" + to_string(id) + "].base.intensity", ref->intensity);
			shader->set1f("ptLight[" + to_string(id) + "].constant", ref->constant);
			shader->set1f("ptLight[" + to_string(id) + "].linear", ref->linear);
			shader->set1f("ptLight[" + to_string(id) + "].quadratic", ref->quadratic);
			ref->onAfterRender();
		}

		void update(Shader* shader, Camera* camera) {

		}
	};

	class SpotLightRenderer : public Renderer {
	private:
		u32 id;
		SpotLight* ref;
	public:
		SpotLightRenderer(SpotLight* ref, u32 id = 0) {
			this->ref = ref;
			this->id = id;
		}

		void render(Shader* shader) {
			ref->onBeforeRender();

			shader->setVec3f("spLight[" + to_string(id) + "].direction", ref->direction);
			shader->set1f("spLight[" + to_string(id) + "].cutoff", ref->cutoff);

			shader->setVec3f("spLight[" + to_string(id) + "].base.position", ref->getPosition());
			shader->set1f("spLight[" + to_string(id) + "].base.constant", ref->constant);
			shader->set1f("spLight[" + to_string(id) + "].base.linear", ref->linear);
			shader->set1f("spLight[" + to_string(id) + "].base.quadratic", ref->quadratic);
			
			shader->setVec3f("spLight[" + to_string(id) + "].base.base.amColor", ref->ambientColor);
			shader->setVec3f("spLight[" + to_string(id) + "].base.base.diColor", ref->diffuseColor);
			shader->setVec3f("spLight[" + to_string(id) + "].base.base.spColor", ref->specularColor);
			shader->set1f("spLight[" + to_string(id) + "].base.base.intensity", ref->intensity);
			
			ref->onAfterRender();
		}

		void update(Shader* shader, Camera* camera) {

		}
	};
}