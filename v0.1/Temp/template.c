#include "../Library/loadpng.h"
#include <GL/glut.h>

#define POSX 420
#define POSY 160
#define WIDTH 400
#define HEIGHT 400

#define SIZE 40.0f
#define MAX_X 2
#define MAX_Y 2
Tmp_X = (int)(px + ADD_LEFT) / SIZE - MAX_X;
    Tmp_X2 = (int)(px + ADD_RIGHT) / SIZE - MAX_X;
    Tmp_Y = (py + 50) / SIZE;
    Is_Stop_Jump = 0;
    if (Tmp_Y >= MAX_Y) {
        Tmp = CFY + 1;
        if (Tmp < MAX_FRAG_Y) {
            Tmp_2 = Frag_Map[Tmp][CFX];
            if (Tmp_2 != -1) {
                if (Map[Tmp_2][Tmp_Y - MAX_Y][Tmp_X] && Map[Tmp_2][Tmp_Y - MAX_Y][Tmp_X2]) {
                    py -= SIZE_FULL_Y;
                    CM = Tmp_2;
                    CFY = Tmp;
                } else
                    Is_Stop_Jump = 1;
            } else
                Is_Stop_Jump = 1;
        } else
            Is_Stop_Jump = 1;
    } else {
        Tmp = Tmp_2 = CM;
        if (Tmp_X < 0) {
            Tmp_X += MAX_X;
            Tmp = Frag_Map[CFY][CFX - 1];
        } else if (Tmp_X >= MAX_X) {
            Tmp_X -= MAX_X;
            Tmp = Frag_Map[CFY][CFX + 1];
        }
        if (Tmp_X2 < 0) {
            Tmp_X2 += MAX_X;
            Tmp_2 = Frag_Map[CFY][CFX - 1];
        } else if (Tmp_X2 >= MAX_X) {
            Tmp_X2 -= MAX_X;
            Tmp_2 = Frag_Map[CFY][CFX + 1];
        }
        if (!Map[Tmp][Tmp_Y][Tmp_X] || !Map[Tmp_2][Tmp_Y][Tmp_X2])
            Is_Stop_Jump = 1;
    }
    if (Is_Stop_Jump) {
        Tmp_F = py;
        py = Tmp_Y * SIZE - 55.0f;
        yMap -= (py - Tmp_F);
        Jump_Stt = 1;
        v = 0.0f;
    }
typedef struct Struct_Rect{
	float Left,Right,Bottom,Top;
} Struct_Rect;

typedef struct Struct_List_Rect{
	int Count;
	Struct_Rect Rect[8];
} Struct_List_Rect;

Struct_List_Rect List_Rect[MAX_Y][MAX_X];
int Map_Wall[MAX_Y*2+1][MAX_X*2+1];
int Map_Frag[MAX_Y][MAX_X]={{1,1},{-1,1}};

int Check_Up(int *x, int *y,int*u,int*v){
	if (*y==0) return 1;
	if (*x==*u&&*y-1==*v) return 1;
	return 0;
}

int Check_Down(int *x, int *y,int*u,int*v){
	if (*y+1==MAX_Y) return 1;
	if (*x==*u&&*y+1==*v) return 1;
	return 0;
}

int Check_Left(int *x, int *y,int*u,int*v){
	if (*x==0) return 1;
	if (*x-1==*u&&*y==*v) return 1;
	return 0;
}

int Check_Right(int *x, int *y,int*u,int*v){
	if (*x+1==MAX_X) return 1;
	if (*x+1==*u&&*y==*v) return 1;
	return 0;
}

void Init(){
	int i,j,k,l,tmp;
	int *p;
	tmp=MAX_X*MAX_Y*2+MAX_X+MAX_Y;
	for (i=0;i<MAX_Y;i++){
		for (j=0;j<MAX_X;j++){
			List_Rect[i][j].Count=0;
			p=&Map_Wall[0][1];
			for (l=0;l<tmp;l++){
				*p=0;
				p+=2;
			}
			for (l=0;l<MAX_Y;l++){
				for (k=0;k<MAX_X;k++){
					if (l!=i||k!=j){
						if (Check_Up(&k,&l,&j,&i))
							Map_Wall[2*l][2*k+1]=1;
						if (Check_Down(&k,&l,&j,&i))
							Map_Wall[2*l+2][2*k+1]=1;
						if (Check_Left(&k,&l,&j,&i))
							Map_Wall[2*l+1][2*k]=1;
						if (Check_Right(&k,&l,&j,&i))
							Map_Wall[2*l+1][2*k+2]=1;
					}
				}
			}
		}
	}
	for (i=0;i<MAX_Y*2+1;i++){
		for (j=0;j<MAX_X*2+1;j++){
			if ((i+j)%2==0) printf(" ");
			else printf("%d",Map_Wall[i][j]);
		}
		printf("\n");
	}
}

void initGL() {
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, WIDTH, 0, HEIGHT);
    glViewport(0, 0, WIDTH, HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glClearColor(0.2f, 0.6f, 0.6f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    Init();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
//	glRectf(l, b, r, t);
    glutSwapBuffers();
}

int main(int argc, char** argv) {
//    glutInit(&argc, argv);
//    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
//    glutInitWindowPosition(POSX, POSY);
//    glutInitWindowSize(WIDTH, HEIGHT);
//    glutCreateWindow("Name");
    initGL();
//    glutDisplayFunc(display);
//    glutMainLoop();
    return 0;
}
