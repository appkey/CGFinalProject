#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <GL/glew.h>

class Shader {
public:
    GLuint Program;
    Shader(const char* vertexPath, const char* fragmentPath);
    void Use();
};

#endif
