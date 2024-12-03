#include "camera.h"

Camera::Camera() {
    mode = THIRD_PERSON;
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
        glm::vec3 topPosition = glm::vec3(Target.x, Target.y + 10.0f, Target.z);
        return glm::lookAt(topPosition, Target, Up);
    }
    return glm::mat4(1.0f);
}

void Camera::SwitchMode() {
    if (mode == FIRST_PERSON) {
        mode = THIRD_PERSON;
    }
    else if (mode == THIRD_PERSON) {
        mode = TOP_VIEW;
    }
    else if (mode == TOP_VIEW) {
        mode = FIRST_PERSON;
    }
}
