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
GLfloat lastX, lastY;
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

void loadScene(string modelPath) {
	vector<string> faces = { 
		"./models/Skybox/right.jpg",
		"./models/Skybox/left.jpg",
		"./models/Skybox/top.jpg",
		"./models/Skybox/bottom.jpg",
		"./models/Skybox/back.jpg",
		"./models/Skybox/front.jpg"
	};
	scene->setBackground(new Skybox(faces));

	Model* model = new Model(modelPath);
	scene->add(model->getNode());
}

int main() {
	loadScene("./models/HololiveEN_3D/GawrGura/GawrGura.obj");

	game.setKeyboardCallback(key_input);
	game.setCameraMovementCallback(CameraMovement);
	game.setMouseCallback(mouse_input);
	game.setUpdateTitleCallback(update_title);
	while (!game.getWindowShouldClose()) {
		game.update();
		game.render(scene);
	}
	return 0;
}