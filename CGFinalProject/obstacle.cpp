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
    update_mode = 0;
    direction = Position.x > 0.0f ? glm::vec3(-1.0f, 0.0f, 0.0f) : glm::vec3(1.0f, 0.0f, 0.0f);

    std::cout << "obstacle construction" << std::endl;
    Init();
    UpdateModelMatrix();
}

Obstacle::Obstacle(glm::vec3 pos, int update_mode_) {
    Position = pos + glm::vec3(0.0f, 0.0f, 7.0f);
    Scale = glm::vec3(1.0f);
    Rotation = glm::vec3(0.0f);
    color = glm::vec3(0.0f, 0.0f, 1.0f);
    update_mode = update_mode_;
    direction = Position.x > 0.0f ? glm::vec3(-1.0f, 0.0f, 0.0f) : glm::vec3(1.0f, 0.0f, 0.0f);
    StartPostion = Position;
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
    glUniform3f(glGetUniformLocation(shader.Program, "objectColor"), color.x, color.y, color.z);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

}

void Obstacle::Update(float deltaTime, int currentStage) {
    float speed = 3.5f; // 이동 속도
    float rotationSpeed = 45.0f; // 회전 속도 (도/초)
    float sideLength = 5.0f;
    static int state = 0;
    static float elapsedTime = 0.0f;
    static float zOffset = 0.0f;
    static float zOffset2 = 0.0f;
    static bool movingUp = true;
    static bool movingForward = true;

    if (update_mode == 0) {

        if (currentStage == 2) {
            glm::vec3 center = glm::vec3(-1.0f, 0.0f, -1.0f);
            float rotationAngle = glm::radians(rotationSpeed * deltaTime);
            // 회전 매트릭스 생성 (y축 기준 회전)
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
            // 장애물의 위치 회전
            Position = glm::vec3(rotationMatrix * glm::vec4(Position - center, 1.0f)) + center;
        }
        else {
            Position += direction * speed * deltaTime;

            float boundary = 7.0f;
            if (Position.x >= boundary) {
                direction.x = -1.0f;
            }
            else if (Position.x <= -boundary) {
                direction.x = 1.0f;
            }
        }
    }
    else if (update_mode == 1) {
        float xLength = 4.0f; // x축 이동 범위
        float zLength = 8.0f; // z축 이동 범위


        float speed = 3.5f;


        if (state == 0) {
            Position.x -= speed * deltaTime;
            if (Position.x <= StartPostion.x - xLength) {
                state = 1;
            }
        }
        else if (state == 1) {
            Position.z += speed * deltaTime;
            if (Position.z >= StartPostion.z + zLength) {
                state = 2;
            }
        }
        else if (state == 2) {
            Position.x += speed * deltaTime;
            if (Position.x >= StartPostion.x) {
                state = 3;
            }
        }
        else if (state == 3) {
            Position.z -= speed * deltaTime;
            if (Position.z <= StartPostion.z) {
                state = 0;
            }
        }
    }
    else if (update_mode == 2) {
        float angle = glm::radians(rotationSpeed * deltaTime);
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::vec3 center = glm::vec3(-11.0f, 0.0f, 1.0f);
        Position = glm::vec3(rotationMatrix * glm::vec4(Position - center, 1.0f)) + center;
    }
    else  if (update_mode == 5) {
        if (movingUp) {
            zOffset += 1.0f * deltaTime;
            if (zOffset >= 4.0f) movingUp = false;
        }
        else {
            zOffset -= 1.0f * deltaTime;
            if (zOffset <= -2.0f) movingUp = true;
        }
        Position.z = StartPostion.z + zOffset;
    }
    else if (update_mode == 6) {
        if (!movingUp) {
            zOffset += 1.0f * deltaTime;
            if (zOffset >= 4.0f) movingUp = true;
        }
        else {
            zOffset -= 1.0f * deltaTime;
            if (zOffset <= -2.0f) movingUp = false;
        }
        Position.z = StartPostion.z - zOffset;
    }
    else if (update_mode == 7) {
        if (!movingForward) {
            zOffset2 += 0.5f * deltaTime;
            if (zOffset2 >= 4.0f) movingForward = true;
        }
        else {
            zOffset2 -= 0.5f * deltaTime;
            if (zOffset2 <= -2.0f) movingForward = false;
        }
        Position.z = StartPostion.z - zOffset;
    }
    UpdateModelMatrix();
}

void Obstacle::SetColor(const glm::vec3& color_)
{
    color = color_;
}

void Obstacle::SetScale(const glm::vec3& factor)
{
    Scale = factor;
}
