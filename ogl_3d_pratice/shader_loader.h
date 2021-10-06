#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iostream>
#include "Logger.h"
#include "Constants.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
	GLuint program;

    Shader(const char* vertexPath, const char* fragmentPath);
    
    void Use();
    void Unuse();

    void set1i(const std::string& name, int value);
    void set1f(const std::string& name, float value);
    void setVec2f(const std::string& name, glm::vec2 value);
    void setVec3f(const std::string& name, glm::vec3 value);
    void setVec4f(const std::string& name, glm::vec4 value);
    void setMat3fv(const std::string& name, glm::mat3 value, GLboolean transpose = GL_FALSE);
    void setMat4fv(const std::string& name, glm::mat4 value, GLboolean transpose = GL_FALSE);

private:

    Logger* logger = new Logger("Shader", config_debug);

    void compile_shaders(const char* shaderSource, GLint mode, GLuint& output_shader);

    void link_shader_program(GLuint& vertexShader, GLuint& fragmentShader, GLuint& output_prog);
};