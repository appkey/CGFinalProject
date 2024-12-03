#include <GL/glew.h>
#include <GL/freeglut.h>
#include "game.h"

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    // �ʿ��� GLUT �ɼ� ����...
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1600, 900);
    glutCreateWindow("3D Game");

    // GLEW �ʱ�ȭ
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "GLEW �ʱ�ȭ ����: %s\n", glewGetErrorString(err));
        return -1;
    }
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    // ������ �ʱ�ȭ...
    Game game;
    game.Run();
    return 0;
}