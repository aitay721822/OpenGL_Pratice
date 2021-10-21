#pragma once

#include "Camera.h"
#include <vector>
#include <functional>
#include "Logger.h"
#include "shader_loader.h"
#include "Mesh.h"
#include "Skybox.h"
#include "Model.h"
#include "Light.h"

typedef function<string()> UpdateTitleCallback;
typedef function<void(Camera*, double)> CameraMovementCallback;

enum ShaderEnum { SHADER_CORE_PROGRAM = 0, SHADER_SKYBOX = 1 };

class Game
{
private:

	/* Window Setting */
	GLFWwindow* window;
	const int width;
	const int height;
	int frameBufferWidth;
	int frameBufferHeight;

	/* OpenGL Version */
	const int GL_VERSION_MAJOR;
	const int GL_VERSION_MINOR;
	
	/* FPS Variable */
	double currentTime;
	double lastTime;
	double deltaTime;

	/* Camera Settings */
	Camera* camera;
	glm::vec3 cameraPosition;
	CameraMovementCallback cameraMoveCallback;

	/* Matrix */
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;
	glm::mat4 ModelMatrix;

	/* Logger Setting */
	bool debug_mode = false;

	/* Model, Skybox, Lights(Direction Light, Point Light, Spot Light) variable */
	std::vector<Shader*> shaders;
	std::vector<Model*> models;
	std::vector<DirectionLight*> dirLights;
	std::vector<PointLight*> pointLights;
	std::vector<SpotLight*> spotLights;
	Skybox* skybox;

	/* Init Functions */
	void initGLFWWindow(const char* title, bool resizable);
	void initGLEW();
	void initOpenGLOpt();
	
	void initWorldMatrix();
	void initShaders();
	void initModels();
	void initPointLights();
	void initDirectionLights();
	void initSpotLights();
	void initLights();

	/* internal update function */
	void updateFPS();
	UpdateTitleCallback updateTitleCallback;

	/* Default Callbacks */
	static void DefaultFrameBufferResizeCallback(GLFWwindow* window, int w, int h) {
		glViewport(0, 0, w, h);
	}

public:
	Logger* logger = new Logger("Game", debug_mode);

	Game(const char* title,
		 const int width, const int height,
		 const int GL_VERSION_MAJOR, const int GL_VERSION_MINOR,
		 bool resizable);
	virtual ~Game();

	int getWindowShouldClose();
	void setWindowShouldClose();
	
	void setCameraMovementCallback(CameraMovementCallback func);
	void setMouseCallback(GLFWcursorposfun func);
	void setKeyboardCallback(GLFWkeyfun func);
	void setScrollCallback(GLFWscrollfun func);
	void setFrameBufferResizeCallback(GLFWframebuffersizefun func);
	void setUpdateTitleCallback(UpdateTitleCallback func);

	// expose camera function for some callback...
	void CameraProcessKeyboard(CameraDirection direction, float deltaTime);
	void CameraProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
	void CameraProcessMouseScroll(float yoffset);
	
	glm::vec3 CameraGetPosition();

	double getFPS();

	void input();
	void update();
	void render();
};
