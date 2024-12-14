#ifndef GAME_H
#define GAME_H
#define MAX_POINT_LIGHTS 10
#include "stage.h"
#include "character.h"
#include "camera.h"
#include "shader.h"
#include "obstacle.h"
#include "skybox.h"

#include "coin.h"
#include <vector>
#include "point_light.h"
class Game {
public:
    int currentStage;
    Stage* stage;
    Character* character;
    Camera* camera;
    Shader* shader;
    Shader* coinShader;
    std::vector<Obstacle*> obstacles;
    std::vector<Coin*> coins;           // Stage Clear Condition preys.size() == 0이여야함
    std::vector<PointLight> pointLights;
    std::vector<Obstacle*> stage3Boundary;
    Skybox* skybox;

    bool wireframe;
    bool showNormals; // 노멀 시각화 여부



    Game();
    ~Game();
    void Init();
    void Run();
    void Update(float deltaTime);
    void Render();
    void SwitchCameraMode();
    void MoveStage(int);
    bool CheckCollisionAABBAndSphere(const Character&, const Obstacle&);



    // 콜백 함수들
    static void DisplayCallback();
    static void KeyboardDownCallback(unsigned char key, int x, int y);
    static void KeyboardUpCallback(unsigned char key, int x, int y);
    static void ReshapeCallback(int width, int height);

private:
    static Game* instance;
    float deltaTime;
    float lastFrame;
    bool keys[256]; // 키 입력 상태
};  

#endif
