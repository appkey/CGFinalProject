#include "stage.h"
#include "model_loader.h"
#include <gl/glm/glm.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <gl/glm/gtc/type_ptr.hpp> // glm::value_ptr을 사용하기 위해 추가

Stage::Stage(int number) {
    stageNumber = number;
    Init();
    color = glm::vec3{ 0.7f, 0.7f, 0.7f };
    stage1_start_position = glm::vec3(0.0f, -1.0f, 13.0f);
    stage1_end_position = glm::vec3(0.0f, -1.0f, -33.0f);

    if (stageNumber == 2) {
        tileMap = {
            {4, 4, 4, 4, 3, 2, 3, 2, 3, 2, 3,2, 3, 2, 3, 2},
            {4, 4, 4, 4, 2, 3, 2, 3, 2, 3, 2,3, 4, 4, 2, 3},
            {4, 4, 4, 4, 3, 2, 3, 2, 3, 2, 3,2, 4, 4, 1, 1},
            {4, 4, 4, 4, 2, 3, 2, 3, 2, 3, 2,3, 4, 4, 1, 1},
            {1, 1, 4, 4, 3, 2, 3, 2, 3, 2, 3,2, 4, 4, 4, 4},
            {1, 1, 4, 4, 2, 3, 2, 3, 2, 3, 2,3, 4, 4, 4, 4},
            {3, 2, 4, 4, 3, 2, 3, 2, 3, 2, 3,2, 4, 4, 4, 4},
            {2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2,3, 4, 4, 4, 4}
        };
    }
    else if (stageNumber == 3) {
        tileMap = {
            {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4},
            {4,3,2,3,2,3,2,3,2,3,4,3,2,3,2,3,2,3,2,3,4,1,1,2,3,2,4},
            {4,2,3,2,3,2,3,2,3,2,4,2,3,2,3,2,3,2,3,2,4,1,1,3,2,3,4},
            {4,3,2,3,2,4,2,3,2,3,4,3,2,3,2,4,2,3,2,3,4,4,4,4,3,2,4},
            {4,2,3,2,3,4,3,2,3,2,4,2,3,2,3,4,3,2,3,2,4,3,2,3,2,3,4},
            {4,3,2,3,2,4,2,3,2,3,4,3,2,3,2,4,2,3,2,3,4,2,3,2,3,2,4},
            {4,2,3,2,3,4,3,2,3,2,4,2,3,2,3,4,3,2,3,2,4,3,2,4,4,4,4},
            {4,3,2,3,2,4,2,3,2,3,4,3,2,3,2,4,2,3,2,3,4,2,3,2,3,2,4},
            {4,2,3,2,3,4,3,2,3,2,4,2,3,2,3,4,3,2,3,2,4,3,2,3,2,3,4},
            {4,3,2,3,2,4,2,3,2,3,4,3,2,3,2,4,2,3,2,3,4,4,4,4,3,2,4},
            {4,2,3,2,3,4,3,2,3,2,4,2,3,2,3,4,3,2,3,2,4,3,2,3,2,3,4},
            {4,3,2,3,2,4,2,3,2,3,4,3,2,3,2,4,2,3,2,3,4,2,3,2,3,2,4},
            {4,2,3,2,3,4,3,2,3,2,4,2,3,2,3,4,3,2,3,2,4,3,2,3,2,3,4},
            {4,3,2,3,2,4,2,3,2,3,4,3,2,3,2,4,2,3,2,3,4,2,3,4,4,4,4},
            {4,2,3,2,3,4,3,2,3,2,4,2,3,2,3,4,3,2,3,2,4,3,2,3,2,3,4},
            {4,3,2,3,2,4,2,3,2,3,4,3,2,3,2,4,2,3,2,3,4,2,3,2,3,2,4},
            {4,2,3,2,3,4,3,2,3,2,4,2,3,2,3,4,3,2,3,2,4,3,2,3,2,3,4},
            {4,3,2,3,2,4,2,3,2,3,4,3,2,3,2,4,2,3,2,3,4,2,3,2,3,2,4},
            {4,2,3,2,3,4,3,2,3,2,4,2,3,2,3,4,3,2,3,2,4,4,4,4,2,3,4},
            {4,1,1,1,1,4,2,3,2,3,2,3,2,3,2,4,2,3,2,3,3,2,3,2,3,2,4},
            {4,1,1,1,1,4,3,2,3,2,3,2,3,2,3,4,3,2,3,2,2,3,2,3,2,2,4},
            { 4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4 }
        };
    }
}

Stage::~Stage() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Stage::Init() {
    // 큐브의 정점 데이터 (8개의 정점)
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

void Stage::Draw(Shader& shader, int stageNumber) {
    shader.Use();
    if (stageNumber == 1) {
        // 메인 플랫폼
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.f, -10.0f));
        model = glm::scale(model, glm::vec3(15.0f, 0.5f, 40.0f));

        shader.Use();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform3f(glGetUniformLocation(shader.Program, "objectColor"), color.x, color.y, color.z);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

        // 시작 지점 표시
        glm::mat4 start_model = glm::mat4(1.0f);
        start_model = glm::translate(start_model, stage1_start_position);
        start_model = glm::scale(start_model, glm::vec3(5.0f, 0.5f, 5.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(start_model));
        glUniform3f(glGetUniformLocation(shader.Program, "objectColor"), 0.0f, 1.0f, 0.0f);

        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

        // 종료 지점 표시
        glm::mat4 end_model = glm::mat4(1.0f);
        end_model = glm::translate(end_model, stage1_end_position);
        end_model = glm::scale(end_model, glm::vec3(5.0f, 0.5f, 5.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(end_model));
        glUniform3f(glGetUniformLocation(shader.Program, "objectColor"), 0.0f, 1.0f, 0.0f);

        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
    }
    else if (stageNumber == 2) {
        glm::mat4 model;
        float tileSize = 2.0f; // 타일 한 변의 크기
        glm::vec3 greenTileColor = glm::vec3(1.0f, 0.0f, 1.0f); // 밝은 청록색
        glm::vec3 lightTileColor = glm::vec3(0.7f, 0.5f, 0.3f); // 밝은 베이지색
            glm::vec3 darkTileColor = glm::vec3(0.3f, 1.0f, 0.3f);
        int mapHeight = tileMap.size();     // 세로 타일 개수
        int mapWidth = tileMap[0].size();  // 가로 타일 개수

        shader.Use();

        for (int z = 0; z < mapHeight; ++z) {
            for (int x = 0; x < mapWidth; ++x) {
                int tileType = tileMap[z][x];

                // 렌더링하지 않는 타일은 건너뛰기
                if (tileType == 4) {
                    continue;
                }

                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(
                    x * tileSize - (mapWidth / 2) * tileSize,
                    -1.0f,
                    z * tileSize - (mapHeight / 2) * tileSize
                ));
                model = glm::scale(model, glm::vec3(tileSize, 0.1f, tileSize));

                // 타일 색상 설정
                glm::vec3 currentColor;
                if (tileType == 1) {
                    currentColor = greenTileColor;
                }
                else if (tileType == 2) {
                    currentColor = lightTileColor;
                }
                else if (tileType == 3) {
                    currentColor = darkTileColor;
                }

                glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                glUniform3f(glGetUniformLocation(shader.Program, "objectColor"), currentColor.x, currentColor.y, currentColor.z);

                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
            }
        }

        glBindVertexArray(0);
    }
    else if (stageNumber == 3) {
        
        glm::mat4 model;
        float tileSize = 2.0f; // 타일 한 변의 크기
        glm::vec3 greenTileColor = glm::vec3(0.0f, 0.6f, 0.4f); // 밝은 청록색
        glm::vec3 lightTileColor = glm::vec3(0.8f, 0.8f, 0.8f); // 밝은 베이지색
        glm::vec3 darkTileColor = glm::vec3(0.8f, 0.8f, 0.8f);

        int mapHeight = tileMap.size();     // 세로 타일 개수
        int mapWidth = tileMap[0].size();  // 가로 타일 개수

        shader.Use();

        for (int z = 0; z < mapHeight; ++z) {
            for (int x = 0; x < mapWidth; ++x) {
                int tileType = tileMap[z][x];

                // 렌더링하지 않는 타일은 건너뛰기
                if (tileType == 4) {
                    continue;
                }

                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(
                    x * tileSize - (mapWidth / 2) * tileSize,
                    -1.0f,
                    z * tileSize - (mapHeight / 2) * tileSize
                ));
                model = glm::scale(model, glm::vec3(tileSize, 0.1f, tileSize));

                // 타일 색상 설정
                glm::vec3 currentColor;
                if (tileType == 1) {
                    currentColor = greenTileColor;
                }
                else if (tileType == 2) {
                    currentColor = lightTileColor;
                }
                else if (tileType == 3) {
                    currentColor = darkTileColor;
                }

                glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                glUniform3f(glGetUniformLocation(shader.Program, "objectColor"), currentColor.x, currentColor.y, currentColor.z);

                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
            }
        }
      
        glBindVertexArray(0);

    }
}

void Stage::Update(float deltaTime) {
    // 필요하다면 스테이지 업데이트 로직 추가
}
