#ifndef COIN_H
#define COIN_H

#include <gl/glm/glm.hpp>
#include "shader.h"

class Coin {
public:
    Coin(const glm::vec3& position);

    void Update(float deltaTime);
    void Draw(Shader& shader);

    void Draw(Shader& shader, const glm::mat4& view, const glm::mat4& projection);

    void SetCollected();
    void SetNotCollected();
    bool IsCollected() const;
    

    const glm::vec3& GetPosition() const;

    // 초기화 함수
    void InitRenderData();

private:
    glm::vec3 position;
    bool collected;
    float rotationAngle;

    unsigned int vao, vbo, ebo;
    int indexCount;
};

#endif
