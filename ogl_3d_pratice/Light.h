#pragma once

#include <glm/glm.hpp>
#include "shader_loader.h"
#include <functional>

 class Light {
protected:
	glm::vec3 color;
	float AmbientIntensity;
	float DiffuseIntensity;
	float SpecularIntensity;
public:
	Light(float intensity, glm::vec3 color) {
		this->AmbientIntensity = intensity;
		this->DiffuseIntensity = intensity;
		this->SpecularIntensity = intensity;
		this->color = color;
	}

	Light(float ambientIntensity, float diffuseIntensity, float specularIntensity, glm::vec3 color) {
		this->AmbientIntensity = ambientIntensity;
		this->DiffuseIntensity = diffuseIntensity;
		this->SpecularIntensity = specularIntensity;
		this->color = color;
	}
	~Light() {}

	virtual void AssignToShader(Shader& shader, int index = 0) = 0;
};

// 定向光源
class DirectionLight : public Light {
protected:
	glm::vec3 direction;
public:
	DirectionLight(glm::vec3 direction, float intensity, glm::vec3 color = glm::vec3(1.f)) : Light(intensity, color) {
		this->direction = direction;
	}

	DirectionLight(glm::vec3 direction, float ambientIntensity, float diffuseIntensity, float specularIntensity, glm::vec3 color = glm::vec3(1.f)) :
		Light(ambientIntensity, diffuseIntensity, specularIntensity, color){
		this->direction = direction;
	}
	
	void AssignToShader(Shader& shader, int index = 0) override {
		shader.setVec3f("dirLight.direction", direction);
		shader.setVec3f("dirLight.baselight.color", color);
		shader.set1f("dirLight.baselight.ambientIntensity", AmbientIntensity);
		shader.set1f("dirLight.baselight.diffuseIntensity", DiffuseIntensity);
		shader.set1f("dirLight.baselight.specularIntensity", SpecularIntensity);
	}
};

// 點光源
class PointLight : public Light {
protected:
	glm::vec3 position;
	float constant, linear, quadratic;
public:
	typedef function<void(PointLight*)> BeforeRenderHooks;

	PointLight(glm::vec3 position, 
		glm::vec3 color = glm::vec3(1.f),
		float intensity = 1.f, 
		float constant = 1.f, 
		float linear = 0.09f, 
		float quadratic = 0.032f) : Light(intensity, color) {

		this->position = position;
		this->constant = constant;
		this->linear = linear;
		this->quadratic = quadratic;

		this->beforeRenderHooks = nullptr;
	}

	~PointLight() {}
	
	void setPosition(const glm::vec3 position) {
		this->position = position;
	}

	void setBeforeRenderHooks(BeforeRenderHooks fn) {
		this->beforeRenderHooks = fn;
	}

	void AssignToShader(Shader& shader, int index = 0) override {
		
		if (beforeRenderHooks != nullptr) 
			beforeRenderHooks(this);

		
		shader.setVec3f("mPointLights[" + to_string(index) + "].position", position);

		shader.setVec3f("mPointLights[" + to_string(index) + "].baselight.color", color);
		shader.set1f("mPointLights[" + to_string(index) + "].baselight.ambientIntensity", AmbientIntensity);
		shader.set1f("mPointLights[" + to_string(index) + "].baselight.diffuseIntensity", DiffuseIntensity);
		shader.set1f("mPointLights[" + to_string(index) + "].baselight.specularIntensity", SpecularIntensity);

		shader.set1f("mPointLights[" + to_string(index) + "].constant", constant);
		shader.set1f("mPointLights[" + to_string(index) + "].linear", linear);
		shader.set1f("mPointLights[" + to_string(index) + "].quadratic", quadratic);
	}
private:
	BeforeRenderHooks beforeRenderHooks; // Usually executed before rendering lights
};

// 聚光光源
class SpotLight : public PointLight {
protected:
	glm::vec3 direction;
	float cutoff;
public:
	typedef function<void(SpotLight*)> BeforeRenderHooks;

	SpotLight(glm::vec3 position,
		glm::vec3 direction,
		float cutoff,
		glm::vec3 color = glm::vec3(1.f),
		float intensity = 1.f,
		float constant = 1.f,
		float linear = 0.09f,
		float quadratic = 0.032f
	) : PointLight(position, color, intensity, constant, linear, quadratic) {
		this->direction = direction;
		this->cutoff = cutoff;
		this->beforeRenderHooks = nullptr;
	}

	~SpotLight(){}
	
	// before render hooks
	void setBeforeRenderHooks(BeforeRenderHooks func) {
		this->beforeRenderHooks = func;
	}
	
	void setDirection(glm::vec3 direction) {
		this->direction = direction;
	}

	void setCutoff(float cutoff) {
		this->cutoff = cutoff;
	}

	void AssignToShader(Shader& shader, int index = 0) override {
		// 還好目前只有這三種燈型，不然就變成嵌入地獄= =

		if (this->beforeRenderHooks != nullptr)
			beforeRenderHooks(this);

		shader.setVec3f("mSpotLights[" + to_string(index) + "].direction", direction);
		shader.set1f("mSpotLights[" + to_string(index) + "].cutoff", cutoff);

		// point light
		shader.setVec3f("mSpotLights[" + to_string(index) + "].baselight.position", position);
		shader.set1f("mSpotLights[" + to_string(index) + "].baselight.constant", constant);
		shader.set1f("mSpotLights[" + to_string(index) + "].baselight.linear", linear);
		shader.set1f("mSpotLights[" + to_string(index) + "].baselight.quadratic", quadratic);
		 
		// base light
		shader.setVec3f("mSpotLights[" + to_string(index) + "].baselight.baselight.color", color);
		shader.set1f("mSpotLights[" + to_string(index) + "].baselight.baselight.ambientIntensity", AmbientIntensity);
		shader.set1f("mSpotLights[" + to_string(index) + "].baselight.baselight.diffuseIntensity", DiffuseIntensity);
		shader.set1f("mSpotLights[" + to_string(index) + "].baselight.baselight.specularIntensity", SpecularIntensity);
	}
private:
	BeforeRenderHooks beforeRenderHooks;
};