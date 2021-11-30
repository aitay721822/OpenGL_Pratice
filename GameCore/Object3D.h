#pragma once

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Logger.h"
#include "ObjectType.h"
#include "matrix_helper.h"
#include "vector_helper.h"

// three.js is done very well.. just refer its code implementation

using namespace std;
using namespace glm;

namespace GameCore {
	const vec3 xAxis = vec3(1.f, 0.f, 0.f);
	const vec3 yAxis = vec3(0.f, 1.f, 0.f);
	const vec3 zAxis = vec3(0.f, 0.f, 1.f);

	const vec3 DefaultUp = vec3(0.f, 1.f, 0.f);
	static u32 Object3DGlobalId = 0;

	class Object3D {
	private:
		Logger logger = Logger("Object3D");
	protected:
		// object properties
		u32 id;
		ObjectType type;
		string name;

		// 3d properties
		vec3 position;
		quat quaternion;
		vec3 scale;
		vec3 up;
		
		// local matrix and world matrix
		mat4 matrix;
		mat4 matrixWorld;

		// update properties
		bool matrixWorldNeedsUpdate;

	public:
		Object3D() {
			this->id = Object3DGlobalId++;
			this->name = "";
			this->type = ObjectType::Object3D;

			this->up = vec3(DefaultUp);
			this->position = vec3(0.f, 0.f, 0.f);
			this->quaternion = quat(1.f, 0.f, 0.f, 0.f);
			this->scale = vec3(1, 1, 1);

			this->matrix = mat4(1.f);
			this->matrixWorld = mat4(1.f);

			matrixWorldNeedsUpdate = false;
		}

		~Object3D() {
		}
		// !------ get
		u32 getId() { return this->id;}
		ObjectType getObjectType() { return this->type; }
		string getName() { return this->name; }
		vec3 getPosition() { return this->position; }
		quat getQuaternion() { return this->quaternion; }
		vec3 getScale() { return this->scale; }
		vec3 getUpVector() { return this->up; }
		mat4 getMatrix() { return this->matrix; }
		mat4 getWorldMatrix() {
			if (matrixWorldNeedsUpdate) logger.Info("Object3D(%d).getWorldMatrix: world matrix not update.", this->id);
			return this->matrixWorld;
		}
		bool getWorldMatrixNeedsUpdate() { return this->matrixWorldNeedsUpdate; }
		// !------ get

		// !------ set
		void setName(string name) { this->name = name; }
		void setPosition(vec3 position) { this->position = position; }
		void setQuaternion(quat quaternion) { this->quaternion = quaternion; }
		void setScale(vec3 scale) { this->scale = scale; }
		// !------ set

		void onBeforeRender() { }
		void onAfterRender() { }

		void applyMatrix4(mat4 m) {
			this->updateMatrix();
			// premultiply this by m
			this->matrix = this->matrix * m;
			Math::decompose(this->matrix, this->position, this->quaternion, this->scale);
		}

		void applyQuat(quat q) {
			this->quaternion = q * this->quaternion;
		}

		void setRotationFromAxisAngle(vec3 axis, float angle) {
			this->quaternion = angleAxis(angle, axis);
		}

		// there has only one way to set euler angle to quaternion
		// just simply convert euler angle to rotation matrix and 
		// convert rotation matrix to quaternion
		void setRotationFromRotationMatrix(mat4 rm) {
			this->quaternion = toQuat(rm);
		}

		void setRotationFromRotationMatrix(mat3 rm) {
			this->quaternion = toQuat(rm);
		}

		void setRotationFromQuaternion(quat q) {
			this->quaternion = quat(q);
		}

		Object3D* rotateOnAxis(vec3 axis, float angle) {
			quat q = angleAxis(angle, axis);
			this->quaternion *= q;
			return this;
		}

		Object3D* rotateX(float angle) {
			return this->rotateOnAxis(xAxis, angle);
		}

		Object3D* rotateY(float angle) {
			return this->rotateOnAxis(yAxis, angle);
		}

		Object3D* rotateZ(float angle) {
			return this->rotateOnAxis(zAxis, angle);
		}

		// TODO: validate this is correct
		Object3D* translateOnAxis(vec3 axis, float distance) {
			vec3 len = Math::applyQuaternion(axis, this->quaternion);
			position += len * distance;
			return this;
		}

		Object3D* translateX(float distance) {
			return this->translateOnAxis(xAxis, distance);
		}

		Object3D* translateY(float distance) {
			return this->translateOnAxis(yAxis, distance);
		}

		Object3D* translateZ(float distance) {
			return this->translateOnAxis(zAxis, distance);
		}

		vec3 localToWorld(vec3 v) {
			return Math::applyMatrix4(v, this->matrixWorld);
		}

		vec3 worldToLocal(vec3 v) {
			return Math::applyMatrix4(v, inverse(this->matrixWorld));
		}

		void lookAt(vec3 pos) {
			vec3 eye = Math::matrixPosition(this->matrixWorld);

			mat4 lookAtMat4(1.f);
			if (isCamera(type) || isLight(type)) {
				lookAtMat4 = glm::lookAt(eye, pos, this->up);
			}
			else {
				lookAtMat4 = glm::lookAt(pos, eye, this->up);
			}

			setRotationFromRotationMatrix(lookAtMat4);
		}

		vec3 getWorldPosition() {
			return Math::matrixPosition(this->matrixWorld);
		}

		quat getWorldQuaternion(quat target) {
			Math::decompose(this->matrixWorld, this->position, target, this->scale);
			return target;
		}

		vec3 getWorldScale(vec3 target) {
			Math::decompose(this->matrixWorld, this->position, this->quaternion, target);
			return target;
		}

		vec3 getWorldDirection() {
			return normalize(vec3(this->matrixWorld[2][0], this->matrixWorld[2][1], this->matrixWorld[2][2]));
		}

		void updateMatrix() {
			Math::compose(this->matrix, this->position, this->quaternion, this->scale);
			// set children need update its world matrix
			this->matrixWorldNeedsUpdate = true;
		}

		void updateWorldMatrix(mat4 transformMatrix) {
			this->updateMatrix();
			this->matrixWorld = this->matrix * transformMatrix;
			this->matrixWorldNeedsUpdate = false;
		}
	};
}