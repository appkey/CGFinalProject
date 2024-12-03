#ifndef GAME_H
#define GAME_H

#include "stage.h"
#include "character.h"
#include "camera.h"
#include "shader.h"
#include "obstacle.h"
#include <vector>

class Game {
public:
    int currentStage;
    Stage* stage;
    Character* character;
    Camera* camera;
    Shader* shader;
    std::vector<Obstacle*> obstacles;

    Game();
    ~Game();
    void Init();
    void Run();
    void Update(float deltaTime);
    void Render();
    void SwitchCameraMode();
    void NextStage();

    // �ݹ� �Լ���
    static void DisplayCallback();
    static void KeyboardDownCallback(unsigned char key, int x, int y);
    static void KeyboardUpCallback(unsigned char key, int x, int y);
    static void ReshapeCallback(int width, int height);

private:
    static Game* instance;
    float deltaTime;
    float lastFrame;
    bool keys[256]; // Ű �Է� ����
};

#endif
