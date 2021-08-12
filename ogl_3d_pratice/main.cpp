#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Logger.h"
#include "Constants.h"
#include <iostream>
#include "shader_loader.h"
#include "Model.h"
#include "Camera.h"

#define WIDTH 1280
#define HEIGHT 720
#define WINDOW_TITLE "OpenGL"

bool keys[1024];

double lastTime = 0.0, currentTime = 0.0, deltaTime = 0.0;
bool firstMouse = true;
GLfloat lastX, lastY;

GLFWwindow* window;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
Shader* shader;
Model* model;// , * roadModel;

// Logger
Logger logger("GameLoop", config_debug);

void update() {

}

void draw() {
	// set background color
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader->Use();

	// projection
	GLfloat aspect = (GLfloat)WIDTH / (GLfloat)HEIGHT;
	glm::mat4 mProjection = glm::perspective(camera.Zoom, aspect, 0.1f, 1000.0f);
	glUniformMatrix4fv(
		glGetUniformLocation(shader->program, "projection"),
		1, GL_FALSE, glm::value_ptr(mProjection)
	);

	// view
	glm::mat4 mView = camera.GetViewMatrix();
	glUniformMatrix4fv(
		glGetUniformLocation(shader->program, "view"),
		1, GL_FALSE, glm::value_ptr(mView)
	);

	// model
	glm::mat4 mModel(1.0f);
	mModel = glm::translate(mModel, glm::vec3(0.0f, -1.75f, 0.0f));
	mModel = glm::scale(mModel, glm::vec3(0.2f, 0.2f, 0.2f));
	glUniformMatrix4fv(
		glGetUniformLocation(shader->program, "model"),
		1, GL_FALSE, glm::value_ptr(mView)
	);

	//roadModel->Draw(*shader);
	model->Draw(*shader);


	glfwSwapBuffers(window);
}

void cameraCtl() {
	double cameraSpeed = 5.0 * deltaTime;
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(CameraDirection::FORWARD, (float)deltaTime);
	//cameraPos += cameraSpeed * cameraFront;
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(CameraDirection::BACKWARD, (float)deltaTime);
	//cameraPos -= cameraSpeed * cameraFront;
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(CameraDirection::LEFT, (float)deltaTime);
	//cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(CameraDirection::RIGHT, (float)deltaTime);
	//cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

static void key_input(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch (key) {
	case 'E':
		if (action == GLFW_PRESS) {
			if (mods == GLFW_MOD_CONTROL) {
				logger.Debug("Closing wire mode");
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			else {
				logger.Debug("Closing wire mode");
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
		}
		break;
	default:
		if (key >= 0 && key < 1024)
		{
			if (action == GLFW_PRESS)
				keys[key] = true;
			else if (action == GLFW_RELEASE)
				keys[key] = false;
		}
		break;
	}
}

static void mouse_input(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse)
	{
		lastX = (GLfloat)xpos;
		lastY = (GLfloat)ypos;
		firstMouse = false;
	}

	GLfloat xoffset = (GLfloat)xpos - lastX;
	GLfloat yoffset = lastY - (GLfloat)ypos;  // Reversed since y-coordinates go from bottom to left

	lastX = (GLfloat)xpos;
	lastY = (GLfloat)ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

static void scroll_input(GLFWwindow* window, double xpos, double ypos) {
	camera.ProcessMouseScroll((float)ypos);
}

void input() {
	glfwPollEvents();
	glfwSetKeyCallback(window, key_input);
	glfwSetCursorPosCallback(window, mouse_input);
	glfwSetScrollCallback(window, scroll_input);
	cameraCtl();
}

int initProgram() {

	// 啟用多重採樣 (4x MSAA)
	// document: https://learnopengl.com/Advanced-OpenGL/Anti-Aliasing
	glfwWindowHint(GLFW_SAMPLES, 4);

	// 視窗不可調整
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	// 初始化 GLFW
	if (!glfwInit()) { return -1; }
	window = glfwCreateWindow(WIDTH, HEIGHT, WINDOW_TITLE, NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// 初始化 GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		return -1;
	}

	return 0;
}

void initView(int width, int height) {
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_ALWAYS);	// always PASS 
	//glDepthFunc(GL_NOTEQUAL); // PASS if incoming depth value is NOT EQUAL to the stored depth value
	glDepthFunc(GL_LESS);		// default, PASS if incoming depth value is LESS than the stored depth value.
								// that means smaller depth value has higher priority than bigger one. 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

int main(void) {

	int status = initProgram();
	if (status != 0) return status;

	initView(WIDTH, HEIGHT);

	shader = new Shader("./shaders/model.loading.vs", "./shaders/model.loading.frag");
	model = new Model("./models/HololiveEN_3D/OBJ/GawrGura.obj");
	//roadModel = new Model("./objs/house2.obj");

	lastTime = glfwGetTime();
	while (!glfwWindowShouldClose(window)) {
		currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		if (deltaTime >= 1 / 60.) {
			char title[20];
			sprintf_s(title, "FPS: %.2lf", 1.0 / deltaTime);
			glfwSetWindowTitle(window, title);
			draw();
			update();
			input();
			lastTime = currentTime;
		}

	}

	glfwTerminate();
	return 0;
}