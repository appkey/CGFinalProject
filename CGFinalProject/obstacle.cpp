#include "obstacle.h"
#include "model_loader.h"
#include <gl/glm/gtc/matrix_transform.hpp>
#include <iostream>
Obstacle::Obstacle(glm::vec3 pos) {
    Position = pos;
    Scale = glm::vec3(1.0f);
    Rotation = glm::vec3(0.0f);
    Init();
}

Obstacle::~Obstacle() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Obstacle::Init() {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    // �� �ε�
    if (!ModelLoader::LoadModel("obj/sphere.obj", vertices, indices)) {
        std::cout << "Failed to load model. Exiting..." << std::endl;
        exit(1);
    }

    indexCount = indices.size();

    // VAO, VBO, EBO ���� �� ����
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    // EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // ���� �Ӽ� ����
    // ��ġ (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    // ��� (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    // �÷� (location = 2)
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Obstacle::UpdateModelMatrix() {
    ModelMatrix = glm::mat4(1.0f);
    ModelMatrix = glm::translate(ModelMatrix, Position);
    ModelMatrix = glm::rotate(ModelMatrix, glm::radians(Rotation.x), glm::vec3(1, 0, 0));
    ModelMatrix = glm::rotate(ModelMatrix, glm::radians(Rotation.y), glm::vec3(0, 1, 0));
    ModelMatrix = glm::rotate(ModelMatrix, glm::radians(Rotation.z), glm::vec3(0, 0, 1));
    ModelMatrix = glm::scale(ModelMatrix, Scale);
}

void Obstacle::Draw(Shader& shader) {
    UpdateModelMatrix();
    shader.Use();
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, &ModelMatrix[0][0]);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Obstacle::Update(float deltaTime) {
    // ���� �������� �¿�� �����̴� ����
    //Position.x = sin(glutGet(GLUT_ELAPSED_TIME) / 1000.0f) * 2.0f;
}
