#include "shader_loader.h"

namespace GameCore {
    Shader::Shader(const char* vertexPath, const char* fragmentPath)
    {
        // 1. Retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // ensures ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::badbit);
        try
        {
            // Open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // Read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // Convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure e)
        {
            logger.Err("File not sucessfully read: %s", e.what());
        }
        if (vertexCode.size() == 0 || fragmentCode.size() == 0) {
            logger.Err("File not exists!");
            return;
        }

        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        // 2. Compile shaders & link shader to program
        GLuint vertex, fragment;
        compile_shaders(vShaderCode, GL_VERTEX_SHADER, vertex);
        compile_shaders(fShaderCode, GL_FRAGMENT_SHADER, fragment);
        link_shader_program(vertex, fragment, this->program);
    }

    void Shader::Use() {
        glUseProgram(this->program);
    }

    void Shader::Unuse() {
        glUseProgram(0);
    }

    void Shader::compile_shaders(const char* shaderSource, GLint mode, GLuint& output_shader) {
        output_shader = glCreateShader(mode);
        glShaderSource(output_shader, 1, &shaderSource, NULL);
        glCompileShader(output_shader);
        GLint compileSuccess;
        GLchar compileInfo[512];
        glGetShaderiv(output_shader, GL_COMPILE_STATUS, &compileSuccess);
        if (!compileSuccess) {
            glGetShaderInfoLog(output_shader, 512, NULL, compileInfo);
            logger.Err("Shader compile failed: %s", compileInfo);
        }
        else {
            logger.Info("Shader compile successfully");
        }
    }

    void Shader::link_shader_program(GLuint& vertexShader, GLuint& fragmentShader, GLuint& output_prog) {
        output_prog = glCreateProgram();
        glAttachShader(output_prog, vertexShader);
        glAttachShader(output_prog, fragmentShader);
        glLinkProgram(output_prog);
        GLint linkSuccess;
        GLchar linkInfo[512];
        glGetProgramiv(output_prog, GL_LINK_STATUS, &linkSuccess);
        if (!linkSuccess) {
            glGetProgramInfoLog(output_prog, 512, NULL, linkInfo);
            logger.Info("LINK_PROGRAM_FAILED");
        }
        else {
            logger.Info("COMPILATION_SUCCESSFULLY");
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    GLint Shader::findUniformLocation(const std::string& name) {
        GLint res = glGetUniformLocation(this->program, name.c_str());
        // using showGLSLUniform instead
        // if (res < 0) {
        //     logger->Debug("cannot found '%s' location", name.c_str());
        // }
        return res;
    }


    void Shader::set1i(const std::string& name, int value, bool shaderAutoDisable) {
        if (shaderAutoDisable) this->Use();
        glUniform1i(findUniformLocation(name), value);
        if (shaderAutoDisable) this->Unuse();
    }

    void Shader::set1f(const std::string& name, float value, bool shaderAutoDisable) {
        if (shaderAutoDisable) this->Use();
        glUniform1f(findUniformLocation(name), value);
        if (shaderAutoDisable) this->Unuse();
    }

    void Shader::setVec2f(const std::string& name, glm::vec2 value, bool shaderAutoDisable) {
        if (shaderAutoDisable) this->Use();
        glUniform2fv(findUniformLocation(name), 1, glm::value_ptr(value));
        if (shaderAutoDisable) this->Unuse();
    }

    void Shader::setVec3f(const std::string& name, glm::vec3 value, bool shaderAutoDisable) {
        if (shaderAutoDisable) this->Use();
        glUniform3fv(findUniformLocation(name), 1, glm::value_ptr(value));
        if (shaderAutoDisable) this->Unuse();
    }

    void Shader::setVec4f(const std::string& name, glm::vec4 value, bool shaderAutoDisable) {
        if (shaderAutoDisable) this->Use();
        glUniform4fv(findUniformLocation(name), 1, glm::value_ptr(value));
        if (shaderAutoDisable) this->Unuse();
    }

    void Shader::setMat3fv(const std::string& name, glm::mat3 value, GLboolean transpose, bool shaderAutoDisable) {
        if (shaderAutoDisable) this->Use();
        glUniformMatrix3fv(findUniformLocation(name), 1, transpose, glm::value_ptr(value));
        if (shaderAutoDisable) this->Unuse();
    }

    void Shader::setMat4fv(const std::string& name, glm::mat4 value, GLboolean transpose, bool shaderAutoDisable) {
        if (shaderAutoDisable) this->Use();
        glUniformMatrix4fv(findUniformLocation(name), 1, transpose, glm::value_ptr(value));
        if (shaderAutoDisable) this->Unuse();
    }

    void Shader::showGLSLUniforms() {
        GLint i;
        GLint count;

        GLint size;
        GLenum type;

        const GLsizei bufSize = 32;
        GLchar name[bufSize];
        GLsizei length;

        glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);
        logger.Debug("Active Uniforms: %d", count);

        for (i = 0; i < count; i++)
        {
            glGetActiveUniform(program, (GLuint)i, bufSize, &length, &size, &type, name);

            logger.Debug("Uniform #%d Type: %u Name: %s", i, type, name);
        }
    }
}