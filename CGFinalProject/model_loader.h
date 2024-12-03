#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include <vector>
#include <string>
#include <gl/glm/glm.hpp>

// Vertex 구조체 정의
struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
};

class ModelLoader {
public:
    static bool LoadModel(const std::string& path, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
};

#endif