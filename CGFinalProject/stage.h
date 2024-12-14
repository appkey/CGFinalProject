#ifndef STAGE_H
#define STAGE_H

#include <vector>
#include <GL/glew.h>
#include <gl/glm/glm.hpp>
#include "shader.h"

// Vertex 구조체 정의

class Stage {
public:
    int stageNumber;
    GLuint VAO, VBO, EBO;
    int indexCount;

    Stage(int number);
    ~Stage();
    void Init();
    void Draw(Shader& shader);
    void Draw(Shader&, int);
    void Update(float deltaTime);
    const std::vector<std::vector<int>> getTilemap() {
        return tileMap;
    }



private:
    glm::vec3 color;
    glm::vec3 stage1_start_position;
    glm::vec3 stage1_end_position;


    std::vector < std::vector<int>> tileMap;
};

#endif
