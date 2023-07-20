#pragma once

#include <string>
#include <unordered_map>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
    Shader(const std::string& vertexFilepath, const std::string& fragmentFilepath);
    ~Shader();

    void bind() const;
    void unbind() const;

    unsigned int getID() const;
    
    void setUniform(const std::string& name, bool value);
    void setUniform(const std::string& name, int value);
    void setUniform(const std::string& name, unsigned int value);
    void setUniform(const std::string& name, float value);
    void setUniform(const std::string& name, double value);
    void setUniform(const std::string& name, glm::vec2 value);
    void setUniform(const std::string& name, glm::vec3 value);
    void setUniform(const std::string& name, glm::vec4 value);
    void setUniform(const std::string& name, glm::mat4 value);

private:
    std::unordered_map<std::string, int> m_uniformLocationCache;
    unsigned int m_programID;

    std::string parseFile(const std::string& filePath);
    unsigned int compile(const std::string& source, GLenum programType);
    unsigned int createProgram(unsigned int vertexProgramID, unsigned int fragmentProgramID);
    int getUniformLocation(const std::string& name);

    static void checkCompileErrors(unsigned int programID, const std::string& type);    
};
