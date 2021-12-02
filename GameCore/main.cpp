#pragma once

#include "Model.h"
#include "OpenGLRenderer.h"

using namespace GameCore;

Scene* scene = new Scene();
OpenGLRenderer game("OpenGL", 1600, 900);

#pragma region Keyboard_Callback
bool keys[1024];

void CameraMovement(Camera* camera, double dt) {
	double cameraSpeed = 5.0 * dt;
	if (keys[GLFW_KEY_W])
		camera->ProcessKeyboard(CameraDirection::FORWARD, (float)dt);
	if (keys[GLFW_KEY_S])
		camera->ProcessKeyboard(CameraDirection::BACKWARD, (float)dt);
	if (keys[GLFW_KEY_A])
		camera->ProcessKeyboard(CameraDirection::LEFT, (float)dt);
	if (keys[GLFW_KEY_D])
		camera->ProcessKeyboard(CameraDirection::RIGHT, (float)dt);
}

static void key_input(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch (key) {
	case 'E':
		if (action == GLFW_PRESS) {
			if (mods == GLFW_MOD_CONTROL) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			else {
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
#pragma endregion

#pragma region Mouse_Callback
bool firstMouse = true;
float lastX, lastY;
static void mouse_input(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	float xoffset = lastX - (float)xpos;
	float yoffset = lastY - (float)ypos;

	lastX = (float)xpos;
	lastY = (float)ypos;

	game.ProcessMouseMovement(xoffset, yoffset);
}
#pragma endregion

static string update_title() {
	double fps = game.getFPS();
	if (fps > 0.0) {
		char title[20];
		sprintf_s(title, "FPS: %.2lf", fps);
		return string(title);
	}
	return "Oops. title is give you up. but rick astley never gonna give you up";
}

//double current = 0, last = 0, interval = 1;
//void rotateMesh(Object3DNode* node, bool check = false) {
//	current = glfwGetTime();
//	if (current - last < interval && !check) {
//		return;
//	}
//	
//	vector<Object3DNode*> children = node->getChildren();
//	if (node->getMeshes().size() == 0) {
//		for (auto& child : children) {
//			rotateMesh(child, true);
//		}
//	}
//	else {
//		for (auto& mesh : node->getMeshes()) {
//			mesh->rotateY(radians(10.f))->rotateX(radians(10.f));
//		}
//	}
//	last = current;
//}


void loadScene() {
	vector<string> faces = { 
		"./models/Skybox/right.jpg",
		"./models/Skybox/left.jpg",
		"./models/Skybox/top.jpg",
		"./models/Skybox/bottom.jpg",
		"./models/Skybox/back.jpg",
		"./models/Skybox/front.jpg"
	};
	scene->setBackground(new Skybox(faces));

	Model* model;
	// volcano
	model = new Model("./models/Demo/volcano2_v3/volcano2_v3.obj");
	scene->add(model->getNode())->rotateY(radians(270.f));
	// street
	model = new Model("./models/Demo/street_v2/street_v2.obj");
	model->getNode()->translateX(42.f)->rotateY(radians(270.f));
	scene->add(model->getNode());
	// river3
	model = new Model("./models/Demo/river3_v2/river3_v2.obj");
	model->getNode()->translateZ(42.f)->rotateY(radians(270.f));
	scene->add(model->getNode());
	// iceland2
	model = new Model("./models/Demo/ocean_v2/ocean_v2.obj");
	model->getNode()->translateX(42.f)->translateZ(42.f)->rotateY(radians(270.f));
	scene->add(model->getNode());

	DirectionalLight* light = new DirectionalLight();
	light->intensity = 1.f;
	light->setDirection(vec3(-1.f, -1.f, -1.f));
	scene->setLights(light);
}

int main() {
	loadScene();

	game.addShader(ShaderType::ShaderSkybox, "./shaders/skybox.vs", "./shaders/skybox.frag");
	game.addShader(ShaderType::ShaderCore, "./shaders/model.core.vs", "./shaders/model.core.frag");
	game.setKeyboardCallback(key_input);
	game.setCameraMovementCallback(CameraMovement);
	game.setMouseCallback(mouse_input);
	game.setUpdateTitleCallback(update_title);
	while (!game.getWindowShouldClose()) {
		game.update();
		game.render(scene);
		//rotateMesh(scene);
	}
	return 0;
}