#include "main.h"

#if 0
	Welcome to Fragments!
#endif

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowPosition(POS_X, POS_Y);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Fragments");
    Init_Game();
    Main_Menu();
    // glutReshapeFunc(Resize);
    glutTimerFunc(0, Timer, 0);
    glutMainLoop();
    Mix_CloseAudio();
    return 0;
}
