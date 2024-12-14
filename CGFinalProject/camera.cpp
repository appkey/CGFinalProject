#include "camera.h"



Camera::Camera() {
    mode = THIRD_PERSON;
    Position = glm::vec3(0.0f, 5.0f, 10.0f);
    Target = glm::vec3(0.0f, 0.0f, 0.0f);
    Up = glm::vec3(0.0f, 1.0f, 0.0f);

    Yaw = -90.0f; // 기본적으로 Z축을 바라봄
    Pitch = 0.0f;
    MouseSensitivity = 0.1f;

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

// 1인칭 모드 초기화
void Camera::SetFirstPerson() {
    Pitch = 0.0f; // 수직 방향 초기화
    Yaw = -90.0f; // Z축 바라보기 초기화
    MouseSensitivity = 0.1f; // 민감도 설정

    // 방향 벡터 재계산
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Target = glm::normalize(front);
}

// 3인칭 모드 초기화
void Camera::SetThirdPerson() {
    Position = glm::vec3(0.0f, 3.0f, 7.0f); // 캐릭터를 중심으로 고정 거리 설정
    Target = glm::vec3(0.0f, 0.0f, 0.0f);   // 캐릭터 중심 바라보기
    Up = glm::vec3(0.0f, 1.0f, 0.0f);       // 상향 벡터 초기화
}

// 탑 뷰 모드 초기화
void Camera::SetTopView() {
    Position = glm::vec3(0.0f, 30.0f, 0.0f); // 캐릭터 위로 설정
    Target = glm::vec3(0.0f, 0.0f, 0.0f);   // 캐릭터 바라보기
    Up = glm::vec3(0.0f, 0.0f, -1.0f);      // 카메라 방향 재설정
}

// Update 함수 수정
void Camera::update(Character& character, float deltaTime, float mouseOffsetX, float mouseOffsetY) {
    if (mode == FIRST_PERSON) {
        // 마우스 입력으로 Yaw와 Pitch 변경
        Yaw += mouseOffsetX * MouseSensitivity;
        Pitch -= mouseOffsetY * MouseSensitivity;

        // Pitch 제한
        if (Pitch > 89.0f) Pitch = 89.0f;
        if (Pitch < -89.0f) Pitch = -89.0f;

        // 방향 벡터 계산
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Target = glm::normalize(front);

        // 카메라 위치를 캐릭터 위치로 설정
        Position = character.Position;
    }
    else if (mode == THIRD_PERSON) {
        // 캐릭터를 따라가는 3인칭 카메라
          // 3인칭 카메라 회전 및 위치 계산
        Yaw += mouseOffsetX * MouseSensitivity; // 좌우 회전
        Pitch += mouseOffsetY * MouseSensitivity; // 위아래 회전

        // Pitch 제한
        if (Pitch > 89.0f) Pitch = 89.0f;
        if (Pitch < -10.0f) Pitch = -10.0f; // 카메라가 너무 아래로 내려가지 않도록 제한

        float radius = 7.0f; // 캐릭터와 카메라의 거리
        glm::vec3 offset;
        offset.x = radius * cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        offset.y = radius * sin(glm::radians(Pitch));
        offset.z = radius * sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

        Position = character.Position + offset; // 카메라 위치 계산
        Target = character.Position; // 캐릭터를 바라보도록 설정
        Up = glm::vec3(0.0f, 1.0f, 0.0f); // 상향 벡터 유지
    }
    else if (mode == TOP_VIEW) {
        // 탑 뷰 모드에서 캐릭터 위로 설정
        Position = glm::vec3(character.Position.x, 30.0f, character.Position.z);
        Target = glm::vec3(character.Position.x, 0.0f, character.Position.z);
        Up = glm::vec3(0.0f, 0.0f, -1.0f);
    }
}


glm::vec3 Camera::GetForwardVector() const {
    return glm::normalize(Target); // 정규화된 전방 벡터
}

glm::vec3 Camera::GetRightVector() const {
    return glm::normalize(glm::cross(GetForwardVector(), Up)); // 정규화된 오른쪽 벡터
}