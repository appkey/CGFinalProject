#ifndef CHARACTER_H
#define CHARACTER_H

#include <GL/glew.h>
#include <gl/glm/glm.hpp>
#include "shader.h"
#include "model_loader.h"
class Camera;

class Character {
public:
    glm::vec3 Position;
    glm::vec3 Scale;
    glm::vec3 Rotation; // Rotation angles in degrees
    glm::mat4 ModelMatrix;
    glm::vec3 direction;
    bool isInvincible;

    GLuint VAO, VBO, EBO;
    int indexCount;
    glm::vec3 color;

    Character();
    ~Character();
    void Init();
    void UpdateModelMatrix();
    void Draw(Shader& shader);
    void Move(float deltaTime, bool* keys);
    void Move(float deltaTime, bool* keys, Camera& camera);
    void ToggleInvincibility();
    glm::vec3 getPosition() const { return Position; }
    void setPosition(const glm::vec3& pos);
    void startPos(const int currentStage);


};

#endif
