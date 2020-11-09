#include <GL/glut.h>

#define MAXX 80
#define MAXY 60
#define SIZE 10

int WIDTH=MAXX*SIZE;
int HEIGHT=MAXY*SIZE;

float offset = 0, offset2 = 0;
float l, r, b, t;
float yGround = 80.0f;
int isJump = 0, isFall=0;
float v, g = 1.4f;
int i, j,k, tmpH;
int Mx=0;
char Map[MAXY][MAXX];

typedef struct Rect{
	int x,y,x2,y2;
} Rect;

typedef struct Quad{
	float l,r,b,t;
} Quad;

Rect rect[6]={{0,0,80,4},{20,13,40,14},{60,13,70,14},{40,22,55,23},{50,17,60,18},{20,34,45,35}};
Quad quad[6];
int countRect=6;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glBegin(GL_QUADS);
    for (i=0;i<countRect;i++){
    	glVertex2f(quad[i].l,quad[i].b);
	    glVertex2f(quad[i].r,quad[i].b);
	    glVertex2f(quad[i].r,quad[i].t);
	    glVertex2f(quad[i].l,quad[i].t);
	}
    glVertex2f(l,b);
    glVertex2f(r,b);
    glVertex2f(r,t);
    glVertex2f(l,t);
    glEnd();
    glutSwapBuffers();
}

void init()
{
    glClearColor(0.2f, 0.4f, 0.4f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, WIDTH, 0.0, HEIGHT);
    glViewport(0, 0, WIDTH, HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    for (i=0;i<MAXY;i++){
    	for (j=0;j<MAXX;j++) Map[i][j]=0;
	}
	for (i=0;i<countRect;i++){
		quad[i].l=rect[i].x*SIZE;
		quad[i].r=rect[i].x2*SIZE;
		quad[i].b=rect[i].y*SIZE;
		quad[i].t=rect[i].y2*SIZE;
		for (j=rect[i].y;j<rect[i].y2;j++){
			for (k=rect[i].x;k<rect[i].x2;k++){
				Map[j][k]=1;
			}
		}
	}
    l = 100;
    r = l + 40;
    b = yGround;
    t = b + 40;
}

int tmpX, tmpY, tmpY2;
void timer(int value)
{
	tmpY=(b+v)/SIZE;
	tmpY2=(b-1)/SIZE;
	tmpX=l/SIZE;
	for (i=tmpY2;i>=tmpY;i--){
		if (Map[i][tmpX]||Map[i][tmpX+1]||Map[i][tmpX+2]||Map[i][tmpX+3]){
			break;
		}
	}
	if (i>=tmpY) yGround=(i+1)*SIZE;
	else yGround=b-40;
	if (Mx!=0){
		l+=Mx;
		r+=Mx;
	}
    if (isJump) {
        t += v;
        b += v;
        v -= g;
        if (v<0) isFall=1;
        if (isFall){
        	if (b < yGround) {
	            isJump = 0;
	            b = yGround;
	            t = b + 40;
	            isFall=0;
	        }
		} 
    } else {
        if (b > yGround) {
        	isJump=1;
            isFall = 1;
            v = 0;
        }
    }
    glutPostRedisplay();
    glutTimerFunc(20, timer, 0);
}

void keyboard(unsigned char key, int x, int y)
{
    if (!isJump) {
        isJump = 1;
        v = 18.0f;
    }
}

void special(int key, int x, int y){
	switch(key){
		case GLUT_KEY_LEFT: Mx=-5; break;
		case GLUT_KEY_RIGHT: Mx=5; break;
	}
}

void specialUp(int key, int x, int y){
	switch(key){
		case GLUT_KEY_LEFT: if(Mx==-5)Mx=0; break;
		case GLUT_KEY_RIGHT: if(Mx==5) Mx=0; break;
	}
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowPosition(150, 40);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("BG Loop");
    init();
    glutTimerFunc(0, timer, 0);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutSpecialUpFunc(specialUp);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
