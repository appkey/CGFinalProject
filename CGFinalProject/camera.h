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

    CameraMode mode;

    Camera();
    glm::mat4 GetViewMatrix();
    void SwitchMode();
    void update(Character& character);
};

#endif
