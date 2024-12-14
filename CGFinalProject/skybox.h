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
    // ������: ��ī�̹ڽ� �ؽ�ó ���� ��θ� ���ͷ� ����
    Skybox(const std::vector<std::string>& faces);
    ~Skybox();

    // ��ī�̹ڽ� ������ �Լ�
    void Draw(const glm::mat4& view, const glm::mat4& projection);

private:
    unsigned int skyboxVAO, skyboxVBO;
    unsigned int cubemapTexture;
    Shader* skyboxShader;

    // ť��� �ؽ�ó �ε� �Լ�
    unsigned int LoadCubemap(const std::vector<std::string>& faces);

    // ��ī�̹ڽ� ������ �����͸� �ʱ�ȭ�ϴ� �Լ�
    void InitRenderData();
};

#endif
