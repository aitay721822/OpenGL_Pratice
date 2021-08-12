#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Logger.h"
#include "Constants.h"

class Shader {
public:
	GLuint program;

    Shader(const char* vertexPath, const char* fragmentPath);

    void Use();
private:

    Logger* logger = new Logger("Shader", config_debug);

    void compile_shaders(const char* shaderSource, GLint mode, GLuint& output_shader);

    void link_shader_program(GLuint& vertexShader, GLuint& fragmentShader, GLuint& output_prog);
};

#endif