#include "character.h"
#include <gl/glm/gtc/matrix_transform.hpp>
#include <gl/glm/gtc/type_ptr.hpp> // glm::value_ptr 사용을 위해 추가
#include <iostream>
#include <vector>
#include <cmath>

Character::Character() : isInvincible(false) {
    Position = glm::vec3(0.0f, -0.25f, 13.5f);
    Scale = glm::vec3(1.0f, 1.1f, 1.0f);
    Rotation = glm::vec3(0.0f);
    color = glm::vec3(1.0f, 0.0f, 0.0f);
    Init();
}

Character::~Character() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Character::Init() {
    // 큐브의 정점 데이터 (24개의 정점 - 각 면마다 4개씩)
    std::vector<Vertex> vertices = {
        // 앞면 (Z+)
        { {-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f} },
        { { 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f} },
        { { 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f} },
        { {-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f} },

        // 뒷면 (Z-)
        { {-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f} },
        { { 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f} },
        { { 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f} },
        { {-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f} },

        // 왼쪽 면 (X-)
        { {-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f} },
        { {-0.5f, -0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f} },
        { {-0.5f,  0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f} },
        { {-0.5f,  0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f} },

        // 오른쪽 면 (X+)
        { { 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f} },
        { { 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f} },
        { { 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f} },
        { { 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f} },

        // 아래 면 (Y-)
        { {-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f} },
        { { 0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f} },
        { { 0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f} },
        { {-0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f} },

        // 위 면 (Y+)
        { {-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f} },
        { { 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f} },
        { { 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f} },
        { {-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f} }
    };

    // 인덱스 데이터 (각 면마다 두 개의 삼각형)
    std::vector<unsigned int> indices = {
        // 앞면
        0, 1, 2,
        2, 3, 0,

        // 뒷면
        4, 5, 6,
        6, 7, 4,

        // 왼쪽 면
        8, 9,10,
       10,11, 8,

       // 오른쪽 면
      12,13,14,
      14,15,12,

      // 아래 면
     16,17,18,
     18,19,16,

     // 위 면
    20,21,22,
    22,23,20
    };

    indexCount = static_cast<int>(indices.size());

    // VAO, VBO, EBO 생성 및 설정
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // 정점 속성 설정
    // 위치 (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
    glEnableVertexAttribArray(0);

    // 노멀 (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Character::ToggleInvincibility() {
    isInvincible = !isInvincible;
    if (isInvincible) {
        std::cout << "Character is now invincible!" << std::endl;
    }
    else {
        std::cout << "Character is no longer invincible!" << std::endl;
    }
}

void Character::setPosition(const glm::vec3& pos)
{
    Position = pos;
}

void Character::startPos(const int currentStage)
{
    if (currentStage == 1) {
        Position = glm::vec3(0.0f, -0.25f, 13.5f);
    }
    else if (currentStage == 2) {
        Position = glm::vec3(-15.f, -0.25f, 1.f);
    }
    else if (currentStage == 3) {
        // 추가적인 스테이지 3 설정 필요 시 여기에 작성
    }
}

void Character::UpdateModelMatrix() {
    ModelMatrix = glm::mat4(1.0f);
    ModelMatrix = glm::translate(ModelMatrix, Position);
    ModelMatrix = glm::rotate(ModelMatrix, glm::radians(Rotation.x), glm::vec3(1, 0, 0));
    ModelMatrix = glm::rotate(ModelMatrix, glm::radians(Rotation.y), glm::vec3(0, 1, 0));
    ModelMatrix = glm::rotate(ModelMatrix, glm::radians(Rotation.z), glm::vec3(0, 0, 1));
    ModelMatrix = glm::scale(ModelMatrix, Scale);
}

void Character::Draw(Shader& shader) {
    UpdateModelMatrix();
    shader.Use();
    shader.setVec3("emission", glm::vec3(0.4f, 0.0f, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));
    glUniform3f(glGetUniformLocation(shader.Program, "objectColor"), color.x, color.y, color.z); // 빨간색
    
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    shader.setVec3("emission", glm::vec3(0.f, 0.f, 0.f));
}

void Character::Move(float deltaTime, bool* keys, const glm::vec3& minBoundary, const glm::vec3& maxBoundary, const glm::vec3& startMin, const glm::vec3& startMax, const glm::vec3& endMin, const glm::vec3& endMax) {
    float speed = 5.0f;
    glm::vec3 newPosition = Position;

    if (keys['w']) {
        newPosition.z -= speed * deltaTime;
    }
    if (keys['s']) {
        newPosition.z += speed * deltaTime;
    }
    if (keys['a']) {
        newPosition.x -= speed * deltaTime;
    }
    if (keys['d']) {
        newPosition.x += speed * deltaTime;
    }

    Position = newPosition;

    UpdateModelMatrix();
}
