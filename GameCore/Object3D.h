#pragma once

#include <string>
#include <functional>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "vector_helper.h"
#include "Logger.h"
#include "matrix_helper.h"
#include "ObjectType.h"

// three.js is done very well.. just refer its code implementation

using namespace std;
using namespace glm;

namespace GameCore {
	const vec3 xAxis = vec3(1.f, 0.f, 0.f);
	const vec3 yAxis = vec3(0.f, 1.f, 0.f);
	const vec3 zAxis = vec3(0.f, 0.f, 1.f);

	const vec3 DefaultUp = vec3(0.f, 1.f, 0.f);
	const bool DefaultMatrixAutoUpdate = true;
	static u32 Object3DGlobalId = 0;

	class Object3D {
	private:
		Logger logger = Logger("Object3D");
	public:
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
		bool matrixAutoUpdate;
		bool matrixWorldNeedsUpdate;

		Object3D* parent = nullptr;
		vector<Object3D*> children;

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

			matrixAutoUpdate = DefaultMatrixAutoUpdate;
			matrixWorldNeedsUpdate = false;
		}

		~Object3D() {
			this->parent = nullptr;
			for (auto& child : children) {
				delete child;
			}
		}

		void onBeforeRender() { }
		void onAfterRender() { }

		void applyMatrix4(mat4 m) {
			if (this->matrixAutoUpdate) this->updateMatrix();
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
			// transform local position to world position
			return Math::applyMatrix4(v, this->matrixWorld);
		}

		vec3 worldToLocal(vec3 v) {
			return Math::applyMatrix4(v, inverse(this->matrixWorld));
		}

		void lookAt(vec3 pos) {
			Object3D* parent = this->parent;
			this->updateWorldMatrix(true, false);

			vec3 eye = Math::matrixPosition(this->matrixWorld);

			mat4 lookAtMat4(1.f);
			if (isCamera(type) || isLight(type)) {
				lookAtMat4 = glm::lookAt(eye, pos, this->up);
			}
			else {
				lookAtMat4 = glm::lookAt(pos, eye, this->up);
			}

			setRotationFromRotationMatrix(lookAtMat4);
			if (parent != nullptr) {
				mat4 rotMat4 = Math::extractRotation(parent->matrixWorld);
				quat rotQuat = toQuat(rotMat4);
				// caution
				this->quaternion = inverse(rotQuat) * this->quaternion;
			}
		}

		Object3D* add(Object3D* child) {
			if (this == child) {
				logger.Err("ObjectID: %d | Object3D.add: object can't be added as a child of itself.", child->id);
				return this;
			}

			if (child != nullptr) {
				if (child->parent != nullptr) {
					child->parent->remove(child);
				}

				child->parent = this;
				this->children.push_back(child);

			}
			else {
				logger.Err("Object3D.add: object cannot be nullptr");
			}
			return this;
		}

		Object3D* remove(Object3D* child) {
			if (child != nullptr) {
				std::remove(children.begin(), children.end(), child);
			}
			else {
				logger.Err("Object3D.remove: object cannot be nullptr");
			}
			return this;
		}

		Object3D* removeFromParent() {
			Object3D* parent = this->parent;
			if (parent != nullptr)
				parent->remove(this);
			return this;
		}

		Object3D* clear() {
			for (auto& child : children) {
				child->parent = nullptr;
			}
			children.clear();
			return this;
		}

		Object3D* attach(Object3D* obj) {
			updateWorldMatrix(true, false);

			mat4 wm = inverse(this->matrixWorld);
			if (obj->parent != nullptr) {
				obj->parent->updateWorldMatrix(true, false);
				// backward
				wm = obj->parent->matrixWorld * wm;
			}
			obj->applyMatrix4(wm);
			this->add(obj);
			obj->updateWorldMatrix(false, true);
			return this;
		}

		Object3D* getObjectById(int id) {
			if (this->id == id)
				return this;
			for (auto& child : children) {
				Object3D* obj = child->getObjectById(id);
				if (obj != nullptr) {
					return obj;
				}
			}
			return nullptr;
		}

		Object3D* getObjectByName(string name) {
			if (this->name == name)
				return this;
			for (auto& child : children) {
				Object3D* obj = child->getObjectByName(name);
				if (obj != nullptr) {
					return obj;
				}
			}
			return nullptr;
		}

		vec3 getWorldPosition() {
			this->updateWorldMatrix(true, false);
			return Math::matrixPosition(this->matrixWorld);
		}

		quat getWorldQuaternion(quat target) {
			this->updateWorldMatrix(true, false);
			Math::decompose(this->matrixWorld, this->position, target, this->scale);
			return target;
		}

		vec3 getWorldScale(vec3 target) {
			this->updateWorldMatrix(true, false);
			Math::decompose(this->matrixWorld, this->position, this->quaternion, this->scale);
			return target;
		}

		vec3 getWorldDirection() {
			this->updateWorldMatrix(true, false);
			return normalize(vec3(this->matrixWorld[2][0], this->matrixWorld[2][1], this->matrixWorld[2][2]));
		}

		void updateWorldMatrix(bool updateParents, bool updateChildren) {
			Object3D* parent = this->parent;
			if (updateParents && parent != nullptr) {
				parent->updateWorldMatrix(true, false);
			}

			if (matrixAutoUpdate) updateMatrix();
			if (parent == nullptr) {
				this->matrixWorld = mat4(this->matrix);
			}
			else {
				this->matrixWorld = this->matrix * this->parent->matrixWorld;
			}

			if (updateChildren) {
				for (auto& child : children) {
					child->updateWorldMatrix(false, true);
				}
			}
		}

		void updateMatrix() {
			Math::compose(this->matrix, this->position, this->quaternion, this->scale);
			// set children need update its world matrix
			this->matrixWorldNeedsUpdate = true;
		}

		void updateMatrixWorld(bool force) {
			if (this->matrixAutoUpdate) this->updateMatrix();
			if (this->matrixAutoUpdate || force) {
				if (this->parent == nullptr) {
					this->matrixWorld = mat4(this->matrix);
				}
				else {
					// glm mat4 multiplication is backward!
					// this.parent.matrixWorld * this.matrix
					this->matrixWorld = this->matrix * this->parent->matrixWorld;
				}
				this->matrixWorldNeedsUpdate = false;
				force = true;
			}
			// update children
			auto children = this->children;
			for (auto& child : children) {
				child->updateMatrixWorld(force);
			}
		}
	};
}