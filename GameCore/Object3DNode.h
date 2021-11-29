#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Logger.h"
#include "Mesh.h"

using namespace std;
using namespace glm;

namespace GameCore {
	static u32 Object3DNodeGlobalId = 0;

	class Object3DNode {
	private:
		Logger logger = Logger("Object3DNode");

		/* Object3DNode properties */
		u32 id;
		string name;

		/* node properties */
		Object3DNode* parent;
		vector<Object3DNode*> children;

		/* each node has multiple meshes */
		vector<Mesh*> meshes;

		/* used to transform children's local space to world space */
		mat4 worldMatrix;
	public:
		Object3DNode(string name = "") {
			this->id = Object3DNodeGlobalId++;
			this->name = name;
			this->parent = parent;
			this->worldMatrix = mat4(1.f);
		}

		~Object3DNode() {
			this->parent = nullptr;
			for (auto& child : children) {
				delete child;
			}
		}

		// !----- get
		u32 getId() { return this->id; }
		string getName() { return this->name; }
		Object3DNode* getParent() { return this->parent; }
		vector<Object3DNode*> getChildren() { return this->children; }
		vector<Mesh*> getMeshes() { return this->meshes; }
		mat4 getWorldMatrix() { return this->worldMatrix; }
		// !----- set
		void setName(string name) { this->name = name; }
		void setParent(Object3DNode* parent) { this->parent = parent; }
		void setWorldMatrix(mat4 matrix) { this->worldMatrix = matrix; }

		void addMesh(Mesh* mesh) {
			mesh->updateWorldMatrix(this->worldMatrix);
			this->meshes.push_back(mesh);
		}

		void onBeforeRender() { }
		void onAfterRender() { }

		Object3DNode* add(Object3DNode* child) {
			if (this == child) {
				logger.Err("Object3DNode(%d).add: object can't be added as a child of itself.", child->id);
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
				logger.Err("Object3DNode.add: object cannot be nullptr");
			}
			return this;
		}

		Object3DNode* remove(Object3DNode* child) {
			if (child != nullptr) {
				std::remove(children.begin(), children.end(), child);
			}
			else {
				logger.Err("Object3D.remove: object cannot be nullptr");
			}
			return this;
		}

		Object3DNode* removeFromParent() {
			Object3DNode* parent = this->parent;
			if (parent != nullptr)
				parent->remove(this);
			return this;
		}

		Object3DNode* clear() {
			for (auto& child : children) {
				child->parent = nullptr;
			}
			children.clear();
			return this;
		}

		Object3DNode* getObjectById(int id) {
			if (this->id == id)
				return this;
			for (auto& child : children) {
				Object3DNode* obj = child->getObjectById(id);
				if (obj != nullptr) {
					return obj;
				}
			}
			return nullptr;
		}

		Object3DNode* getObjectByName(string name) {
			if (this->name == name)
				return this;
			for (auto& child : children) {
				Object3DNode* obj = child->getObjectByName(name);
				if (obj != nullptr) {
					return obj;
				}
			}
			return nullptr;
		}

		//Object3DNode* attach(Object3DNode* obj) {
		//	updateWorldMatrix(true, false);
		//	
		//	mat4 wm = inverse(this->worldMatrix);
		//	if (obj->parent != nullptr) {
		//		obj->parent->updateWorldMatrix(true, false);
		//		// backward
		//		wm = obj->parent->worldMatrix * wm;
		//	}
		//	
		//	for (auto& mesh : obj->meshes) {
		//		mesh->applyMatrix4(wm);
		//	}
		//	this->add(obj);
		//	obj->updateWorldMatrix(false, true);
		//	return this;
		//}

		void updateWorldMatrix(bool updateParents, bool updateChildren) {
			// update parent transformation matrix and meshes world matrix
			Object3DNode* parent = this->parent;
			if (updateParents && parent != nullptr) {
				parent->updateWorldMatrix(true, false);
			}
			
			// update meshes local matrix.
			for (auto& mesh : meshes) {
				mesh->updateMatrix();
			}

			if (parent != nullptr) {
				// update global transformation matrix
				this->worldMatrix = this->worldMatrix * this->parent->worldMatrix;
			}

			// update meshes world matrix
			for (auto& mesh : meshes) {
				if (mesh->getWorldMatrixNeedsUpdate()) {
					mesh->updateWorldMatrix(this->worldMatrix);
				}
			}

			// update children transformation matrix and meshes world matrix
			if (updateChildren) {
				for (auto& child : children) {
					child->updateWorldMatrix(false, true);
				}
			}
		}

		// force to update meshes world matrix but not update parent world matrix
		void updateWorldMatrix() {
			for (auto& mesh : meshes) {
				mesh->updateMatrix();					
				mesh->updateWorldMatrix(this->worldMatrix);
				// update cchildren's
				for (auto& child : children) {
					child->updateWorldMatrix();
				}
			}
		}
	};
}