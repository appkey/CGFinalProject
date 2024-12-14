#include "coin.h"
#include <GL/glew.h>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <gl/glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <cmath>

Coin::Coin(const glm::vec3& position)
    : position(position), collected(false), rotationAngle(0.0f), vao(0), vbo(0), ebo(0), indexCount(0)
{
    std::cout << "���� ���� " << ": ( x, y, z ): "
        << "( " << position.x << ", "
        << position.y << ", " << position.z << ")" << std::endl;
    InitRenderData();
}

void Coin::Update(float deltaTime) {
    // ������ ȸ����ŵ�ϴ�.
    rotationAngle += deltaTime * 90.0f; // �ʴ� 90�� ȸ��
    if (rotationAngle > 360.0f) {
        rotationAngle -= 360.0f;
    }
}

void Coin::Draw(Shader& shader) {
    if (collected) return; // ������ ������ ��� ���������� ����

    shader.Use();
    shader.setVec3("emission", glm::vec3(0.3f));

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    // ȸ��
    model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    // �������� �����ؼ� ����ó�� ��� ���� ���� �������
    float scaleFactor = 1.f;
    model = glm::scale(model, glm::vec3(scaleFactor));

    
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

    // ���� Ư�� ���� (�ݻ�)
    glm::vec3 coinColor = glm::vec3(1.0f, 0.843f, 0.0f); // �ݻ�
    glUniform3fv(glGetUniformLocation(shader.Program, "objectColor"), 1, glm::value_ptr(coinColor));

    // ������ �������մϴ�.
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    shader.setVec3("emission", glm::vec3(0.0f));
}


void Coin::Draw(Shader& shader, const glm::mat4& view, const glm::mat4& projection) {
    if (collected) return; // ������ ������ ��� ���������� ����

    shader.Use();

    // �� ��Ʈ���� ����
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    // ȸ��
    model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    // �������� �����ؼ� ����ó�� ��� ���� ���� �������
    float scaleFactor = 1.0f;
    model = glm::scale(model, glm::vec3(scaleFactor));

    // ���̴� ������ ����
    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    // ��ü ����� �߱� ���� ����
    glm::vec3 coinColor = glm::vec3(1.0f, 0.843f, 0.0f); // �ݻ�
    glm::vec3 emissionColor = glm::vec3(0.7f, 0.7f, 0.7f); // �߱� ���� (�ݻ� ��)
    shader.setVec3("objectColor", coinColor);
    shader.setVec3("emission", emissionColor);

    // ������ �������մϴ�.
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Coin::SetCollected() {
    collected = true;
}

bool Coin::IsCollected() const {
    return collected;
}

const glm::vec3& Coin::GetPosition() const {
    return position;
}

void Coin::InitRenderData() {
    const int segments = 60; 
    const float radius = 0.5f; 
    const float thickness = 0.1f; 

    std::vector<float> vertices;
    std::vector<unsigned int> indices;


    vertices.push_back(0.0f); 
    vertices.push_back(0.0f); 
    vertices.push_back(0.0f); 
    vertices.push_back(0.0f);
    vertices.push_back(0.0f); 
    vertices.push_back(1.0f); 

    
    for (int i = 0; i < segments; ++i) {
        float angle = glm::radians((360.0f / segments) * i);
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(1.0f);
    }


    for (int i = 0; i < segments; ++i) {
        indices.push_back(0);
        indices.push_back(i + 1);
        indices.push_back((i + 1) % segments + 1);
    }

   
    unsigned int backCenterIndex = vertices.size() / 6;
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(thickness); 
    vertices.push_back(0.0f);
    vertices.push_back(0.0f); 
    vertices.push_back(-1.0f); 

    for (int i = 0; i < segments; ++i) {
        float angle = glm::radians((360.0f / segments) * i);
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(thickness);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(-1.0f);
    }

    
    for (int i = 0; i < segments; ++i) {
        indices.push_back(backCenterIndex);
        indices.push_back((i + 1) % segments + backCenterIndex + 1);
        indices.push_back(i + backCenterIndex + 1);
    }

   
    unsigned int sideStartIndex = vertices.size() / 6;
    for (int i = 0; i < segments; ++i) {
        float angle = glm::radians((360.0f / segments) * i);
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        glm::vec3 normal = glm::normalize(glm::vec3(x, y, 0.0f)); 

   
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(0.0f);
        vertices.push_back(normal.x);
        vertices.push_back(normal.y);
        vertices.push_back(0.0f);

        // �޸� ���� ����
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(thickness);
        vertices.push_back(normal.x);
        vertices.push_back(normal.y);
        vertices.push_back(0.0f);
    }

   
    for (int i = 0; i < segments; ++i) {
        unsigned int currentFront = sideStartIndex + i * 2;
        unsigned int currentBack = currentFront + 1;
        unsigned int nextFront = sideStartIndex + ((i + 1) % segments) * 2;
        unsigned int nextBack = nextFront + 1;

 
        indices.push_back(currentFront);
        indices.push_back(currentBack);
        indices.push_back(nextFront);

        indices.push_back(nextFront);
        indices.push_back(currentBack);
        indices.push_back(nextBack);
    }

  
    indexCount = static_cast<int>(indices.size());

    
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

  
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

 
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}