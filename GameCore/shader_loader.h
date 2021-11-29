#pragma once

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <gl/glew.h>

#include "Logger.h"

using namespace std;
using namespace glm;

namespace GameCore {
    class Shader {
    public:
        GLuint program;

        Shader(const char* vertexPath, const char* fragmentPath);

        void Use();
        void Unuse();

        void set1i(const std::string& name, int value, bool shaderAutoDisable = true);
        void set1f(const std::string& name, float value, bool shaderAutoDisable = true);
        void setVec2f(const std::string& name, glm::vec2 value, bool shaderAutoDisable = true);
        void setVec3f(const std::string& name, glm::vec3 value, bool shaderAutoDisable = true);
        void setVec4f(const std::string& name, glm::vec4 value, bool shaderAutoDisable = true);
        void setMat3fv(const std::string& name, glm::mat3 value, GLboolean transpose = GL_FALSE, bool shaderAutoDisable = true);
        void setMat4fv(const std::string& name, glm::mat4 value, GLboolean transpose = GL_FALSE, bool shaderAutoDisable = true);

        void showGLSLUniforms();

    private:

        Logger logger = Logger("Shader");

        void compile_shaders(const char* shaderSource, GLint mode, GLuint& output_shader);

        void link_shader_program(GLuint& vertexShader, GLuint& fragmentShader, GLuint& output_prog);

        GLint findUniformLocation(const std::string& name);
    };
}