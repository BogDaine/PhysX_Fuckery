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
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
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


Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    Init(vertexPath, fragmentPath);
}

Shader::~Shader()
{
    glDeleteProgram(ID);
}

void Shader::Use() const
{
    glUseProgram(ID);
}


void Shader::Bind() const
{
    glUseProgram(ID);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}


unsigned int Shader::GetID() const
{
    return ID;
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value)
{
    Use();
    auto location = GetUniformLocation(name);
    glUniform3fv(location, 1, &value[0]);
}

void Shader::SetVec3(const std::string& name, float x, float y, float z)
{
    Use();
    int location = GetUniformLocation(name);
    glUniform3f(location, x, y, z);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& mat)
{
    Use();
    int location = GetUniformLocation(name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::SetMat3(const std::string& name, const glm::mat3& val)
{
    Bind();
    int location = GetUniformLocation(name);
    GLCall(glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(val)));
}


void Shader::SetFloat(const std::string& name, const float& value)
{
    Use();
    int location = GetUniformLocation(name);
    glUniform1f(location, value);
}

void Shader::SetInt(const std::string& name, const int& value)
{
    Use();
    int location = GetUniformLocation(name);
    glUniform1i(location, value);
}

void Shader::Init(const char* vertexPath, const char* fragmentPath)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. compile shaders
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    CheckCompileErrors(vertex, "VERTEX");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    CheckCompileErrors(fragment, "FRAGMENT");
    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    CheckCompileErrors(ID, "PROGRAM");

    // 3. delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::CheckCompileErrors(unsigned int shader, std::string type)
{
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (uniformLocationCache.find(name) != uniformLocationCache.end())
        return uniformLocationCache[name];

    GLCall(unsigned int location = glGetUniformLocation(ID, name.c_str()));
    //ASSERT(location != -1);
    //if (location == -1)
    //{
    //    std::cout << "[WARNING]:" << " UNIFORM <" << name << "> does not exist" << std::endl;
    //}
    return uniformLocationCache[name] = location;
    //return location;
}
