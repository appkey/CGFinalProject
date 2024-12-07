#ifndef CHARACTER_H
#define CHARACTER_H

#include <GL/glew.h>
#include <gl/glm/glm.hpp>
#include "shader.h"
#include "model_loader.h"



class Character {
public:
    glm::vec3 Position;
    glm::vec3 Scale;
    glm::vec3 Rotation; // Rotation angles in degrees
    glm::mat4 ModelMatrix;

    bool isInvincible;

    GLuint VAO, VBO, EBO;
    int indexCount;
    glm::vec3 color;

    Character();
    ~Character();
    void Init();
    void UpdateModelMatrix();
    void Draw(Shader& shader);
    void Move(float deltaTime, bool* keys, const glm::vec3& minBoundary, const glm::vec3& maxBoundary,
        const glm::vec3& startMin, const glm::vec3& startMax,
        const glm::vec3& endMin, const glm::vec3& endMax);
    void ToggleInvincibility();

};

#endif
