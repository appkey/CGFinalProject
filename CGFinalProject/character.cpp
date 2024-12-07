#include "character.h"
#include <gl/glm/gtc/matrix_transform.hpp>
#include <iostream>

Character::Character() : isInvincible(false) {
    Position = glm::vec3(0.0f, 0.0f, 13.5f);
    Scale = glm::vec3(1.0f);
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
    // 큐브의 정점 데이터 (8개의 정점)
    std::vector<Vertex> vertices = {
        // 위치                       // 법선             // 색상
        {{-0.5f, -0.5f, -0.5f},    {0.0f, 0.0f, 0.0f}}, // 정점 0
        {{ 0.5f, -0.5f, -0.5f},    {0.0f, 0.0f, 0.0f}}, // 정점 1
        {{ 0.5f,  0.5f, -0.5f},    {0.0f, 0.0f, 0.0f}}, // 정점 2
        {{-0.5f,  0.5f, -0.5f},    {0.0f, 0.0f, 0.0f}}, // 정점 3
        {{-0.5f, -0.5f,  0.5f},    {0.0f, 0.0f, 0.0f}}, // 정점 4
        {{ 0.5f, -0.5f,  0.5f},    {0.0f, 0.0f, 0.0f}}, // 정점 5
        {{ 0.5f,  0.5f,  0.5f},    {0.0f, 0.0f, 0.0f}}, // 정점 6
        {{-0.5f,  0.5f,  0.5f},    {0.0f, 0.0f, 0.0f}}  // 정점 7
    };

    // 큐브의 인덱스 데이터
    std::vector<unsigned int> indices = {
        // 앞면
        4, 5, 6,
        6, 7, 4,
        // 뒷면
        0, 1, 2,
        2, 3, 0,
        // 왼쪽 면
        0, 4, 7,
        7, 3, 0,
        // 오른쪽 면
        1, 5, 6,
        6, 2, 1,
        // 아래 면
        0, 1, 5,
        5, 4, 0,
        // 위 면
        3, 2, 6,
        6, 7, 3
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
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, &ModelMatrix[0][0]);
    glUniform3f(glGetUniformLocation(shader.Program, "objectColor"), color.x, color.y, color.z); // 빨간색
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
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

    // 경계 체크
    bool insideMainBoundary =
        newPosition.x >= minBoundary.x && newPosition.x <= maxBoundary.x &&
        newPosition.z >= minBoundary.z && newPosition.z <= maxBoundary.z;

    bool insideStartBoundary =
        newPosition.x >= startMin.x && newPosition.x <= startMax.x &&
        newPosition.z >= startMin.z && newPosition.z <= startMax.z;

    bool insideEndBoundary =
        newPosition.x >= endMin.x && newPosition.x <= endMax.x &&
        newPosition.z >= endMin.z && newPosition.z <= endMax.z;

    if (insideMainBoundary || insideStartBoundary || insideEndBoundary) {
        Position = newPosition;
    }
}
