#include "obstacle.h"
#include "model_loader.h"
#include <gl/glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <algorithm>


Obstacle::Obstacle(glm::vec3 pos) {
    Position = pos + glm::vec3(0.0f, 0.0f, 7.0f);
    Scale = glm::vec3(1.0f);
    Rotation = glm::vec3(0.0f);
    color = glm::vec3(0.0f, 0.0f, 1.0f);

    if (Position.x > 0.0f) {
        direction = glm::vec3(-1.0f, 0.0f, 0.0f); 
    }
    else {
        direction = glm::vec3(1.0f, 0.0f, 0.0f);
    }

    std::cout << "obstacle construction" << std::endl;
    Init();
    UpdateModelMatrix();
}

Obstacle::~Obstacle() {

    std::cout << "obstacle destruction" << std::endl;
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Obstacle::Init() {



    const unsigned int latitudeBands = 50;   // 위도 분할 수
    const unsigned int longitudeBands = 50;  // 경도 분할 수
    const float radius = 0.5f;               // 구체의 반지름

    // 정점 데이터 생성
    for (unsigned int latNumber = 0; latNumber <= latitudeBands; latNumber++) {
        float theta = latNumber * glm::pi<float>() / latitudeBands;
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);

        for (unsigned int longNumber = 0; longNumber <= longitudeBands; longNumber++) {
            float phi = longNumber * 2 * glm::pi<float>() / longitudeBands;
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);

            glm::vec3 normal = glm::vec3(cosPhi * sinTheta, cosTheta, sinPhi * sinTheta);
            glm::vec3 position = radius * normal;

            Vertex vertex;
            vertex.position = position;
            vertex.normal = normal;

            vertices.push_back(vertex);
        }
    }

    // 인덱스 데이터 생성
    for (unsigned int latNumber = 0; latNumber < latitudeBands; latNumber++) {
        for (unsigned int longNumber = 0; longNumber < longitudeBands; longNumber++) {
            unsigned int first = (latNumber * (longitudeBands + 1)) + longNumber;
            unsigned int second = first + longitudeBands + 1;

            // 두 개의 삼각형으로 사각형을 구성
            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }

    // OpenGL 버퍼 생성 및 설정
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // VBO에 정점 데이터 전송
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // EBO에 인덱스 데이터 전송
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // 정점 속성 설정
    // 위치 속성 (location = 0)

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    // 노멀 속성 (location = 1)

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
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
    glUniform3f(glGetUniformLocation(shader.Program, "objectColor"), color.x, color.y, color.z); // 빨간색
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

}

void Obstacle::Update(float deltaTime) {
    float speed = 3.5f; // 이동 속도
    float boundary = 7.0f; // 경계 위치

    Position += direction * speed * deltaTime;

    if (Position.x >= boundary) {
        direction.x = -1.0f; 
    }
    else if (Position.x <= -boundary) {
        direction.x = 1.0f; 
    }

    UpdateModelMatrix();
}
