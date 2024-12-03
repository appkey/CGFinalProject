#include "model_loader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <gl/glm/glm.hpp>


bool ModelLoader::LoadModel(const std::string& path, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) {
    // 파일 열기 및 오류 처리
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "Failed to open model file: " << path << std::endl;
        return false;
    }

    // 임시 저장소
    std::vector<glm::vec3> temp_positions;
    std::vector<glm::vec3> temp_normals;
    std::vector<glm::vec2> temp_texcoords;

    // 중복 정점 관리를 위한 맵
    struct VertexIndices {
        unsigned int positionIndex;
        unsigned int texcoordIndex;
        unsigned int normalIndex;

        bool operator<(const VertexIndices& other) const {
            return std::tie(positionIndex, texcoordIndex, normalIndex) < std::tie(other.positionIndex, other.texcoordIndex, other.normalIndex);
        }
    };
    std::map<VertexIndices, unsigned int> uniqueVertices;

    // OBJ 파일 파싱
    std::string line;
    while (getline(file, line)) {
        if (line.substr(0, 1) == "#") continue; // 주석 무시

        std::istringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "v") {
            // 정점 좌표
            glm::vec3 position;
            ss >> position.x >> position.y >> position.z;
            temp_positions.push_back(position);
        }
        else if (prefix == "vt") {
            // 텍스처 좌표
            glm::vec2 texcoord;
            ss >> texcoord.x >> texcoord.y;
            temp_texcoords.push_back(texcoord);
        }
        else if (prefix == "vn") {
            // 노멀 벡터
            glm::vec3 normal;
            ss >> normal.x >> normal.y >> normal.z;
            temp_normals.push_back(normal);
        }
        else if (prefix == "f") {
            // 면 정보
            std::string vertexString;
            std::vector<unsigned int> faceIndices;

            while (ss >> vertexString) {
                std::istringstream vertexSS(vertexString);
                std::string indexString;
                unsigned int posIndex = 0, texIndex = 0, normIndex = 0;
                int indexPart = 0;

                while (std::getline(vertexSS, indexString, '/')) {
                    if (!indexString.empty()) {
                        unsigned int index = std::stoi(indexString);
                        if (indexPart == 0) posIndex = index;
                        else if (indexPart == 1) texIndex = index;
                        else if (indexPart == 2) normIndex = index;
                    }
                    indexPart++;
                }

                VertexIndices vertexIndices = { posIndex - 1, texIndex > 0 ? texIndex - 1 : 0, normIndex > 0 ? normIndex - 1 : 0 };

                if (uniqueVertices.count(vertexIndices) == 0) {
                    Vertex vertex;

                    // 위치
                    if (vertexIndices.positionIndex < temp_positions.size()) {
                        vertex.position = temp_positions[vertexIndices.positionIndex];
                    }
                    else {
                        continue;
                    }

                    // 노멀
                    if (vertexIndices.normalIndex < temp_normals.size()) {
                        vertex.normal = temp_normals[vertexIndices.normalIndex];
                    }
                    else {
                        vertex.normal = glm::vec3(0.0f, 0.0f, 0.0f);
                    }

                    // 컬러 (임의로 설정)
   

                    vertices.push_back(vertex);
                    uniqueVertices[vertexIndices] = (unsigned int)(vertices.size() - 1);
                }

                faceIndices.push_back(uniqueVertices[vertexIndices]);
            }

            // 삼각형 구성
            for (size_t i = 1; i + 1 < faceIndices.size(); ++i) {
                indices.push_back(faceIndices[0]);
                indices.push_back(faceIndices[i]);
                indices.push_back(faceIndices[i + 1]);
            }
        }
    }

    file.close();
    return true;
}