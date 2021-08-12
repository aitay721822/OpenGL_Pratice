//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include "Logger.h"
//#include "Constants.h"
//#include <iostream>
//#include "shader_loader.h"
//#include "Model.h"
//#include <SOIL/SOIL.H>
//#include "Camera.h"
//
//
//#define WIDTH 1280
//#define HEIGHT 720
//#define WINDOW_TITLE "OpenGL"
//
//
//bool keys[1024];
//
//glm::vec3 lightPos(100.0f, 100.0f, 100.0f);
//double lastTime = 0.0, currentTime = 0.0, deltaTime = 0.0;
//GLuint vbo = 0, vao = 0, ebo = 0,
//	   light_vbo = 0, light_vao = 0, light_ebo = 0;
//bool firstMouse = true;
//GLfloat lastX = 400, lastY = 300;
//
//GLFWwindow* window;
//
//Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
//Shader *shader, *light_shader;
//Model *model, *light_model;
//
//// Logger
//Logger logger("GameLoop", config_debug);
//
//void update() {
//	
//}
//
//void draw() {
//	// set background color
//	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//
//	shader->use();
//	// model
//	//float time = glfwGetTime();
//	glm::mat4 model_matrix = glm::mat4(1.0f);
//	//glm::mat4 model_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(time * 45.0f), glm::vec3(1.0f, 1.0f, 1.0f));
//	GLint model_location = glGetUniformLocation(shader->program, "model");
//	glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model_matrix));
//		
//	// projection
//	GLfloat aspect = (GLfloat)WIDTH / (GLfloat)HEIGHT;
//	glm::mat4 projection_matrix = glm::perspective(camera.Zoom, aspect, 0.1f, 1000.0f);
//	GLint projection_location = glGetUniformLocation(shader->program, "projection");
//	glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection_matrix));
//
//	// view
//	// glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
//	glm::mat4 view = camera.GetViewMatrix();
//	GLint view_location = glGetUniformLocation(shader->program, "view");
//	glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));
//
//	// color
//	GLint objectColorLoc = glGetUniformLocation(shader->program, "objectColor");
//	GLint lightColorLoc = glGetUniformLocation(shader->program, "lightColor");
//	GLint lightPosLoc = glGetUniformLocation(shader->program, "lightPosition");
//	GLint viewPosLoc = glGetUniformLocation(shader->program, "viewPosition");
//	glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
//	glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
//	glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
//	glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);
//
//	glBindVertexArray(vao);
//	glDrawElements(GL_TRIANGLES, model->get_indices().size() * sizeof(unsigned int), GL_UNSIGNED_INT, NULL);
//	glBindVertexArray(0);
//
//	light_shader->use();
//
//	model_location = glGetUniformLocation(light_shader->program, "model");
//	view_location = glGetUniformLocation(light_shader->program, "view");
//	projection_location = glGetUniformLocation(light_shader->program, "projection");
//
//	glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));
//	glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection_matrix));
//	model_matrix = glm::translate(glm::mat4(1.0f), lightPos);
//	model_matrix = glm::scale(model_matrix, glm::vec3(0.2f));
//	glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model_matrix));
//
//	glBindVertexArray(light_vao);
//	glDrawElements(GL_TRIANGLES, light_model->get_indices().size() * sizeof(unsigned int), GL_UNSIGNED_INT, NULL);
//	glBindVertexArray(0);
//
//	glfwSwapBuffers(window);
//}
//
//void cameraCtl() {
//	double cameraSpeed = 5.0 * deltaTime;
//	if (keys[GLFW_KEY_W])
//		camera.ProcessKeyboard(CameraDirection::FORWARD, (float)deltaTime);
//		//cameraPos += cameraSpeed * cameraFront;
//	if (keys[GLFW_KEY_S])
//		camera.ProcessKeyboard(CameraDirection::BACKWARD, (float)deltaTime);
//		//cameraPos -= cameraSpeed * cameraFront;
//	if (keys[GLFW_KEY_A])
//		camera.ProcessKeyboard(CameraDirection::LEFT, (float)deltaTime);
//		//cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
//	if (keys[GLFW_KEY_D])
//		camera.ProcessKeyboard(CameraDirection::RIGHT, (float)deltaTime);
//		//cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
//}
//
//static void key_input(GLFWwindow* window, int key, int scancode, int action, int mods) {
//	switch (key) {
//	case 'E':
//		if (action == GLFW_PRESS) {
//			if (mods == GLFW_MOD_CONTROL) {
//				logger.Debug("Closing wire mode");
//				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//			}
//			else {
//				logger.Debug("Closing wire mode");
//				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//			}
//		}
//		break;
//	default:
//		if (key >= 0 && key < 1024)
//		{
//			if (action == GLFW_PRESS)
//				keys[key] = true;
//			else if (action == GLFW_RELEASE)
//				keys[key] = false;
//		}
//		break;
//	}
//}
//
//static void mouse_input(GLFWwindow* window, double xpos, double ypos) {
//	if (firstMouse)
//	{
//		lastX = (GLfloat)xpos;
//		lastY = (GLfloat)ypos;
//		firstMouse = false;
//	}
//
//	GLfloat xoffset = (GLfloat)xpos - lastX;
//	GLfloat yoffset = lastY - (GLfloat)ypos;  // Reversed since y-coordinates go from bottom to left
//
//	lastX = (GLfloat)xpos;
//	lastY = (GLfloat)ypos;
//
//	camera.ProcessMouseMovement(xoffset, yoffset);
//}
//
//static void scroll_input(GLFWwindow* window, double xpos, double ypos) {
//	camera.ProcessMouseScroll((float)ypos);
//}
//
//void input() {
//	glfwPollEvents();
//	glfwSetKeyCallback(window, key_input);
//	glfwSetCursorPosCallback(window, mouse_input);
//	glfwSetScrollCallback(window, scroll_input);
//	cameraCtl();
//}
//
//int initProgram() {
//
//	// 啟用多重採樣 (4x MSAA)
//	// document: https://learnopengl.com/Advanced-OpenGL/Anti-Aliasing
//	glfwWindowHint(GLFW_SAMPLES, 4);
//
//	// 視窗不可調整
//	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
//
//	// 初始化 GLFW
//	if (!glfwInit()) { return -1; }
//	window = glfwCreateWindow(WIDTH, HEIGHT, WINDOW_TITLE, NULL, NULL);
//	if (!window) {
//		glfwTerminate();
//		return -1;
//	}
//	glfwMakeContextCurrent(window);
//
//	// 初始化 GLEW
//	glewExperimental = GL_TRUE;
//	if (glewInit() != GLEW_OK) {
//		return -1;
//	}
//
//	return 0;
//}
//
//void initView(int width, int height) {
//	glfwGetFramebufferSize(window, &width, &height);
//	glViewport(0, 0, width, height);
//	glEnable(GL_DEPTH_TEST);
//	//glDepthFunc(GL_ALWAYS);	// always PASS 
//	//glDepthFunc(GL_NOTEQUAL); // PASS if incoming depth value is NOT EQUAL to the stored depth value
//	glDepthFunc(GL_LESS);		// default, PASS if incoming depth value is LESS than the stored depth value.
//								// that means smaller depth value has higher priority than bigger one. 
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//}
//
//int main(void) {
//
//	int status = initProgram();
//	if (status != 0) return status;
//
//	initView(WIDTH, HEIGHT);
//
//	light_model = new Model("./objs/VideoShip.obj");
//	model = new Model("./objs/mountains.obj");
//	shader = new Shader("./shaders/vertex.glsl", "./shaders/shadow.glsl");
//	light_shader = new Shader("./shaders/light_vertex.glsl", "./shaders/light_fragment.glsl");
//
//
//	// main object
//	glGenVertexArrays(1, &vao);
//	glGenBuffers(1, &vbo);
//	glGenBuffers(1, &ebo);
//
//	glBindVertexArray(vao);
//	glBindBuffer(GL_ARRAY_BUFFER, vbo);
//	glBufferData(GL_ARRAY_BUFFER, model->get_vertices().size() * sizeof(glm::vec3), &model->get_vertices()[0], GL_STATIC_DRAW);
//
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->get_indices().size() * sizeof(unsigned int), &model->get_indices()[0], GL_STATIC_DRAW);
//
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
//	glEnableVertexAttribArray(0);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindVertexArray(0);
//
//	// light object
//	glGenVertexArrays(1, &light_vao);
//	glGenBuffers(1, &light_vbo);
//	glGenBuffers(1, &light_ebo);
//
//	glBindVertexArray(light_vao);
//	glBindBuffer(GL_ARRAY_BUFFER, light_vbo);
//	glBufferData(GL_ARRAY_BUFFER, light_model->get_vertices().size() * sizeof(glm::vec3), &light_model->get_vertices()[0], GL_STATIC_DRAW);
//	
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, light_ebo);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, light_model->get_indices().size() * sizeof(unsigned int), &light_model->get_indices()[0], GL_STATIC_DRAW);
//
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
//	glEnableVertexAttribArray(0);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindVertexArray(0);
//
//	lastTime = glfwGetTime();
//	while (!glfwWindowShouldClose(window)) {
//		currentTime = glfwGetTime();
//		deltaTime = currentTime - lastTime;
//		if (deltaTime >= 1 / 60.) {
//			char title[20];
//			sprintf_s(title, "FPS: %.2lf", 1.0 / deltaTime);
//			glfwSetWindowTitle(window, title);
//			draw();
//			update();
//			input();
//			lastTime = currentTime;
//		}
//
//	}
//
//	glDeleteVertexArrays(1, &vao);
//	glDeleteBuffers(1, &vbo);
//	glDeleteBuffers(1, &ebo);
//
//	glDeleteVertexArrays(1, &light_vao);
//	glDeleteBuffers(1, &light_vbo);
//	glDeleteBuffers(1, &light_ebo);
//
//	glfwTerminate();
//	return 0;
//}
//
