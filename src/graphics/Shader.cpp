#include "Shader.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

Shader::Shader(const std::string& vertexFilepath, const std::string& fragmentFilepath)
{
    std::string vertexSource = parseFile(vertexFilepath);
    std::string fragmentSource = parseFile(fragmentFilepath);

    unsigned int vertexProgramID = compile(vertexSource, GL_VERTEX_SHADER);
    unsigned int fragmentProgramID = compile(fragmentSource, GL_FRAGMENT_SHADER);

    m_programID = createProgram(vertexProgramID, fragmentProgramID);

    glDeleteShader(vertexProgramID);
    glDeleteShader(fragmentProgramID);
}

Shader::~Shader()
{
    glDeleteProgram(m_programID);
}

void Shader::bind() const
{
    glUseProgram(m_programID);
}

void Shader::unbind() const
{
    glUseProgram(0);
}

unsigned int Shader::getID() const
{
    return m_programID;
}

void Shader::setUniform(const std::string& name, bool value)
{
    glUniform1i(getUniformLocation(name), value);
}

void Shader::setUniform(const std::string& name, int value)
{
    glUniform1i(getUniformLocation(name), value);
}

void Shader::setUniform(const std::string& name, unsigned int value)
{
    glUniform1ui(getUniformLocation(name), value);
}

void Shader::setUniform(const std::string& name, float value)
{
    glUniform1f(getUniformLocation(name), value);
}

void Shader::setUniform(const std::string& name, double value)
{
    glUniform1d(getUniformLocation(name), value);
}

void Shader::setUniform(const std::string& name, glm::vec2 value)
{
    glUniform2fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::setUniform(const std::string& name, glm::vec3 value)
{
    glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::setUniform(const std::string& name, glm::vec4 value)
{
    glUniform4fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::setUniform(const std::string& name, glm::mat4 value)
{
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

std::string Shader::parseFile(const std::string& filePath)
{
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        file.open(filePath);
        std::stringstream fileStream;

        fileStream << file.rdbuf();
        file.close();

        return fileStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::GRAPHICS::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    return "";

}

unsigned int Shader::compile(const std::string& source, GLenum programType)
{
    unsigned int programID = glCreateShader(programType);
    const char* src = source.c_str();

    glShaderSource(programID, 1, &src, NULL);
    glCompileShader(programID);

    std::string type = [programType]()
    {
        switch (programType)
        {
            case GL_VERTEX_SHADER: return "VERTEX";
            case GL_FRAGMENT_SHADER: return "FRAGMENT";
            default: return "UNKNOWN TYPE";
        }
    }();

    checkCompileErrors(programID, type);

    return programID;
}

unsigned int Shader::createProgram(unsigned int vertexProgramID, unsigned int fragmentProgramID)
{
    unsigned int programID = glCreateProgram();

    glAttachShader(programID, vertexProgramID);
    glAttachShader(programID, fragmentProgramID);
    
    glLinkProgram(programID);

    checkCompileErrors(programID, "PROGRAM");

    return programID;
}

int Shader::getUniformLocation(const std::string& name)
{   
    if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
        return m_uniformLocationCache[name];

    int location = glGetUniformLocation(m_programID, name.c_str());
    if (location == -1)
        std::cout << "WARN::GRAPHICS::SHADER::UNIFORM_DOESNT_EXIST::" << name << std::endl;
    else
        m_uniformLocationCache[name] = location;

    return location;
}

void Shader::checkCompileErrors(unsigned int programID, const std::string& type)
{
    int success;
    int maxLength = 0;
    std::vector<char> infoLog;

    if (type == "PROGRAM")
    {
        glGetProgramiv(programID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetShaderiv(programID, GL_INFO_LOG_LENGTH, &maxLength);
            infoLog.resize(maxLength);
            glGetShaderInfoLog(programID, maxLength, &maxLength, infoLog.data());
            std::cout << "ERROR::GRAPHICS::SHADER::" << type << "::LINK_FAILED\n";
            for (const auto& ch : infoLog)
                std::cout << ch;
            std::cout << std::endl;
        }
    }
    else
    {
        glGetShaderiv(programID, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderiv(programID, GL_INFO_LOG_LENGTH, &maxLength);
            infoLog.resize(maxLength);
            glGetShaderInfoLog(programID, maxLength, &maxLength, infoLog.data());
            std::cout << "ERROR::GRAPHICS::SHADER::" << type << "::COMPILATION_FAILED\n";
            for (const auto& ch : infoLog)
                std::cout << ch;
            std::cout << std::endl;
        }
    }
}
