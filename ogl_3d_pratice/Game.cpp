#include "Game.h"

Game::Game(const char* title,
	const int width, const int height,
	const int GL_VERSION_MAJOR, const int GL_VERSION_MINOR,
	bool resizable) 
	: width(width), 
	  height(height), 
	  GL_VERSION_MAJOR(GL_VERSION_MAJOR), 
	  GL_VERSION_MINOR(GL_VERSION_MINOR),
	  debug_mode(config_debug) {
	
	this->window = nullptr;
	this->frameBufferWidth = this->width;
	this->frameBufferHeight = this->height;

	this->cameraPosition = glm::vec3(0.f, 0.f, 3.f);
	this->camera = new Camera(cameraPosition);
	this->cameraMoveCallback = nullptr;

	this->currentTime = 0.0;
	this->lastTime = 0.0;
	this->deltaTime = 0.0;

	this->initGLFWWindow(title, resizable);
	this->initGLEW();
	this->initOpenGLOpt();

	this->initWorldMatrix();
	this->initShaders();
	this->initModels();
	this->initLights();
}

Game::~Game() {
	glfwDestroyWindow(this->window);
	glfwTerminate();
	
	for (auto& shader : shaders) {
		delete shader;
	}

	for (auto& model : models) {
		delete model;
	}

	for (auto& light : lights) {
		delete light;
	}

	delete skybox;
}

int Game::getWindowShouldClose() {
	return glfwWindowShouldClose(this->window);
}

void Game::setWindowShouldClose() {
	glfwSetWindowShouldClose(this->window, GLFW_TRUE);
}

void Game::updateFPS() {
	currentTime = glfwGetTime();
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;
}

void Game::setKeyboardCallback(GLFWkeyfun func) {
	if (func == nullptr) return;
	glfwSetKeyCallback(this->window, func);
}

void Game::setMouseCallback(GLFWcursorposfun func) {
	if (func == nullptr) return;
	glfwSetCursorPosCallback(this->window, func);
}

void Game::setScrollCallback(GLFWscrollfun func) {
	if (func == nullptr) return;
	glfwSetScrollCallback(this->window, func);
}

void Game::setFrameBufferResizeCallback(GLFWframebuffersizefun func) {
	if (func == nullptr) return;
	glfwSetFramebufferSizeCallback(this->window, func);
}

void Game::setCameraMovementCallback(CameraMovementCallback func) {
	if (func == nullptr) return;
	this->cameraMoveCallback = func;
}

void Game::setUpdateTitleCallback(UpdateTitleCallback func) {
	if (func == nullptr) return;
	this->updateTitleCallback = func;
}

void Game::input() {
	glfwPollEvents();
	if (this->cameraMoveCallback != nullptr)
		this->cameraMoveCallback(this->camera, deltaTime);
}


void Game::initGLFWWindow(const char* title, bool resizable) {
	// 初始化 GLFW
	if (!glfwInit()) { 
		logger->Fatal("Initialize GLFW error"); 
	}

	// 啟用多重採樣 (4x MSAA)
	// document: https://learnopengl.com/Advanced-OpenGL/Anti-Aliasing
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, resizable);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->GL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->GL_VERSION_MINOR);

	window = glfwCreateWindow(this->width, this->height, title, NULL, NULL);
	if (!window) {
		glfwTerminate();
		logger->Fatal("Creating GLFW Window Error");
	}
	glfwGetFramebufferSize(this->window, &this->frameBufferWidth, &this->frameBufferHeight);
	glfwSetFramebufferSizeCallback(this->window, this->DefaultFrameBufferResizeCallback);
	glfwMakeContextCurrent(window);
}

void Game::initGLEW() {
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		logger->Fatal("Initialize GLEW error");
	}
}

void Game::initOpenGLOpt() {
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_ALWAYS);	// always PASS 
	//glDepthFunc(GL_NOTEQUAL); // PASS if incoming depth value is NOT EQUAL to the stored depth value
	glDepthFunc(GL_LESS);		// default, PASS if incoming depth value is LESS than the stored depth value.
								// that means smaller depth value has higher priority than bigger one. 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Game::initWorldMatrix() {
	this->ViewMatrix = glm::mat4(1.0f);
	this->ProjectionMatrix = glm::mat4(1.0f);
	this->ModelMatrix = glm::mat4(1.0f);
	
	this->ViewMatrix = camera->GetViewMatrix();
	this->ProjectionMatrix = glm::perspective(
		camera->Zoom,
		static_cast<float>(this->frameBufferWidth / this->frameBufferHeight),
		camera->Near,
		camera->Far
	);
}

void Game::initShaders() {
	// TODO: pass shader source by constructor
	//this->shaders.push_back(new Shader("./shaders/model.loading.vs", "./shaders/model.loading.frag"));
	this->shaders.push_back(new Shader("./shaders/model.core.vs", "./shaders/model.core.frag"));
	this->shaders.push_back(new Shader("./shaders/skybox.vs", "./shaders/skybox.frag"));
}

void Game::initModels() {
	models.push_back(new Model("./models/HololiveEN_3D/GawrGura/GawrGura.obj"));
	
	// init skybox model and textures
	vector<const GLchar*> faces;
	faces.push_back("./models/Skybox/right.jpg");
	faces.push_back("./models/Skybox/left.jpg");
	faces.push_back("./models/Skybox/top.jpg");
	faces.push_back("./models/Skybox/bottom.jpg");
	faces.push_back("./models/Skybox/back.jpg");
	faces.push_back("./models/Skybox/front.jpg");
	skybox = new Skybox();
	skybox->loadCubeMap(faces);
}

void Game::render() {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	ViewMatrix = camera->GetViewMatrix();
	shaders[SHADER_CORE_PROGRAM]->setMat4fv("view", ViewMatrix);
	shaders[SHADER_CORE_PROGRAM]->setVec3f("cameraPosition", camera->Position);

	glfwGetFramebufferSize(this->window, &this->frameBufferWidth, &this->frameBufferHeight);
	GLfloat aspect = (GLfloat)this->frameBufferWidth / (GLfloat)this->frameBufferHeight;
	ProjectionMatrix = glm::perspective(this->camera->Zoom, aspect, this->camera->Near, this->camera->Far);
	shaders[SHADER_CORE_PROGRAM]->setMat4fv("projection", ProjectionMatrix);

	ModelMatrix = glm::mat4(1.0f);
	ModelMatrix = glm::translate(this->ModelMatrix, glm::vec3(0.0f, -1.75f, 0.0f));
	ModelMatrix = glm::scale(this->ModelMatrix, glm::vec3(0.2f, 0.2f, 0.2f));
	shaders[SHADER_CORE_PROGRAM]->setMat4fv("model", ModelMatrix);

	// render lights
	for (auto& pt : this->lights) {
		pt->AssignToShader(*shaders[SHADER_CORE_PROGRAM]);
	}

	// render model
	for (auto& model : models) {
		model->Draw(*shaders[SHADER_CORE_PROGRAM]);
	}

	// render skybox
	// remove transition from the view matrix
	ViewMatrix = glm::mat4(glm::mat3(camera->GetViewMatrix()));
	shaders[SHADER_SKYBOX]->setMat4fv("view", ViewMatrix);
	shaders[SHADER_SKYBOX]->setMat4fv("projection", ProjectionMatrix);
	skybox->Draw(*shaders[SHADER_SKYBOX]);

	glfwSwapBuffers(this->window);
	// unbind avoid somethere forgot to unbind vertex array, shaders ...etc
	glBindVertexArray(0);
	glUseProgram(0);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Game::update() {
	updateFPS();
	if (this->updateTitleCallback != nullptr) {
		glfwSetWindowTitle(this->window, updateTitleCallback().c_str());
	}
	this->input();
}

void Game::CameraProcessKeyboard(CameraDirection direction, float deltaTime) {
	this->camera->ProcessKeyboard(direction, deltaTime);
}

void Game::CameraProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
	this->camera->ProcessMouseMovement(xoffset, yoffset, constrainPitch);
}

void Game::CameraProcessMouseScroll(float yoffset) {
	this->camera->ProcessMouseScroll(yoffset);
}

double Game::getFPS() {
	return 1.0 / this->deltaTime;
}

void Game::initLights() {
	this->initDirectionLights();
	this->initPointLights();
}

void Game::initPointLights()  {
	this->lights.push_back(new PointLight(glm::vec3(0.f, 10.f, 0.f), glm::vec3(1.f, 0.0f, 0.f), 0.5f));
}

void Game::initDirectionLights() {
	this->lights.push_back(new DirectionLight(glm::vec3(0.f, 10.f, 50.f), 0.8f, 0.8f, 0.8f));
}