// skybox.h
#ifndef SKYBOX_H
#define SKYBOX_H

#include <vector>
#include <string>
#include <GL/glew.h>
#include <gl/glm/glm.hpp>
#include "shader.h"

class Skybox {
public:
    // 생성자: 스카이박스 텍스처 파일 경로를 벡터로 전달
    Skybox(const std::vector<std::string>& faces);
    ~Skybox();

    // 스카이박스 렌더링 함수
    void Draw(const glm::mat4& view, const glm::mat4& projection);

private:
    unsigned int skyboxVAO, skyboxVBO;
    unsigned int cubemapTexture;
    Shader* skyboxShader;

    // 큐브맵 텍스처 로드 함수
    unsigned int LoadCubemap(const std::vector<std::string>& faces);

    // 스카이박스 렌더링 데이터를 초기화하는 함수
    void InitRenderData();
};

#endif
