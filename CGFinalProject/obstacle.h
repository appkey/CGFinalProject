#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <GL/glew.h>
#include <gl/glm/glm.hpp>
#include "shader.h"
#include <fstream>
#include <sstream>
#include <vector>
#include "model_loader.h"






class Obstacle {
public:
    glm::vec3 Position;
    glm::vec3 Scale;
    glm::vec3 Rotation; // Rotation angles in degrees
    glm::mat4 ModelMatrix;
    glm::vec3 color;

    GLuint VAO, VBO, EBO, NBO;
    int indexCount;

    std::vector<unsigned int> vertexIndices, normalIndices;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;



    Obstacle(glm::vec3 pos);
    ~Obstacle();
    void Init();
    void UpdateModelMatrix();
    void Draw(Shader& shader);
    void Update(float deltaTime);

};

#endif
