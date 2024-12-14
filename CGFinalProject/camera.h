#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include "character.h"

enum CameraMode {
    FIRST_PERSON,
    THIRD_PERSON,
    TOP_VIEW
};

class Camera {
public:
    glm::vec3 Position;
    glm::vec3 Target;
    glm::vec3 Up;

    float Yaw;
    float Pitch;
    float MouseSensitivity;

    CameraMode mode;

    Camera();
    glm::mat4 GetViewMatrix();
    void SwitchMode();
    void SetFirstPerson();
    void SetThirdPerson();
    void SetTopView();
    void update(Character& character, float deltaTime, float mouseOffsetX, float mouseOffsetY);
    glm::vec3 GetForwardVector() const;
    glm::vec3 GetRightVector() const;
};

#endif
