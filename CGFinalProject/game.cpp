#include "game.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <gl/glm/gtc/type_ptr.hpp>
#include <cstring> // memset을 위한 헤더
#include <iostream>

Game* Game::instance = nullptr;

Game::Game() {
    currentStage = 1;
    stage = nullptr;
    character = new Character();
    camera = new Camera();
    shader = new Shader("vertex_shader.glsl", "fragment_shader.glsl");
    coinShader = new Shader("coin_vertex_shader.glsl", "coin_fragment_shader.glsl");
    lightOn = false;
    showNormals = false;
    deltaTime = 0.0f;
    lastFrame = 0.0f;
    memset(keys, 0, sizeof(keys));
    instance = this;
    wireframe = false;
    std::vector<std::string> faces
    {
        "res/skybox/1-1.jpg",
        "res/skybox/1-2.jpg",
        "res/skybox/1-3.jpg",
        "res/skybox/1-4.jpg",
        "res/skybox/1-5.jpg",
        "res/skybox/1-6.jpg"
    };
    skybox = new Skybox(faces);


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
    if (skybox != nullptr) delete skybox;
    stage = new Stage(currentStage);
    character = new Character();

    CenterMouse();

    if (currentStage == 1) {
        goalAreaMin = glm::vec3(-2.f, -0.5f, -34.0f); // 예시 최소 좌표
        goalAreaMax = glm::vec3(2.0f, 0.5f, -31.0f);  // 예시 최대 좌표
    }
    else if (currentStage == 2) {
        goalAreaMin = glm::vec3(11.0f, -0.5f, -5.0f); // 예시 최소 좌표
        goalAreaMax = glm::vec3(15.0f, 0.5f, -3.0f);   // 예시 최대 좌표

     

    }
    else if (currentStage == 3) {
        goalAreaMin = glm::vec3(14.5f, -0.5f, -20.0f); // 예시 최소 좌표
        goalAreaMax = glm::vec3(19.0f, 0.5f, -18.0f);  // 예시 최대 좌표
    }



    coins.clear();
    obstacles.clear();
    stage1Boundary.clear();
    stage2Boundary.clear();
    stage3Boundary.clear();
    if (currentStage == 1) {
        std::vector<std::string> faces
        {
            "res/skybox/1-1.jpg",
            "res/skybox/1-2.jpg",
            "res/skybox/1-3.jpg",
            "res/skybox/1-4.jpg",
            "res/skybox/1-5.jpg",
            "res/skybox/1-6.jpg"
        };
        skybox = new Skybox(faces);

        // 스테이지 1의 장애물 설정
        float boundaryX = 8.0f;  // 스테이지 X 경계
        float boundaryZ = 30.0f;  // 스테이지 Z 경계
        float spacing = 2.0f;     // 장애물 간 간격
        float smallSquareOffset = 3.0f;
        float smallSquareSize = 2.0f;

        coins.push_back(new Coin(glm::vec3(-6.0f, 0.0f, 9.5f)));
        coins.push_back(new Coin(glm::vec3(6.0f, 0.0f, -24.5f)));
        for (int i = 0; i < 10; ++i) {
            obstacles.push_back(new Obstacle(glm::vec3(-4.0f, 0.0f, -4.f * i)));
        }
        for (int i = 0; i < 10; ++i) {
            obstacles.push_back(new Obstacle(glm::vec3(+4.0f, 0.0f, -4.f * i + 1.f)));

        }
        for (float x = -boundaryX; x <= boundaryX; x += spacing) {
            if (x < -smallSquareSize || x > smallSquareSize) {
                stage1Boundary.push_back(new Obstacle(glm::vec3(x, -1.0f, -boundaryZ - 7.0f)));  // 뒷면
                stage1Boundary.push_back(new Obstacle(glm::vec3(x, -1.0f, boundaryZ - 27.0f)));   // 앞면
            }
        }
        for (float z = -boundaryZ; z <= boundaryZ - 20.0f; z += spacing) {
            stage1Boundary.push_back(new Obstacle(glm::vec3(-boundaryX, -1.0f, z - 7.0f)));  // 왼쪽
            stage1Boundary.push_back(new Obstacle(glm::vec3(boundaryX, -1.0f, z - 7.0f)));   // 오른쪽

        }
        for (float x = -smallSquareSize; x <= smallSquareSize; x += spacing) {
            // 뒷면 작은 사각형
            stage1Boundary.push_back(new Obstacle(glm::vec3(x, -1.0f, -boundaryZ - smallSquareOffset - 10.0f)));

            // 앞면 작은 사각형
            stage1Boundary.push_back(new Obstacle(glm::vec3(x, -1.0f, boundaryZ + smallSquareOffset - 25.0f)));
        }
        for (float z = -smallSquareOffset; z <= smallSquareOffset; z += spacing) {
            // 뒷면 작은 사각형 왼쪽/오른쪽
            stage1Boundary.push_back(new Obstacle(glm::vec3(-smallSquareSize, -1.0f, -boundaryZ + z - 10.0f)));
            stage1Boundary.push_back(new Obstacle(glm::vec3(smallSquareSize, -1.0f, -boundaryZ + z - 10.0f)));

            // 앞면 작은 사각형 왼쪽/오른쪽
            stage1Boundary.push_back(new Obstacle(glm::vec3(-smallSquareSize, -1.0f, boundaryZ + z - 25.0f)));
            stage1Boundary.push_back(new Obstacle(glm::vec3(smallSquareSize, -1.0f, boundaryZ + z - 25.0f)));
        }
    }
    else if (currentStage == 2) {

        std::vector<std::string> faces
        {
            "res/skybox/2-1.jpg",
            "res/skybox/2-2.jpg",
            "res/skybox/2-3.jpg",
            "res/skybox/2-4.jpg",
            "res/skybox/2-5.jpg",
            "res/skybox/2-6.jpg"
        };
        skybox = new Skybox(faces);



        character->startPos(2);
        // 스테이지 2의 초기화 로직 추가
        // 중앙 장애물
        obstacles.push_back(new Obstacle(glm::vec3(-1.0f, 0.0f, -8.0f)));

        // 세로 방향 장애물
        for (int i = 1; i <= 4; ++i) {
            obstacles.push_back(new Obstacle(glm::vec3(-1.0f, 0.0f, -8.0f - 2.0f * i)));  // 위쪽
            obstacles.push_back(new Obstacle(glm::vec3(-1.0f, 0.0f, -8.0f + 2.0f * i)));   // 아래쪽
        }

        // 가로 방향 장애물
        for (int i = 1; i <= 4; ++i) {
            obstacles.push_back(new Obstacle(glm::vec3(-1.0f - 2.0f * i, 0.0f, -8.0f)));  // 왼쪽
            obstacles.push_back(new Obstacle(glm::vec3(-1.0f + 2.0f * i, 0.0f, -8.0f)));   // 오른쪽
        }

        const std::vector<std::vector<int>>& tileMap = stage->getTilemap();

        float tileSize = 2.0f; // 타일 한 변의 크기 (Stage::Draw와 동일해야 함)
        int mapHeight = tileMap.size();
        int mapWidth = tileMap[0].size();

        for (int z = 0; z < mapHeight; ++z) {
            for (int x = 0; x < mapWidth; ++x) {
                if (tileMap[z][x] == 5) {
                    // 월드 좌표 계산
                    glm::vec3 pos = glm::vec3(
                        x * tileSize - (mapWidth / 2) * tileSize,
                        -0.25f, // 타일의 y 위치(-1.0f) 위에 약간 올려 배치
                        z * tileSize - (mapHeight / 2) * tileSize - 7.f
                    );

                    // 원형 장애물 생성 (예: 빨간색, 반지름 1.0f)
                    stage2Boundary.push_back(new Obstacle(pos));

                }
                else if (tileMap[z][x] == 6) {
                    // 월드 좌표 계산
                    glm::vec3 pos = glm::vec3(
                        x * tileSize - (mapWidth / 2) * tileSize,
                        -0.25f, // 타일의 y 위치(-1.0f) 위에 약간 올려 배치
                        z * tileSize - (mapHeight / 2) * tileSize + 0.f
                    );

                    // 원형 장애물 생성 (예: 빨간색, 반지름 1.0f)
                    coins.push_back(new Coin(pos));
                }
            }
            for (auto& boundary : stage2Boundary) {
                boundary->SetScale(glm::vec3(1.5f, 1.5f, 1.5f));
            }
        }


    }
    else if (currentStage == 3) {
        character->startPos(3);


        std::vector<std::string> faces
        {
            "res/skybox/corona_ft.png",
            "res/skybox/corona_bk.png",
            "res/skybox/corona_up.png",
            "res/skybox/corona_dn.png",
            "res/skybox/corona_rt.png",
            "res/skybox/corona_lf.png"
        };
        skybox = new Skybox(faces);



        const std::vector<std::vector<int>>& tileMap = stage->getTilemap();

        float tileSize = 2.0f; // 타일 한 변의 크기 (Stage::Draw와 동일해야 함)
        int mapHeight = tileMap.size();
        int mapWidth = tileMap[0].size();

        for (int z = 0; z < mapHeight; ++z) {
            for (int x = 0; x < mapWidth; ++x) {
                if (tileMap[z][x] == 4) {
                    // 월드 좌표 계산
                    glm::vec3 pos = glm::vec3(
                        x * tileSize - (mapWidth / 2) * tileSize,
                        -0.25f, // 타일의 y 위치(-1.0f) 위에 약간 올려 배치
                        z * tileSize - (mapHeight / 2) * tileSize - 7.f
                    );

                    // 원형 장애물 생성 (예: 빨간색, 반지름 1.0f)
                    stage3Boundary.push_back(new Obstacle(pos));

                }
                else if (tileMap[z][x] == 6) {
                    // 월드 좌표 계산
                    glm::vec3 pos = glm::vec3(
                        x * tileSize - (mapWidth / 2) * tileSize,
                        -0.25f, // 타일의 y 위치(-1.0f) 위에 약간 올려 배치
                        z * tileSize - (mapHeight / 2) * tileSize + 0.f
                    );

                    // 원형 장애물 생성 (예: 빨간색, 반지름 1.0f)
                    coins.push_back(new Coin(pos));
                }
                else if (tileMap[z][x] == 6) {
                    // 월드 좌표 계산
                    glm::vec3 pos = glm::vec3(
                        x * tileSize - (mapWidth / 2) * tileSize,
                        -0.25f, // 타일의 y 위치(-1.0f) 위에 약간 올려 배치
                        z * tileSize - (mapHeight / 2) * tileSize + 0.f
                    );

                    // 원형 장애물 생성 (예: 빨간색, 반지름 1.0f)
                    coins.push_back(new Coin(pos));
                }
                else if (tileMap[z][x] == 6) {
                    // 월드 좌표 계산
                    glm::vec3 pos = glm::vec3(
                        x * tileSize - (mapWidth / 2) * tileSize,
                        -0.25f, // 타일의 y 위치(-1.0f) 위에 약간 올려 배치
                        z * tileSize - (mapHeight / 2) * tileSize + 0.f
                    );

                    // 원형 장애물 생성 (예: 빨간색, 반지름 1.0f)
                    coins.push_back(new Coin(pos));
                }
            }
            //첫 번째 줄
            for (int i = 0; i <= 10; ++i) {
            }
            //첫 번째 줄
            for (int i = 0; i <= 10; ++i) {
                if (i == 0 || i == 1 || i == 2 || i == 4 || i == 5 || i == 6 || i == 8 || i == 9 || i == 10) {
                    glm::vec3 pos = glm::vec3(-21.0f, 0.0f, i * tileSize - 20.0f);
                    Obstacle* movingCube = new Obstacle(pos, -1);
                    obstacles.push_back(movingCube);
                }
            }
        }
        for (int i = 0; i <= 10; ++i) {
            if (i == 0 || i == 4 || i == 8) {
                glm::vec3 pos = glm::vec3(-19.0f, 0.0f, i * tileSize - 22.0f);
                Obstacle* movingCube = new Obstacle(pos, 1);
                obstacles.push_back(movingCube);
            }
        }
        //두 번째 줄
        for (int i = 0; i <= 16; ++i) {
            if (i == 0 || i == 2 || i == 4 || i == 6 || i == 8 || i == 10 || i == 12 || i == 14 || i == 16) {
                glm::vec3 pos = glm::vec3(-11.0f, 0.0f, i * tileSize - 22.0f);
                Obstacle* movingCube = new Obstacle(pos, 2);
                obstacles.push_back(movingCube);
            }
        }
        //세 번째 줄
        for (int i = 0; i <= 15; ++i) {
            if (i == 0 || i == 3 || i == 6 || i == 9 || i == 12) {
                glm::vec3 pos = glm::vec3(-4.0f, 0.0f, i * tileSize - 21.0f);
                Obstacle* movingCube = new Obstacle(pos, 3);
                obstacles.push_back(movingCube);
            }
        }
        for (int i = 0; i <= 15; ++i) {
            if (i == 0 || i == 3 || i == 6 || i == 9 || i == 12) {
                glm::vec3 pos = glm::vec3(-2.0f, 0.0f, i * tileSize - 21.0f);
                Obstacle* movingCube = new Obstacle(pos, 3);
                obstacles.push_back(movingCube);
            }
        }
        for (int i = 0; i <= 15; ++i) {
            if (i == 0 || i == 3 || i == 6 || i == 9 || i == 12 || i == 15) {
                glm::vec3 pos = glm::vec3(0.0f, 0.0f, i * tileSize - 24.0f);
                Obstacle* movingCube = new Obstacle(pos, 4);
                obstacles.push_back(movingCube);
            }
        }
        for (int i = 0; i <= 15; ++i) {
            if (i == 0 || i == 3 || i == 6 || i == 9 || i == 12 || i == 15) {
                glm::vec3 pos = glm::vec3(2.0f, 0.0f, i * tileSize - 24.0f);
                Obstacle* movingCube = new Obstacle(pos, 4);
                obstacles.push_back(movingCube);
            }
        }
        //네 번째 줄
        for (int i = 0; i <= 15; ++i) {
            if (i == 0 || i == 3 || i == 6 || i == 9 || i == 12 || i == 15) {
                glm::vec3 pos = glm::vec3(12.0f, 0.0f, i * tileSize - 24.0f);
                Obstacle* movingCube = new Obstacle(pos, 5);
                obstacles.push_back(movingCube);
            }
        }
        for (int i = 0; i <= 15; ++i) {
            if (i == 0 || i == 3 || i == 6 || i == 9 || i == 12 || i == 15) {
                glm::vec3 pos = glm::vec3(10.0f, 0.0f, i * tileSize - 24.0f);
                Obstacle* movingCube = new Obstacle(pos, 6);
                obstacles.push_back(movingCube);
            }
        }
        for (int i = 0; i <= 15; ++i) {
            if (i == 0 || i == 3 || i == 6 || i == 9 || i == 12 || i == 15) {
                glm::vec3 pos = glm::vec3(8.0f, 0.0f, i * tileSize - 24.0f);
                Obstacle* movingCube = new Obstacle(pos, 5);
                obstacles.push_back(movingCube);
            }
        }
        for (int i = 0; i <= 15; ++i) {
            if (i == 0 || i == 3 || i == 6 || i == 9 || i == 12 || i == 15) {
                glm::vec3 pos = glm::vec3(6.0f, 0.0f, i * tileSize - 24.0f);
                Obstacle* movingCube = new Obstacle(pos, -1);
                obstacles.push_back(movingCube);
            }
        }
        //다섯 번째 줄
        for (int i = 0; i <= 19; ++i) {
            if (i == 0 || i == 1 || i == 2 || i == 3 || i == 7 || i == 8 || i == 9 || i == 10 || i == 14 || i == 15 || i == 16 || i == 17 || i == 18 || i == 19) {
                glm::vec3 pos = glm::vec3(20.0f, 0.0f, i * tileSize - 26.0f);
                Obstacle* movingCube = new Obstacle(pos, 7);
                obstacles.push_back(movingCube);
            }
        }
      

        for (auto& boundary : stage3Boundary) {
            boundary->SetScale(glm::vec3(1.5f, 1.5f, 1.5f));
        }
    }
}

void Game::Run() {

    glEnable(GL_DEPTH_TEST);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    Init();

    glutDisplayFunc(DisplayCallback);
    glutIdleFunc(DisplayCallback);
    glutKeyboardFunc(KeyboardDownCallback);
    glutKeyboardUpFunc(KeyboardUpCallback);
    glutReshapeFunc(ReshapeCallback);
    glutMouseFunc(MouseCallback);  // 마우스 클릭 콜백
    glutMotionFunc(MotionCallback); // 마우스 이동 콜백
    glutPassiveMotionFunc(PassiveMotionCallback);
    glutSetCursor(GLUT_CURSOR_NONE); // 커서 가리기
    glutMainLoop();
}

void Game::Update(float deltaTime) {

    if (camera->mode == FIRST_PERSON || camera->mode == THIRD_PERSON) {
        character->Move(deltaTime, keys, *camera);
    }
    else
        character->Move(deltaTime, keys);
    stage->Update(deltaTime);
    for (auto& obstacle : obstacles) {
        obstacle->Update(deltaTime, currentStage);

        if (CheckCollisionAABBAndSphere(*character, *obstacle)) {
            if (!character->isInvincible) {
                //std::cout << "Collision detected! Character is not invincible." << std::endl;
                // 캐릭터가 충돌했을 때 초기 위치로 이동
                character->startPos(currentStage);
                for (auto& coin : coins) {
                    coin->SetNotCollected();
                }
            }
            else {
               // std::cout << "Collision detected, but character is invincible." << std::endl;
            }
        }
    }
    for (auto& boundary : stage1Boundary) {
        if (CheckCollisionAABBAndSphere(*character, *boundary)) {
            character->startPos(currentStage);
            for (auto& coin : coins) {
                coin->SetNotCollected();
            }
            return;
        }
    }
    if (instance->currentStage == 2) {
        for (auto& boundary : stage2Boundary) {
            if (!character->isInvincible && CheckCollisionAABBAndSphere(*character, *boundary)) {
                character->startPos(currentStage);
                for (auto& coin : coins) {
                    coin->SetNotCollected();
                }
            }
        }
    }

    if (instance->currentStage == 3) {
        for (auto& boundary : stage3Boundary) {
            if (!character->isInvincible && CheckCollisionAABBAndSphere(*character, *boundary)) {
                character->startPos(currentStage);
                for (auto& coin : coins) {
                    coin->SetNotCollected();
                }
            }
        }
    }
    for (auto& coin : coins) {
        coin->Update(deltaTime);
    }
    int count = 0;
    for (auto& coin : coins) {
        if (!coin->IsCollected() && CheckCollisionCharacterAndCoin(*character, *coin)) {
            //std::cout << "Coin collected!" << std::endl;
            coin->SetCollected(); // 코인 수집 상태로 전환
            
        }
    }
    for (auto& coin : coins) {
        if (coin->IsCollected()) {
            count++;
        }
    }
    
    if (CheckGoalArea()) {
        std::cout << count << "," << coins.size();
        if (count == coins.size()) {
            //std::cout <<"all clear" << std::endl;
            ++currentStage;
            if (currentStage > 3) {
                currentStage -= 3;
            }
            MoveStage(currentStage);
        }
        
    }

    camera->update(*character, deltaTime, mouseOffsetX, mouseOffsetY);

}
void Game::Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f); // 배경색 설정

    // 메인 뷰포트 설정
    glViewport(0, 0, 1600, 900);
    shader->Use();

    // 뷰 및 프로젝션 매트릭스 설정
    glm::mat4 view = camera->GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1600.0f / 900.0f, 0.1f, 100.0f);

    shader->setMat4("view", view);
    shader->setMat4("projection", projection);

  
    glm::vec3 mainLightPos = glm::vec3(0.0f, 20.0f, -10.0f); 
    glm::vec3 mainLightColor = glm::vec3(1.0f, 1.0f, 1.0f); 

   
    if (currentStage == 2 && !lightOn) {
        mainLightPos = glm::vec3(-1.0f, 4.0f, -3.0f); 
        mainLightColor = glm::vec3(0.1f, 0.1f, 0.1f); 
    }
    else if (currentStage == 3 && !lightOn) {
        mainLightPos = glm::vec3(-1.0f, 4.0f, -3.0f);
        mainLightColor = glm::vec3(0.05f, 0.05f, 0.05f); 
    }

    shader->setFloat("alpha", 1.0f);
    shader->setVec3("lightPos", mainLightPos);
    shader->setVec3("lightColor", mainLightColor);
    shader->setVec3("viewPos", camera->Position);

    
    shader->setBool("showNormals", showNormals);

    
    shader->setVec3("emission", glm::vec3(0.0f));

    
    pointLights.clear();
    for (auto& coin : coins) {
        if (!coin->IsCollected()) {
            PointLight pl;
            pl.position = coin->GetPosition();
            pl.color = glm::vec3(1.0f, 0.843f, 0.0f); // 금색 빛
            pl.intensity = 0.7f;
            pointLights.push_back(pl);
        }

        if (instance->currentStage == 2) {
            PointLight p1;
            p1.position = character->getPosition();
            p1.position.y += 2.0f;
            p1.color = glm::vec3(1.0f, 1.0f, 1.0f);
            p1.intensity = 0.9f;
            pointLights.push_back(p1);
        }
        else if (instance->currentStage == 3) {
            PointLight p1;
            p1.position = character->getPosition();
            p1.position.y += 2.0f;
            p1.color = glm::vec3(1.0f, 1.0f, 1.0f);
            p1.intensity = 2.7f;
            pointLights.push_back(p1);
        }

        // 점광원 유니폼 설정
        int activePointLights = static_cast<int>(pointLights.size());
        if (activePointLights > MAX_POINT_LIGHTS) {
            activePointLights = MAX_POINT_LIGHTS; // 최대 광원 수 제한
        }
        shader->setInt("numPointLights", activePointLights);
        for (int i = 0; i < activePointLights; ++i) {
            std::string idx = std::to_string(i);
            shader->setVec3("pointLights[" + idx + "].position", pointLights[i].position);
            shader->setVec3("pointLights[" + idx + "].color", pointLights[i].color);
            shader->setFloat("pointLights[" + idx + "].intensity", pointLights[i].intensity);
        }

        // 발광 비활성화
        shader->setVec3("emission", glm::vec3(0.0f));

        // 객체 렌더링

        skybox->Draw(view, projection);


        if (camera->mode != FIRST_PERSON) {
            character->Draw(*shader);
        }

        for (auto& obs : obstacles) {
            obs->Draw(*shader);
        }
        if (instance->currentStage == 3) {
            for (auto& boundary : stage3Boundary) {
                boundary->Draw(*shader);
            }
        }
        for (auto& boundary : stage2Boundary) {
            boundary->Draw(*shader);
        }

        for (auto& boundary : stage1Boundary) {
            boundary->Draw(*shader);
        }

        for (auto& coin : coins) {
            coin->Draw(*coinShader, view, projection);
        }
        shader->Use();
        glDepthMask(GL_FALSE);
        stage->Draw(*shader, currentStage);
        glDepthMask(GL_TRUE);

        // 미니맵 렌더링
        // 미니맵 1
        glViewport(1200, 700, 400, 200);

        shader->Use();

        float orthoWidth = 30.0f; 
        float orthoHeight = 20.0f;
        view = glm::lookAt(glm::vec3(character->getPosition().x, 30.0f, character->getPosition().z),
            glm::vec3(character->getPosition().x, 0.0f, character->getPosition().z),
            glm::vec3(0.0f, 0.0f, -1.0f));
        projection = glm::ortho(-orthoWidth / 2.0f, orthoWidth / 2.0f,
            -orthoHeight / 2.0f, orthoHeight / 2.0f,
            0.1f, 100.0f);

        shader->setMat4("view", view);
        shader->setMat4("projection", projection);
        shader->setVec3("emission", glm::vec3(0.0f));
        shader->setInt("numPointLights", pointLights.size());

        character->Draw(*shader);
        for (auto& obs : obstacles) {
            obs->Draw(*shader);
        }
        for (auto& coin : coins) {
            coin->Draw(*coinShader, view, projection); 
        }
        if (currentStage == 3) {
            for (auto& boundary : stage3Boundary) {
                boundary->Draw(*shader);
            }
        }
        stage->Draw(*shader, currentStage);
    }
    glutSwapBuffers();
}

void Game::SwitchCameraMode() {
    camera->SwitchMode();
}

void Game::MoveStage(int stageNum) {
    currentStage = stageNum;
    Init();
}

void Game::DisplayCallback() {
    float currentFrame = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    instance->deltaTime = currentFrame - instance->lastFrame;
    instance->lastFrame = currentFrame;
    instance->CenterMouse(); // 마우스 중앙처리
    instance->Update(instance->deltaTime);
    instance->Render();
}

void Game::KeyboardDownCallback(unsigned char key, int x, int y) {
    instance->keys[key] = true;
    if (key == 'i') {
        instance->character->ToggleInvincibility();
    }  else if (key == 'c') {
        instance->SwitchCameraMode();
    } else if (key == '1') {
        instance->MoveStage(1);
    } else if (key == '2') {
        instance->MoveStage(2);
    } else if (key == '3') {
        instance->MoveStage(3);
    }  else if (key == 'y') {
        instance->wireframe = !instance->wireframe;
        if (instance->wireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    }  else if (key == 'n' || key == 'N') {
        instance->showNormals = !instance->showNormals;
    } else if (key == 'l' || key == 'L') {
        instance->lightOn = !instance->lightOn;
    } else if (key == 27) { // ESC 키
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
  
    glm::vec3 aabbMin = character.Position - character.Scale * 0.5f;
    glm::vec3 aabbMax = character.Position + character.Scale * 0.5f;

    glm::vec3 sphereCenter = obstacle.Position;
    float sphereRadius = 0.5f * obstacle.Scale.x; // Collision Shpere radius

    glm::vec3 closestPoint = glm::clamp(sphereCenter, aabbMin, aabbMax);

    float distance = glm::length(closestPoint - sphereCenter);
    return distance <= sphereRadius;

}


void Game::CenterMouse() { // 마우스 중심으로 옮겨 마우스 나가는거 방지 
    int centerX = glutGet(GLUT_WINDOW_WIDTH) / 2;
    int centerY = glutGet(GLUT_WINDOW_HEIGHT) / 2;

    glutWarpPointer(centerX, centerY);

    lastMouseX = centerX;
    lastMouseY = centerY;
}

bool Game::CheckGoalArea()  // 캐릭터 목표 지점 도달 확인
{
     glm::vec3 charPos = character->Position;

    bool inGoalArea = 
        charPos.x >= goalAreaMin.x && charPos.x <= goalAreaMax.x &&
        charPos.z >= goalAreaMin.z && charPos.z <= goalAreaMax.z;

    if (inGoalArea) {
        //std::cout << "" << std::endl;
    }
    return inGoalArea;
}

 void Game::MouseCallback(int button, int state, int x, int y) {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            int viewport[4];
            glGetIntegerv(GL_VIEWPORT, viewport);
            float winX = static_cast<float>(x);
            float winY = static_cast<float>(viewport[3] - y); 
            float winZ;
            glReadPixels(x, viewport[3] - y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
            glm::mat4 view = instance->camera->GetViewMatrix();
            glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1600.0f / 900.0f, 0.1f, 100.0f);
            glm::vec4 viewportVec = glm::vec4(viewport[0], viewport[1], viewport[2], viewport[3]);
            glm::vec3 screenPos(winX, winY, winZ);
            glm::vec3 worldPos = glm::unProject(screenPos, view, projection, viewportVec);
            std::cout << "Mouse clicked at: "
                << "Window coords: (" << x << ", " << y << "), "
                << "World coords: (" << worldPos.x << ", " << worldPos.y << ", " << worldPos.z << ")"
                << std::endl;
        }
}

void Game::MotionCallback(int x, int y) {
    int centerX = glutGet(GLUT_WINDOW_WIDTH) / 2;
    int centerY = glutGet(GLUT_WINDOW_HEIGHT) / 2;

    // 마우스 이동량 계산
    float deltaX = x - instance->lastMouseX;
    float deltaY = y - instance->lastMouseY;

    // 민감도 조정
    float sensitivity = 0.1f;
    deltaX *= sensitivity;
    deltaY *= sensitivity;

    // 카메라 회전 업데이트
    instance->camera->Yaw += deltaX;
    instance->camera->Pitch -= deltaY;

    // Pitch 제한
    if (instance->camera->Pitch > 89.0f) instance->camera->Pitch = 89.0f;
    if (instance->camera->Pitch < -89.0f) instance->camera->Pitch = -89.0f;

    // 마우스를 창의 중심으로 이동
    instance->CenterMouse();
}


void Game::PassiveMotionCallback(int x, int y) {
    int centerX = glutGet(GLUT_WINDOW_WIDTH) / 2;
    int centerY = glutGet(GLUT_WINDOW_HEIGHT) / 2;

    // 마우스 이동량 계산
    float deltaX = x - instance->lastMouseX;
    float deltaY = y - instance->lastMouseY;

    // 민감도 조정
    float sensitivity = 0.1f;
    deltaX *= sensitivity;
    deltaY *= sensitivity;

    // 카메라 회전 업데이트
    instance->camera->Yaw += deltaX;
    instance->camera->Pitch -= deltaY;

    // Pitch 제한
    if (instance->camera->Pitch > 89.0f) instance->camera->Pitch = 89.0f;
    if (instance->camera->Pitch < -89.0f) instance->camera->Pitch = -89.0f;

    // 마우스 마지막 위치 업데이트
    instance->lastMouseX = x;
    instance->lastMouseY = y;

    // 마우스를 창의 중심으로 이동
    instance->CenterMouse();
}

bool Game::CheckCollisionCharacterAndCoin(const Character& character, Coin& coin) {
   
    glm::vec3 charMin = character.Position - character.Scale * 0.5f;
    glm::vec3 charMax = character.Position + character.Scale * 0.5f;

    
    float coinRadius = 0.5f; // 코인의 반지름
    glm::vec3 coinMin = coin.GetPosition() - glm::vec3(coinRadius);
    glm::vec3 coinMax = coin.GetPosition() + glm::vec3(coinRadius);
 
    bool collisionX = charMax.x > coinMin.x && charMin.x < coinMax.x;
    bool collisionY = charMax.y > coinMin.y && charMin.y < coinMax.y;
    bool collisionZ = charMax.z > coinMin.z && charMin.z < coinMax.z;

    return collisionX && collisionY && collisionZ;

}