#ifndef STAGE_H
#define STAGE_H

#include <vector>
#include <GL/glew.h>
#include <gl/glm/glm.hpp>
#include "shader.h"

// Vertex ����ü ����

class Stage {
public:
    int stageNumber;
    GLuint VAO, VBO, EBO;
    int indexCount;

    Stage(int number);
    ~Stage();
    void Init();
    void Draw(Shader& shader);
    void Update(float deltaTime);
};

#endif
