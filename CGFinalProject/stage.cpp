#include "stage.h"
#include "model_loader.h"
#include <gl/glm/glm.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>

Stage::Stage(int number) {
    stageNumber = number;
    Init();
    color = glm::vec3(0.7f, 0.7f, 0.7f);
}

Stage::~Stage() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Stage::Init() {
    // ť���� ���� ������ (8���� ����)
    std::vector<Vertex> vertices = {
        // ��ġ                       // ����             // ����
        {{-0.5f, -0.5f, -0.5f},    {0.0f, 0.0f, -1.0f}},
        {{ 0.5f, -0.5f, -0.5f},    {0.0f, 0.0f, -1.0f}},
        {{ 0.5f,  0.5f, -0.5f},    {0.0f, 0.0f, -1.0f}},
        {{-0.5f,  0.5f, -0.5f},    {0.0f, 0.0f, -1.0f}},
        {{-0.5f, -0.5f,  0.5f},    {0.0f, 0.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.5f},    {0.0f, 0.0f, 1.0f}},
        {{ 0.5f,  0.5f,  0.5f},    {0.0f, 0.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.5f},    {0.0f, 0.0f, 1.0f} }
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


    glBindVertexArray(0);
}

void Stage::Draw(Shader& shader) {
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(0.0f, -1.f, -10.0f)); 
    model = glm::scale(model, glm::vec3(15.0f, 0.5f, 40.0f)); 


    shader.Use();
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, &model[0][0]);
    glUniform3f(glGetUniformLocation(shader.Program, "objectColor"), color.x, color.y, color.z); //

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Stage::Update(float deltaTime) {
    // �ʿ��ϴٸ� �������� ������Ʈ ���� �߰�
}