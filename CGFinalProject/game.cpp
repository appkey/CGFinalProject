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
    stage = new Stage(currentStage);
    for (int i = 0; i < 10; ++i) {
        obstacles.push_back(new Obstacle(glm::vec3(-4.0f, 0.5f, -4.f * i )));
    }
    for (int i = 0; i < 10; ++i) {
        obstacles.push_back(new Obstacle(glm::vec3(+4.0f, 0.5f,-4.f * i+1.f)));
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
    character->Move(deltaTime, keys);
    stage->Update(deltaTime);
    for (auto& obs : obstacles) {
        obs->Update(deltaTime);
    }
    
    for (auto & obstacle : obstacles) {
        if (CheckCollisionAABBAndSphere(*character, *obstacle)) {
            std::cout << "Collision detected!" << std::endl;

            // 충돌 처리 (예: 색상 변경)
            character->Position = glm::vec3(0.0f, 0.0f, 13.5f);
        }
    }



   camera->Position = character->Position + glm::vec3(0.0f, 3.0f, 7.0f);
    camera->Target = character->Position;
}

void Game::Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
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

    stage->Draw(*shader);
    character->Draw(*shader);
    for (auto& obs : obstacles) {
        obs->Draw(*shader);
    }

    glutSwapBuffers();
}

void Game::SwitchCameraMode() {
    camera->SwitchMode();
}

void Game::NextStage() {
    currentStage++;
    delete stage;
    stage = new Stage(currentStage);
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
    if (key == 'c') {
        instance->SwitchCameraMode();
    }
    else if (key == 'n') {
        instance->NextStage();
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