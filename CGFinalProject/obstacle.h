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
    glm::vec3 direction;
    
    glm::vec3 StartPostion;
    int update_mode;

    GLuint VAO, VBO, EBO, NBO;
    int indexCount;

    std::vector<unsigned int> vertexIndices, normalIndices;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;



    Obstacle(glm::vec3 pos);
    Obstacle(glm::vec3 pos, int update_mode);
    ~Obstacle();
    void Init();
    void UpdateModelMatrix();
    void Draw(Shader& shader);
    void Update(float deltaTime, int stage);
    void SetColor(const glm::vec3& color );
    void SetScale(const glm::vec3& factor);
};

#endif
