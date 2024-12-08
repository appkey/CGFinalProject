#include "camera.h"



Camera::Camera() {
    mode = TOP_VIEW;
    Position = glm::vec3(0.0f, 5.0f, 10.0f);
    Target = glm::vec3(0.0f, 0.0f, 0.0f);
    Up = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::mat4 Camera::GetViewMatrix() {
    if (mode == FIRST_PERSON) {
        // 1��Ī ī�޶� ����
        return glm::lookAt(Position, Position + Target, Up);
    }
    else if (mode == THIRD_PERSON) {
        // 3��Ī ī�޶� ����
        return glm::lookAt(Position, Target, Up);
    }
    else if (mode == TOP_VIEW) {
        // ž �� ī�޶� ����

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
        Position = glm::vec3(character.getPosition().x, 30.0f, character.getPosition().z); // ĳ���� ���� �̵�
        Target = glm::vec3(character.getPosition().x, 0.0f, character.getPosition().z);    // ĳ���� ��ġ�� �ٶ�
        Up = glm::vec3(0.0f, 0.0f, -1.0f); // ī�޶� ������ �������� �ʵ��� ����
        break;
    case THIRD_PERSON:
        Position = character.Position + glm::vec3(0.0f, 3.0f, 7.0f);
        Target = character.Position;
        Up = glm::vec3(0.0f, 0.0f, -1.0f);
        break;
    }
}