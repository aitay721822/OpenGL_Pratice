#pragma once

#include "Camera.h"
#include <vector>
#include "Logger.h"
#include "shader_loader.h"
#include "Mesh.h"
#include "Skybox.h"
#include "Model.h"

typedef void (*CameraMovementCallback)(Camera*, double dt);

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

	/* Model, Texture, Material, Model variable */
	std::vector<Shader*> shaders;
	std::vector<Model*> models;
	Skybox* skybox;

	/* Init Functions */
	void initGLFWWindow(const char* title, bool resizable);
	void initGLEW();
	void initOpenGLOpt();
	
	void initWorldMatrix();
	void initShaders();
	void initModels();

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

	// expose camera function for some callback...
	void CameraProcessKeyboard(CameraDirection direction, float deltaTime);
	void CameraProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
	void CameraProcessMouseScroll(float yoffset);

	void updateFPS();
	void input();
	void update();
	void render();
};
