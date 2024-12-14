#include "game.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <cstring> // memset을 위한 헤더
#include <iostream>

Game* Game::instance = nullptr;

Game::Game() {
    currentStage = 1;
    stage = nullptr;
    character = new Character();

    camera = new Camera();
    shader = new Shader("vertex_shader.glsl", "fragment_shader.glsl");
    deltaTime = 0.0f;
    lastFrame = 0.0f;
    memset(keys, 0, sizeof(keys));
    instance = this;
}

Game::~Game() {
    delete stage;
    delete character;
    delete camera;
    delete shader;
}

void Game::Init() {
    if (stage != nullptr) delete stage;
    if (character != nullptr) delete character;

    stage = new Stage(currentStage);
    character = new Character();

    obstacles.clear();

    if (currentStage == 1) {
        // 스테이지 1의 장애물 설정
        for (int i = 0; i < 10; ++i) {
            obstacles.push_back(new Obstacle(glm::vec3(-4.0f, 0.0f, -4.f * i)));
        }
        for (int i = 0; i < 10; ++i) {
            obstacles.push_back(new Obstacle(glm::vec3(+4.0f, 0.0f, -4.f * i + 1.f)));
        }
    }
    else if (currentStage == 2) {
        character->startPos(2);
        // 스테이지 2의 초기화 로직 추가

        // 중앙 장애물
        obstacles.push_back(new Obstacle(glm::vec3(-1.0f, 0.0f, -8.0f)));

        // 세로 방향 장애물
        for (int i = 1; i <= 4; ++i) {
            obstacles.push_back(new Obstacle(glm::vec3(-1.0f, 0.0f, -8.0f -2.0f * i)));  // 위쪽
            obstacles.push_back(new Obstacle(glm::vec3(-1.0f, 0.0f, - 8.0f + 2.0f * i)));   // 아래쪽
        }

        // 가로 방향 장애물
        for (int i = 1; i <= 4; ++i) {
            obstacles.push_back(new Obstacle(glm::vec3(-1.0f -2.0f * i, 0.0f, -8.0f)));  // 왼쪽
            obstacles.push_back(new Obstacle(glm::vec3(-1.0f + 2.0f * i, 0.0f, -8.0f)));   // 오른쪽
        }
    }

}

void Game::Run() {

    glEnable(GL_DEPTH_TEST);

    Init();

    // 콜백 함수 설정
    glutDisplayFunc(DisplayCallback);
    glutIdleFunc(DisplayCallback);
    glutKeyboardFunc(KeyboardDownCallback);
    glutKeyboardUpFunc(KeyboardUpCallback);
    glutReshapeFunc(ReshapeCallback);

    glutMainLoop();
}

void Game::Update(float deltaTime) {
    glm::vec3 minBoundary(-7.0f, -1.0f, -29.0f);
    glm::vec3 maxBoundary(7.0f, 1.0f, 10.0f);

    glm::vec3 startMin(-2.0f, -1.0f, 10.0f);
    glm::vec3 startMax(2.0f, 1.0f, 15.0f);

    glm::vec3 endMin(-2.0f, -1.0f, -34.0f);
    glm::vec3 endMax(2.0f, 1.0f, -29.0f);

    character->Move(deltaTime, keys, minBoundary, maxBoundary, startMin, startMax, endMin, endMax);
    stage->Update(deltaTime);
    for (auto& obstacle : obstacles) {
        obstacle->Update(deltaTime, currentStage);

        if (CheckCollisionAABBAndSphere(*character, *obstacle)) {
            if (!character->isInvincible) {
                std::cout << "Collision detected! Character is not invincible." << std::endl;
                // 캐릭터가 충돌했을 때 초기 위치로 이동
                character->startPos(currentStage);
            }
            else {
                std::cout << "Collision detected, but character is invincible." << std::endl;
            }
        }
    }
    camera->update(*character);

}

void Game::Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.5f, 0.5f, 0.5f, 0.5f);

    glViewport(0, 0, 1600, 900);
    shader->Use();
    // 뷰 및 프로젝션 행렬 설정
    glm::mat4 view = camera->GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1600.0f / 900.0f, 0.1f, 100.0f);

    glUniformMatrix4fv(glGetUniformLocation(shader->Program, "view"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader->Program, "projection"), 1, GL_FALSE, &projection[0][0]);

    // 조명 설정
    glm::vec3 lightPos = glm::vec3(0.0f, 3.0f, -5.0f);
    glUniform3fv(glGetUniformLocation(shader->Program, "lightPos"), 1, &lightPos[0]);
    glUniform3fv(glGetUniformLocation(shader->Program, "viewPos"), 1, &camera->Position[0]);

    stage->Draw(*shader, currentStage);
    character->Draw(*shader);
    for (auto& obs : obstacles) {
        obs->Draw(*shader);
    }
    glViewport(1200, 700, 400, 200);
    shader->Use();

    // 원근 투영 행렬 설정
    shader->Use();

    // 직교 투영 행렬 설정
    float orthoWidth = 30.0f; // 맵 가로 크기
    float orthoHeight = 20.0f; // 맵 세로 크기
    view = glm::lookAt(glm::vec3(character->getPosition().x, 30.0f, character->getPosition().z), glm::vec3(glm::vec3(character->getPosition().x, 0, character->getPosition().z)), glm::vec3(0.0f, 0.0f, -1.0f));
    projection = glm::ortho(-orthoWidth / 2.0f, orthoWidth / 2.0f, -orthoHeight / 2.0f, orthoHeight / 2.0f, 0.1f, 100.0f);

    glUniformMatrix4fv(glGetUniformLocation(shader->Program, "view"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader->Program, "projection"), 1, GL_FALSE, &projection[0][0]);

    // 미니맵 렌더링
    stage->Draw(*shader, currentStage);
    character->Draw(*shader);
    for (auto& obs : obstacles) {
        obs->Draw(*shader);
    }

    glViewport(1200, 450, 400, 200); // 미니맵 아래 영역
    view = glm::lookAt(glm::vec3(0.0f, 0.0f, 30.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    projection = glm::ortho(-15.0f, 15.0f, -10.0f, 10.0f, 0.1f, 100.0f);

    glUniformMatrix4fv(glGetUniformLocation(shader->Program, "view"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader->Program, "projection"), 1, GL_FALSE, &projection[0][0]);

    // 평면도 그리기
    stage->Draw(*shader, currentStage);
    character->Draw(*shader);
    for (auto& obs : obstacles) {
        obs->Draw(*shader);
    }


    glutSwapBuffers();
}

void Game::SwitchCameraMode() {
    camera->SwitchMode();
}

void Game::MoveStage(int stageNum) {
    currentStage = stageNum;
    Init(); // 삭제와 초기화를 Init()에서 처리
}


void Game::DisplayCallback() {
    float currentFrame = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    instance->deltaTime = currentFrame - instance->lastFrame;
    instance->lastFrame = currentFrame;

    instance->Update(instance->deltaTime);
    instance->Render();
}

void Game::KeyboardDownCallback(unsigned char key, int x, int y) {
    instance->keys[key] = true;

    if (key == 'i') {
        instance->character->ToggleInvincibility();
    }
    else if (key == 'c') {
        instance->SwitchCameraMode();
    }
    else if (key == '2') {
        instance->MoveStage(2);
    }
    else if (key == 27) { // ESC 키
        exit(0);
    }
}

void Game::KeyboardUpCallback(unsigned char key, int x, int y) {
    instance->keys[key] = false;
}

void Game::ReshapeCallback(int width, int height) {
    glViewport(0, 0, width, height);
}

bool Game::CheckCollisionAABBAndSphere(const Character& character, const Obstacle& obstacle) {
    // 큐브(AABB)의 중심 및 반쪽 크기
    glm::vec3 aabbMin = character.Position - character.Scale * 0.5f;
    glm::vec3 aabbMax = character.Position + character.Scale * 0.5f;

    // 구의 중심 및 반지름
    glm::vec3 sphereCenter = obstacle.Position;
    float sphereRadius = 0.5f; // 구의 반지름 (필요에 따라 조정)

    // AABB의 각 축에서 구의 중심을 투영
    glm::vec3 closestPoint = glm::clamp(sphereCenter, aabbMin, aabbMax);

    // 투영된 점과 구의 중심 간 거리 계산
    float distance = glm::length(closestPoint - sphereCenter);

    // 충돌 여부 확인
    return distance <= sphereRadius;

}
