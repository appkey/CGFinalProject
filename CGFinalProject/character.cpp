#include "character.h"
#include <gl/glm/gtc/matrix_transform.hpp>
#include <iostream>

Character::Character() {
    Position = glm::vec3(0.0f, 0.0f, 0.0f);
    Scale = glm::vec3(1.0f);
    Rotation = glm::vec3(0.0f);
    Init();
}

Character::~Character() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Character::Init() {
    // ť���� ���� ������ (8���� ����)
    std::vector<Vertex> vertices = {
        // ��ġ                       // ����             // ����
        {{-0.5f, -0.5f, -0.5f},    {0.0f, 0.0f, 0.0f},   {1.0f, 0.0f, 0.0f}}, // ���� 0
        {{ 0.5f, -0.5f, -0.5f},    {0.0f, 0.0f, 0.0f},   {1.0f, 0.0f, 0.0f}}, // ���� 1
        {{ 0.5f,  0.5f, -0.5f},    {0.0f, 0.0f, 0.0f},   {1.0f, 0.0f, 0.0f}}, // ���� 2
        {{-0.5f,  0.5f, -0.5f},    {0.0f, 0.0f, 0.0f},   {1.0f, 0.0f, 0.0f}}, // ���� 3
        {{-0.5f, -0.5f,  0.5f},    {0.0f, 0.0f, 0.0f},   {1.0f, 0.0f, 0.0f}}, // ���� 4
        {{ 0.5f, -0.5f,  0.5f},    {0.0f, 0.0f, 0.0f},   {1.0f, 0.0f, 0.0f}}, // ���� 5
        {{ 0.5f,  0.5f,  0.5f},    {0.0f, 0.0f, 0.0f},   {1.0f, 0.0f, 0.0f}}, // ���� 6
        {{-0.5f,  0.5f,  0.5f},    {0.0f, 0.0f, 0.0f},   {1.5f, 0.0f, 0.0f}}  // ���� 7
    };

    // ť���� �ε��� ������
    std::vector<unsigned int> indices = {
        // �ո�
        4, 5, 6,
        6, 7, 4,
        // �޸�
        0, 1, 2,
        2, 3, 0,
        // ���� ��
        0, 4, 7,
        7, 3, 0,
        // ������ ��
        1, 5, 6,
        6, 2, 1,
        // �Ʒ� ��
        0, 1, 5,
        5, 4, 0,
        // �� ��
        3, 2, 6,
        6, 7, 3
    };

    indexCount = static_cast<int>(indices.size());

    // VAO, VBO, EBO ���� �� ����
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

    // ���� �Ӽ� ����
    // ��ġ (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
    glEnableVertexAttribArray(0);

    // ��� (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
    glEnableVertexAttribArray(1);

    // �÷� (location = 2)
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, color)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
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

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Character::Move(float deltaTime, bool* keys) {
    float speed = 5.0f;
    if (keys['w']) {
        Position.z -= speed * deltaTime;
    }
    if (keys['s']) {
        Position.z += speed * deltaTime;
    }
    if (keys['a']) {
        Position.x -= speed * deltaTime;
    }
    if (keys['d']) {
        Position.x += speed * deltaTime;
    }
}
