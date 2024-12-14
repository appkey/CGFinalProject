#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include <gl/glm/glm.hpp>

struct PointLight {
    glm::vec3 position;
    glm::vec3 color;
    float intensity;
};

#endif
