#pragma once


using namespace std;
using namespace glm;

namespace GameCore {
	class Scene : public Object3DNode {
	private:
		/* Scene properties */
		bool autoUpdate;
		/* Background properties */
		Object3D* background;
		/* Lights */
		vector<Light*> lights;
	public:
		Scene() : Object3DNode() {
			this->background = nullptr;
			this->autoUpdate = true;
		}

		Object3D* getBackground() {
			return this->background;
		}

		void setBackground(Object3D* background) {
			this->background = background;
		}

		vector<Light*> getLights() {
			return this->lights;
		}

		void setLights(Light* light) {
			this->lights.push_back(light);
		}

		void setLights(vector<Light*>::iterator begin, vector<Light*>::iterator end) {
			this->lights = vector<Light*>(begin, end);
		}

		bool getAutoUpdate() {
			return this->autoUpdate;
		}

		void setAutoUpdate(bool update) {
			this->autoUpdate = update;
		}
	};
}