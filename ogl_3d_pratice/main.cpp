#include "Game.h"

Game game(
	"OpenGL tutorial",
	1280, 720,
	4, 2,
	false
);

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

	game.CameraProcessMouseMovement(xoffset, yoffset);
}

static void scroll_input(GLFWwindow* window, double xpos, double ypos) {
	game.CameraProcessMouseScroll((float)ypos);
}
#pragma endregion

int main() {
	game.setKeyboardCallback(key_input);
	game.setCameraMovementCallback(CameraMovement);
	game.setMouseCallback(mouse_input);
	game.setScrollCallback(scroll_input);
	while (!game.getWindowShouldClose()) {
		game.update();
		game.render();
	}
	return 0;
}