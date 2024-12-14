#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <GL/glew.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/gtc/type_ptr.hpp> 

class Shader {
public:
    GLuint Program;
    Shader(const char* vertexPath, const char* fragmentPath);
    void Use();

    // Uniform 설정 메서드
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;
};

#endif
