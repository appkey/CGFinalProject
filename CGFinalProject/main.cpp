#include <GL/glew.h>
#include <GL/freeglut.h>
#include "game.h"

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    // 필요한 GLUT 옵션 설정...
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1600, 900);
    glutCreateWindow("3D Game");

    // GLEW 초기화
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "GLEW 초기화 실패: %s\n", glewGetErrorString(err));
        return -1;
    }
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    // 나머지 초기화...
    Game game;
    game.Run();
    return 0;
}