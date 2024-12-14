#include "camera.h"



Camera::Camera() {
    mode = THIRD_PERSON;
    Position = glm::vec3(0.0f, 5.0f, 10.0f);
    Target = glm::vec3(0.0f, 0.0f, 0.0f);
    Up = glm::vec3(0.0f, 1.0f, 0.0f);

    Yaw = -90.0f; // �⺻������ Z���� �ٶ�
    Pitch = 0.0f;
    MouseSensitivity = 0.1f;

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
        SetThirdPerson();
    }
    else if (mode == THIRD_PERSON) {
        mode = FIRST_PERSON;
        SetFirstPerson();
    }
    else if (mode == FIRST_PERSON) {
        mode = TOP_VIEW;
        SetTopView();
    }
}

// 1��Ī ��� �ʱ�ȭ
void Camera::SetFirstPerson() {
    Pitch = 0.0f; // ���� ���� �ʱ�ȭ
    Yaw = -90.0f; // Z�� �ٶ󺸱� �ʱ�ȭ
    MouseSensitivity = 0.1f; // �ΰ��� ����

    // ���� ���� ����
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Target = glm::normalize(front);
}

// 3��Ī ��� �ʱ�ȭ
void Camera::SetThirdPerson() {
    Position = glm::vec3(0.0f, 3.0f, 7.0f); // ĳ���͸� �߽����� ���� �Ÿ� ����
    Target = glm::vec3(0.0f, 0.0f, 0.0f);   // ĳ���� �߽� �ٶ󺸱�
    Up = glm::vec3(0.0f, 1.0f, 0.0f);       // ���� ���� �ʱ�ȭ
}

// ž �� ��� �ʱ�ȭ
void Camera::SetTopView() {
    Position = glm::vec3(0.0f, 30.0f, 0.0f); // ĳ���� ���� ����
    Target = glm::vec3(0.0f, 0.0f, 0.0f);   // ĳ���� �ٶ󺸱�
    Up = glm::vec3(0.0f, 0.0f, -1.0f);      // ī�޶� ���� �缳��
}

// Update �Լ� ����
void Camera::update(Character& character, float deltaTime, float mouseOffsetX, float mouseOffsetY) {
    if (mode == FIRST_PERSON) {
        // ���콺 �Է����� Yaw�� Pitch ����
        Yaw += mouseOffsetX * MouseSensitivity;
        Pitch -= mouseOffsetY * MouseSensitivity;

        // Pitch ����
        if (Pitch > 89.0f) Pitch = 89.0f;
        if (Pitch < -89.0f) Pitch = -89.0f;

        // ���� ���� ���
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Target = glm::normalize(front);

        // ī�޶� ��ġ�� ĳ���� ��ġ�� ����
        Position = character.Position;
    }
    else if (mode == THIRD_PERSON) {
        // ĳ���͸� ���󰡴� 3��Ī ī�޶�
          // 3��Ī ī�޶� ȸ�� �� ��ġ ���
        Yaw += mouseOffsetX * MouseSensitivity; // �¿� ȸ��
        Pitch += mouseOffsetY * MouseSensitivity; // ���Ʒ� ȸ��

        // Pitch ����
        if (Pitch > 89.0f) Pitch = 89.0f;
        if (Pitch < -10.0f) Pitch = -10.0f; // ī�޶� �ʹ� �Ʒ��� �������� �ʵ��� ����

        float radius = 7.0f; // ĳ���Ϳ� ī�޶��� �Ÿ�
        glm::vec3 offset;
        offset.x = radius * cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        offset.y = radius * sin(glm::radians(Pitch));
        offset.z = radius * sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

        Position = character.Position + offset; // ī�޶� ��ġ ���
        Target = character.Position; // ĳ���͸� �ٶ󺸵��� ����
        Up = glm::vec3(0.0f, 1.0f, 0.0f); // ���� ���� ����
    }
    else if (mode == TOP_VIEW) {
        // ž �� ��忡�� ĳ���� ���� ����
        Position = glm::vec3(character.Position.x, 30.0f, character.Position.z);
        Target = glm::vec3(character.Position.x, 0.0f, character.Position.z);
        Up = glm::vec3(0.0f, 0.0f, -1.0f);
    }
}


glm::vec3 Camera::GetForwardVector() const {
    return glm::normalize(Target); // ����ȭ�� ���� ����
}

glm::vec3 Camera::GetRightVector() const {
    return glm::normalize(glm::cross(GetForwardVector(), Up)); // ����ȭ�� ������ ����
}