#pragma once

#include <functional>

#include <GLFW/glfw3.h>

#include <GL/glew.h>

#include "Renderer.h"
#include "Camera.h"
#include "PerspectiveCamera.h"
#include "shader_loader.h"
#include "Scene.h"
#include "LightRenderer.h"
#include "MeshRenderer.h"
#include "SkyboxRenderer.h"


using namespace std;
using namespace glm;

namespace GameCore {

	typedef function<string()> UpdateTitleCallback;
	typedef function<void(Camera*, double)> CameraMovementCallback;

	enum class ShaderType {
		ShaderCore,
		ShaderSkybox
	};

	class OpenGLRenderer {
	private:
		/* Logger Settings */
		Logger logger = Logger("OpenGLRenderer");

		/* Window Settings */
		GLFWwindow* window;
		const int width;
		const int height;
		const int GL_VERSION_MAJOR;
		const int GL_VERSION_MINOR;
		int frameBufferWidth;
		int frameBufferHeight;

		/* FPS variable */
		double currentTime;
		double lastTime;
		double deltaTime;

		/* Camera Settings */
		Camera* camera;
		CameraMovementCallback cameraMoveCallback;
		UpdateTitleCallback updateTitleCallback;

		/* Shader List */
		unordered_map<ShaderType, Shader*> shaders;

		/* Object Renderer */
		unordered_map<u32, Renderer*> renderer;

		/* Default Callback */
		static void DefaultFrameBufferResizeCallback(GLFWwindow* window, int w, int h) {
			glViewport(0, 0, w, h);
		}

		void initGLFWWindow(const char* title, bool resizable) {
			if (!glfwInit()) {
				logger.Fatal("Initialize GLFW error");
			}
			// 啟用多重採樣 (4x MSAA)
			// document: https://learnopengl.com/Advanced-OpenGL/Anti-Aliasing
			glfwWindowHint(GLFW_SAMPLES, 4);
			glfwWindowHint(GLFW_RESIZABLE, resizable);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
			
			window = glfwCreateWindow(this->width, this->height, title, NULL, NULL);
			if (!window) {
				glfwTerminate();
				logger.Fatal("Creating GLFW Window Error");
			}
			glfwGetFramebufferSize(this->window, &this->frameBufferWidth, &this->frameBufferHeight);
			glfwSetFramebufferSizeCallback(this->window, this->DefaultFrameBufferResizeCallback);
			glfwMakeContextCurrent(window);
		}

		void initGLEW() {
			glewExperimental = GL_TRUE;
			if (glewInit() != GLEW_OK) {
				logger.Fatal("Initialize GLEW error");
			}
		}
		
		void initOpenGLOptions() {
			glEnable(GL_DEPTH_TEST);
			//glDepthFunc(GL_ALWAYS);	// always PASS 
			//glDepthFunc(GL_NOTEQUAL); // PASS if incoming depth value is NOT EQUAL to the stored depth value
			glDepthFunc(GL_LESS);		// default, PASS if incoming depth value is LESS than the stored depth value.
										// that means smaller depth value has higher priority than bigger one. 
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}


		void setupLightView(Scene* scene) {
			Shader* shader = getShader(ShaderType::ShaderCore);
			if (shader == nullptr) {
				logger.Err("Shader: ShaderCore is missing");
				return;
			}

			int numOfAmbientLights = 0,
				numOfDirectionalLights = 0,
				numOfPointLights = 0,
				numOfSpotLights = 0;

			vector<Light*> lights = scene->getLights();
			for (auto& light : lights) {
				u32 id = light->getId();
				// try to find renderer
				if (renderer.find(id) == renderer.end()) {
					ObjectType type = light->getObjectType();
					if (type == ObjectType::AmbientLight) {
						AmbientLight* amLight = static_cast<AmbientLight*>(light);
						AmbientLightRenderer* r = new AmbientLightRenderer(amLight, numOfAmbientLights++);
						this->renderer.insert({ id, r });
					}
					else if (type == ObjectType::DirectionalLight) {
						DirectionalLight* dirLight = static_cast<DirectionalLight*>(light);
						DirectionalLightRenderer* r = new DirectionalLightRenderer(dirLight, numOfDirectionalLights++);
						this->renderer.insert({ id, r });
					}
					else if (type == ObjectType::PointLight) {
						PointLight* ptLight = static_cast<PointLight*>(light);
						PointLightRenderer* r = new PointLightRenderer(ptLight, numOfPointLights++);
						this->renderer.insert({ id, r });
					}
					else if (type == ObjectType::SpotLight) {
						SpotLight* spLight = static_cast<SpotLight*>(light);
						SpotLightRenderer* r = new SpotLightRenderer(spLight, numOfSpotLights++);
						this->renderer.insert({ id, r });
					}
					else {
						// skip other type
						continue;
					}
				}
				Renderer* r = renderer[id];
				r->update(shader, camera);
				r->render(shader);
			}
		}

		void renderScene(Scene* scene, Camera* camera) {
			// render background
			Object3D* background = scene->getBackground();
			if (background != nullptr) {
				logger.Info("background object is nullptr, skip rendering!");
			}
			else {
				renderBackground(background, camera);
			}
			// setup lights
			setupLightView(scene);
			// render objects
			renderObjects(scene, camera);
		}

		void renderObjects(Object3DNode* node, Camera* camera) {
			// render mesh
			for (auto& mesh : node->getMeshes()) {
				renderMesh(mesh, camera);
			}

			for (auto& child : node->getChildren()) {
				renderObjects(child, camera);
			}
		}

		void renderMesh(Mesh* mesh, Camera* camera) {
			Shader* shader = getShader(ShaderType::ShaderCore);
			if (shader == nullptr) {
				logger.Err("Shader: ShaderCore is missing");
				return;
			}
			u32 id = mesh->getId();
			// try to find renderer
			if (renderer.find(id) == renderer.end()) {
				MeshRenderer* r = new MeshRenderer(mesh);
				this->renderer.insert({ id, r });
			}
			Renderer* r = renderer[id];
			r->update(shader, camera);
			r->render(shader);
		}

		void renderBackground(Object3D* background, Camera* camera) {
			if (background->getObjectType() == ObjectType::Skybox) {
				Shader* shader = getShader(ShaderType::ShaderSkybox);
				if (shader != nullptr) {
					u32 id = background->getId();
					// try to find renderer
					if (renderer.find(id) == renderer.end()) {
						// init new renderer and register it.
						Skybox* skybox = static_cast<Skybox*>(background);
						SkyboxRenderer* r = new SkyboxRenderer(skybox);
						this->renderer.insert({ id, r });
					}
					Renderer* r = renderer[id];
					r->update(shader, camera);
					r->render(shader);
				}
				else {
					logger.Info("Shader: ShaderSkybox is missing.");
				}
			}
			else {
				logger.Info("background type : %s, not support rendering this as background", background->getObjectType());
			}
		}

		void renderView(Camera* camera) {
			Shader* shader = getShader(ShaderType::ShaderCore);
			if (shader == nullptr) {
				logger.Err("Shader: ShaderCore is missing");
				return;
			}
			shader->setVec3f("cameraPosition", camera->getPosition());
			shader->setMat4fv("viewMatrix", camera->getWorldMatrix());
			shader->setMat4fv("projectionMatrix", camera->getProjectionMatrix());
		}

		void updateFPS() {
			currentTime = glfwGetTime();
			deltaTime = currentTime - lastTime;
			lastTime = currentTime;
		}

	public:
		OpenGLRenderer(const char* title = "OpenGL Demo",
			           const int width = 1280, 
					   const int height = 720,
					   const int GL_VERSION_MAJOR = 4,
					   const int GL_VERSION_MINOR = 2,
			           bool resizable = false) : width(width), height(height), GL_VERSION_MAJOR(GL_VERSION_MAJOR), GL_VERSION_MINOR(GL_VERSION_MINOR) {

			this->window = nullptr;
			this->frameBufferHeight = this->width;
			this->frameBufferHeight = this->height;

			this->camera = new PerspectiveCamera();

			initGLFWWindow(title, resizable);
			initGLEW();
			initOpenGLOptions();
		}

		~OpenGLRenderer() {
			glfwDestroyWindow(this->window);
			glfwTerminate();
			
			for (auto& shader : this->shaders) {
				delete shader.second;
			}
		}

		void setCamera(Camera* camera) {
			this->camera = camera;
		}

		int getWindowShouldClose() { 
			return glfwWindowShouldClose(this->window); 
		}

		void setWindowShouldClose() { 
			glfwSetWindowShouldClose(this->window, GLFW_TRUE);
		}

		void setKeyboardCallback(GLFWkeyfun func) {
			if (func == nullptr) return;
			glfwSetKeyCallback(this->window, func);
		}
		
		void setMouseCallback(GLFWcursorposfun func) {
			if (func == nullptr) return;
			glfwSetCursorPosCallback(this->window, func);
		}
		
		void setFrameBufferResizeCallback(GLFWframebuffersizefun func) {
			if (func == nullptr) return;
			glfwSetFramebufferSizeCallback(this->window, func);
		}
		
		void setCameraMovementCallback(CameraMovementCallback func) {
			if (func == nullptr) return;
			this->cameraMoveCallback = func;
		}

		void setUpdateTitleCallback(UpdateTitleCallback func) {
			if (func == nullptr) return;
			this->updateTitleCallback = func;
		}

		double getFPS() {
			return 1.0 / this->deltaTime;
		}

		void ProcessMouseMovement(float xoffset, float yoffset) {
			this->camera->ProcessMouseMovement(xoffset, yoffset);
		}

		bool addShader(ShaderType type, string vtxPath, string fragPath) {
			if (shaders.find(type) != shaders.end()) {
				logger.Err("shader name is already exists: %s", type);
				return false;
			}
			
			Shader* s = new Shader(vtxPath.c_str(), fragPath.c_str());
			if (s == nullptr) {
				return false;
			}
			shaders.insert({ type, s });
			return true;
		}

		Shader* getShader(ShaderType key) {
			if (shaders.find(key) == shaders.end()) {
				logger.Err("shader not exists: %s", key);
				return nullptr;
			}
			return shaders[key];
		}
		
		void render(Scene* scene) {
			if (scene == nullptr || camera == nullptr) { 
				logger.Err("Scene or Camera is incorrectly set to nullptr.");
				return; 
			}

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			if (scene->getAutoUpdate()) {
				scene->updateWorldMatrix();
			}
			// The camera's world matrix is set to 1.f by default.
			camera->updateWorldMatrix();
			renderView(camera);

			scene->onBeforeRender();
			renderScene(scene, camera);
			scene->onAfterRender();

			glfwSwapBuffers(this->window);
			glBindVertexArray(0);
			glUseProgram(0);
			glActiveTexture(0);
		}

		void update() {
			updateFPS();
			if (this->updateTitleCallback != nullptr) {
				glfwSetWindowTitle(this->window, updateTitleCallback().c_str());
			}
			this->input();
		}

		void input() {
			glfwPollEvents();
			if (this->cameraMoveCallback != nullptr)
				this->cameraMoveCallback(this->camera, deltaTime);
		}
	};
}