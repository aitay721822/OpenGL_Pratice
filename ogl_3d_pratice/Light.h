#pragma once

#include <glm/glm.hpp>
#include "shader_loader.h"

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

	virtual void AssignToShader(Shader& shader) = 0;
};

// ©w¦V¥ú·½
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
	
	void AssignToShader(Shader& shader) {
		shader.setVec3f("dirLight.direction", direction);
		shader.setVec3f("dirLight.baselight.color", color);
		shader.set1f("dirLight.baselight.ambientIntensity", AmbientIntensity);
		shader.set1f("dirLight.baselight.diffuseIntensity", DiffuseIntensity);
		shader.set1f("dirLight.baselight.specularIntensity", SpecularIntensity);
	}
};

class PointLight : public Light {
protected:
	glm::vec3 position;
	float constant, linear, quadratic;
public:
	
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
	}

	~PointLight() {}
	
	void setPosition(const glm::vec3 position) {
		this->position = position;
	}

	void AssignToShader(Shader& shader) {
		shader.setVec3f("pointlight.position", position);

		shader.setVec3f("pointlight.baselight.color", color);
		shader.set1f("pointlight.baselight.ambientIntensity", AmbientIntensity);
		shader.set1f("pointlight.baselight.diffuseIntensity", DiffuseIntensity);
		shader.set1f("pointlight.baselight.specularIntensity", SpecularIntensity);

		shader.set1f("pointlight.constant", constant);
		shader.set1f("pointlight.linear", linear);
		shader.set1f("pointlight.quadratic", quadratic);
	}
};


