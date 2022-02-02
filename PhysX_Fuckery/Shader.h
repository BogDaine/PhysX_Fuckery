#pragma once
#include "opengl_boilerplate/GLCall.h"

#include "opengl_boilerplate/Maths.h"
#include <fstream>
#include <sstream>
#include <unordered_map>

class Shader
{
public:
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Shader(const char* vertexPath, const char* fragmentPath);

    ~Shader();


    // activate the shader
    // ------------------------------------------------------------------------
    void Use() const;
    void Bind() const;
    void Unbind() const;


    unsigned int GetID() const;

    // MVP
    unsigned int loc_model_matrix;
    unsigned int loc_view_matrix;
    unsigned int loc_projection_matrix;

    // utility uniform functions
    void SetUniform(const std::string &name, const float& value)
    {
        Use();
        int location = GetUniformLocation(name);
        glUniform1f(location, value);
    }

    void SetUniform(const std::string &name, const glm::vec3& value)
    {
        Use();
        auto location = GetUniformLocation(name);
        glUniform3fv(location, 1, &value[0]);
    }

    void SetUniform(const std::string &name, const glm::mat3& value)
    {
        Use();
        int location = GetUniformLocation(name);
        GLCall(glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value)));
    }

    void SetUniform(const std::string &name, const glm::mat4& value)
    {
        Use();
        int location = GetUniformLocation(name);
        GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value)));
    }

    void SetUniform(const std::string &name, const int& value)
    {
        Use();
        int location = GetUniformLocation(name);
        glUniform1i(location, value);
    }

    void SetVec3(const std::string& name, const glm::vec3& value);

    void SetVec3(const std::string& name, float x, float y, float z);

    void SetMat4(const std::string& name, const glm::mat4& mat);

    void SetMat3(const std::string& name, const glm::mat3& mat);

    void SetFloat(const std::string& name, const float& value);

    void SetInt(const std::string& name, const int& value);

    //void SetLight(const Light& val, const std::string& name = "light");

private:
    void Init(const char* vertexPath, const char* fragmentPath);

    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void CheckCompileErrors(unsigned int shader, std::string type);

    int GetUniformLocation(const std::string& name);

private:
    unsigned int ID;
    std::unordered_map<std::string, int> uniformLocationCache;
};
