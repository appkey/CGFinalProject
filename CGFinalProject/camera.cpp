#include "camera.h"



Camera::Camera() {
    mode = TOP_VIEW;
    Position = glm::vec3(0.0f, 5.0f, 10.0f);
    Target = glm::vec3(0.0f, 0.0f, 0.0f);
    Up = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::mat4 Camera::GetViewMatrix() {
    if (mode == FIRST_PERSON) {
        // 1인칭 카메라 설정
        return glm::lookAt(Position, Position + Target, Up);
    }
    else if (mode == THIRD_PERSON) {
        // 3인칭 카메라 설정
        return glm::lookAt(Position, Target, Up);
    }
    else if (mode == TOP_VIEW) {
        // 탑 뷰 카메라 설정

        return glm::lookAt(Position, Target, Up);
    }
    return glm::mat4(1.0f);
}

void Camera::SwitchMode() {
    if (mode == TOP_VIEW) {
        mode = THIRD_PERSON;
    }
    else if (mode == THIRD_PERSON) {
        mode = TOP_VIEW;
    }
    /*else if (mode == TOP_VIEW) {
        mode = FIRST_PERSON;
    }*/
}

void Camera::update(Character& character) {
    switch (mode) {
    case TOP_VIEW:
        Position = glm::vec3(character.getPosition().x, 30.0f, character.getPosition().z); // 캐릭터 위로 이동
        Target = glm::vec3(character.getPosition().x, 0.0f, character.getPosition().z);    // 캐릭터 위치를 바라봄
        Up = glm::vec3(0.0f, 0.0f, -1.0f); // 카메라 방향이 뒤집히지 않도록 설정
        break;
    case THIRD_PERSON:
        Position = character.Position + glm::vec3(0.0f, 3.0f, 7.0f);
        Target = character.Position;
        Up = glm::vec3(0.0f, 0.0f, -1.0f);
        break;
    }
}