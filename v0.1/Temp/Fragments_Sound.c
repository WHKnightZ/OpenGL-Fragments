#include "../Library/loadpng.h"
#include <SDL2/SDL_mixer.h>
#include <GL/glut.h>

#define WIDTH 640.0f
#define HEIGHT 440.0f
#define POS_X 300
#define POS_Y 160
#define SIZE 40
#define MAX_X 16
#define MAX_Y 10
#define MAX_FRAG_X 2
#define MAX_FRAG_Y 2
#define NUM_FRAG 3
#define TIME_SWITCH_MAP 10
#define TIME_SWITCH_FRAG 10
#define INTERVAL 15
#define MIN_V -38.0f

void FuncMoveLeft();
void FuncMoveRight();
void FuncCollideAbove();
void FuncCollideBelow();

int w, h;

float l, r, b, t, px, py;
float v, g = 1.4f;
int i, j, k;
float Mx = 0.0f;
float xMap, yMap, tmpF;

GLubyte* ImgRun[2][20];
GLubyte* ImgJump[2][2];
GLubyte* ImgStand;
GLubyte* ImgArrow;
GLubyte* ImgDoor;

Mix_Music* Music_BackGround = NULL;
Mix_Chunk* Sound_Jump = NULL;
Mix_Chunk* Sound_Win = NULL;

int Drt = 1, Stt = 0, IsRun = 0, JumpStt = -1, Win = 0;
int Loop20[20];
int tmp, tmp2, tmp3, tmpX, tmpX2, tmpY, tmpY2;
int CM = 0, CFX = 0, CFY = 0, SFX = 0, SFY = 1, SSFX, SSFY;
char SwitchMap = 0, IsStopJump, IsStopFall, IsReDraw;
float SaveXMap, SaveYMap, OffsetXSwitch, OffsetYSwitch, ScaleSwitch = 1.0f, OffsetScaleSwitch, OffsetXFrag, OffsetYFrag, OffsetSaveX, OffsetSaveY;
int TimeSwitchMap = TIME_SWITCH_MAP, TimeSwitchFrag = TIME_SWITCH_FRAG;
int Level = 1;
char str[40];
float Alpha = 0.0f;

GLuint DefDrt[] = { GLUT_KEY_LEFT, GLUT_KEY_RIGHT };

typedef struct Pos {
    float x, y;
} Pos;

char FragMap[MAX_FRAG_Y][MAX_FRAG_X] = { { 0, 1 }, { -1, 2 } };
Pos FragPos[MAX_FRAG_Y][MAX_FRAG_X] = { { { 0.0f, 0.0f }, { 640.0f, 0.0f } }, { { 0.0f, -400.0f }, { 640.0f, -400.0f } } };
Pos FragOffset[MAX_FRAG_Y][MAX_FRAG_X] = { { { 0.0f, 0.0f }, { 0.0f, 0.0f } }, { { 0.0f, 0.0f }, { 0.0f, 0.0f } } };

int Map[NUM_FRAG][MAX_Y][MAX_X];

void (*FuncMove[2])();
void (*FuncCollide[2])();
void SpecialGame(int, int, int);
void SpecialUpGame(int, int, int);
void SpecialSwitch(int, int, int);

int ImportMap()
{
    sprintf(str, "Maps/%02d.txt", Level);
    FILE* f = fopen(str, "r");
    if (f == NULL)
        return 0;
    for (i = 0; i < NUM_FRAG; i++)
        for (j = 0; j < MAX_Y; j++)
            for (k = 0; k < MAX_X; k++)
                fscanf(f, "%d", &Map[i][j][k]);
    fclose(f);
    for (i = 0; i < NUM_FRAG; i++) {
        glNewList(i + 1, GL_COMPILE);
        for (j = 0; j < MAX_Y; j++) {
            for (k = 0; k < MAX_X; k++) {
                if (!Map[i][j][k]) {
                    glBegin(GL_POLYGON);
                    glVertex2f(k * SIZE, j * SIZE);
                    glVertex2f(k * SIZE + SIZE, j * SIZE);
                    glVertex2f(k * SIZE + SIZE, j * SIZE + SIZE);
                    glVertex2f(k * SIZE, j * SIZE + SIZE);
                    glEnd();
                } else if (Map[i][j][k] == 2) {
                    glEnable(GL_TEXTURE_2D);
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 48, 70, 0, GL_RGBA, GL_UNSIGNED_BYTE, ImgDoor);
                    glColor3f(1.0f, 1.0f, 1.0f);
                    glBegin(GL_POLYGON);
                    glTexCoord2f(0.0f, 0.0f);
                    glVertex2f(k * SIZE - 4, j * SIZE);
                    glTexCoord2f(1.0f, 0.0f);
                    glVertex2f(k * SIZE + 4 + SIZE, j * SIZE);
                    glTexCoord2f(1.0f, 1.0f);
                    glVertex2f(k * SIZE + 4 + SIZE, j * SIZE + SIZE + 30);
                    glTexCoord2f(0.0f, 1.0f);
                    glVertex2f(k * SIZE - 4, j * SIZE + SIZE + 30);
                    glEnd();
                    glColor3f(0.0f, 0.0f, 0.0f);
                    glDisable(GL_TEXTURE_2D);
                }
            }
        }
        glEndList();
    }
    return 1;
}

void DisplaySwitch()
{
    for (i = 0; i < MAX_FRAG_Y; i++) {
        for (j = 0; j < MAX_FRAG_X; j++) {
            if (FragMap[i][j] != -1) {
                glLoadIdentity();
                glScalef(ScaleSwitch, ScaleSwitch, 1.0f);
                glTranslatef(xMap + FragPos[i][j].x + FragOffset[i][j].x, yMap + FragPos[i][j].y + FragOffset[i][j].y, 0.0f);
                glCallList(FragMap[i][j] + 1);
            }
        }
    }
    glEnable(GL_TEXTURE_2D);
    if (TimeSwitchMap == TIME_SWITCH_MAP && TimeSwitchFrag == TIME_SWITCH_FRAG) {
        glLoadIdentity();
        glTranslatef((xMap + FragPos[SFY][SFX].x) / 2, (yMap + FragPos[SFY][SFX].y) / 2, 0.0f);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 100, 100, 0, GL_RGBA, GL_UNSIGNED_BYTE, ImgArrow);
        glBegin(GL_POLYGON);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(110, 50);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(210, 50);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(210, 150);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(110, 150);
        glEnd();
    }
    if (JumpStt > -1)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, ImgJump[Drt][JumpStt]);
    else if (IsRun)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, ImgRun[Drt][Stt]);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, ImgStand);
    glLoadIdentity();
    glScalef(ScaleSwitch, ScaleSwitch, 1.0f);
    glTranslatef(xMap - SaveXMap, yMap - SaveYMap, 0.0f);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(l, b);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(r, b);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(r, t);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(l, t);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void DisplayNoSwitch()
{
    for (i = 0; i < MAX_FRAG_Y; i++) {
        for (j = 0; j < MAX_FRAG_X; j++) {
            if (FragMap[i][j] != -1) {
                glLoadIdentity();
                glTranslatef(xMap + FragPos[i][j].x, yMap + FragPos[i][j].y, 0.0f);
                glCallList(FragMap[i][j] + 1);
            }
        }
    }
    glEnable(GL_TEXTURE_2D);
    if (JumpStt > -1)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, ImgJump[Drt][JumpStt]);
    else if (IsRun)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, ImgRun[Drt][Stt]);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, ImgStand);
    glLoadIdentity();
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(l, b);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(r, b);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(r, t);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(l, t);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    if (Win) {
        glColor4f(0.2f, 0.6f, 0.8f, Alpha);
        glTranslatef(250, 200, 0);
        glScalef(0.6f, 0.6f, 1.0f);
        glLineWidth(8.0f);
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, 87);
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, 73);
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, 78);
        glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
    }
}

void Display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    if (SwitchMap)
        DisplaySwitch();
    else
        DisplayNoSwitch();
    glutSwapBuffers();
}

void Resize()
{
    glutPositionWindow(POS_X, POS_Y);
    glutReshapeWindow(WIDTH, HEIGHT);
}

void Init()
{
	// SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != -1)
	{
		Music_BackGround = Mix_LoadMUS("Sounds/BackGround.mp3");
		Sound_Jump=Mix_LoadWAV("Sounds/Jump.wav");
		Sound_Win=Mix_LoadWAV("Sounds/Win.wav");
		if (!Mix_PlayingMusic())
			Mix_PlayMusic(Music_BackGround, -1);
	}
	// OpenGL
    glClearColor(0.8f, 0.9f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, WIDTH, 0.0, HEIGHT);
    glViewport(0, 0, WIDTH, HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    for (i = 0; i < 20; i++) {
        sprintf(str, "Images/RL%02d.png", i);
        loadPngSwap(&ImgRun[0][i], &w, &h, str);
        sprintf(str, "Images/RR%02d.png", i);
        loadPngSwap(&ImgRun[1][i], &w, &h, str);
    }
    loadPngSwap(&ImgJump[0][0], &w, &h, "Images/JL.png");
    loadPngSwap(&ImgJump[0][1], &w, &h, "Images/FL.png");
    loadPngSwap(&ImgJump[1][0], &w, &h, "Images/JR.png");
    loadPngSwap(&ImgJump[1][1], &w, &h, "Images/FR.png");
    loadPngSwap(&ImgStand, &w, &h, "Images/Stand.png");
    loadPngSwap(&ImgArrow, &tmp, &tmp2, "Images/Arrow.png");
    loadPngSwap(&ImgDoor, &tmp, &tmp2, "Images/Door.png");
    px = 200.0f;
    py = 160.0f;
    xMap = WIDTH / 2 - px;
    l = WIDTH / 2 - 30.0f;
    r = l + 60.0f;
    b = 120.0f;
    yMap = b - py;
    t = b + 60.0f;
    for (i = 0; i < 19; i++)
        Loop20[i] = i + 1;
    Loop20[19] = 0;
    glColor3f(0.0f, 0.0f, 0.0f);
    ImportMap();
    FuncMove[0] = &FuncMoveLeft;
    FuncMove[1] = &FuncMoveRight;
    FuncCollide[0] = &FuncCollideAbove;
    FuncCollide[1] = &FuncCollideBelow;
    JumpStt = 1;
}

void FuncMoveLeft()
{
    tmpX = (int)(px + 612) / SIZE - 16;
    tmpY = (py + 10) / SIZE;
    tmpY2 = (py + 50) / SIZE;
    if (tmpX <= -1) {
        tmp = CFX - 1;
        if (tmp > -1) {
            tmp2 = FragMap[CFY][tmp];
            if (tmp2 != -1) {
                tmpX += MAX_X;
                if (Map[tmp2][tmpY][tmpX] && Map[tmp2][tmpY2][tmpX]) {
                    xMap -= Mx;
                    px += Mx + 640;
                    CM = tmp2;
                    CFX = tmp;
                }
            }
        }
    } else if (Map[CM][tmpY][tmpX] && Map[CM][tmpY2][tmpX]) {
        xMap -= Mx;
        px += Mx;
        tmp = CM;
        tmp2 = (px + 10) / SIZE;
        if (tmp2 >= MAX_X) {
            tmp2 -= MAX_X;
            tmp = FragMap[CFY][CFX + 1];
        }
        if (JumpStt == -1 && tmpY > 0 && Map[tmp][tmpY - 1][tmp2]) {
            JumpStt = 1;
            v = 0;
        }
    }
}

void FuncMoveRight()
{
    tmpX = (px + 28) / SIZE;
    tmpY = (py + 10) / SIZE;
    tmpY2 = (py + 50) / SIZE;
    if (tmpX >= MAX_X) {
        tmp = CFX + 1;
        if (tmp < MAX_FRAG_X) {
            tmp2 = FragMap[CFY][tmp];
            if (tmp2 != -1) {
                tmpX -= MAX_X;
                if (Map[tmp2][tmpY][tmpX] && Map[tmp2][tmpY2][tmpX]) {
                    xMap -= Mx;
                    px += Mx - 640;
                    CM = tmp2;
                    CFX = tmp;
                }
            }
        }
    } else if (Map[CM][tmpY][tmpX] && Map[CM][tmpY2][tmpX]) {
        xMap -= Mx;
        px += Mx;
        tmp = CM;
        tmp2 = (int)(px + 630) / SIZE - 16;
        if (tmp2 < 0) {
            tmp2 += MAX_X;
            tmp = FragMap[CFY][CFX - 1];
        }
        if (JumpStt == -1 && tmpY > 0 && Map[tmp][tmpY - 1][tmp2]) {
            JumpStt = 1;
            v = 0;
        }
    }
}

void FuncCollideAbove()
{
    tmpX = (int)(px + 630) / SIZE - 16;
    tmpX2 = (int)(px + 650) / SIZE - 16;
    tmpY = (py + 50) / SIZE;
    IsStopJump = 0;
    if (tmpY >= MAX_Y) {
        tmp = CFY - 1;
        if (tmp > -1) {
            tmp2 = FragMap[tmp][CFX];
            if (tmp2 != -1) {
                if (Map[tmp2][tmpY - MAX_Y][tmpX] && Map[tmp2][tmpY - MAX_Y][tmpX2]) {
                    py -= 400.0f;
                    CM = tmp2;
                    CFY = tmp;
                } else
                    IsStopJump = 1;
            } else
                IsStopJump = 1;
        } else
            IsStopJump = 1;
    } else {
        tmp = tmp2 = CM;
        if (tmpX < 0) {
            tmpX += MAX_X;
            tmp = FragMap[CFY][CFX - 1];
        } else if (tmpX >= MAX_X) {
            tmpX -= MAX_X;
            tmp = FragMap[CFY][CFX + 1];
        }
        if (tmpX2 < 0) {
            tmpX2 += MAX_X;
            tmp2 = FragMap[CFY][CFX - 1];
        } else if (tmpX2 >= MAX_X) {
            tmpX2 -= MAX_X;
            tmp2 = FragMap[CFY][CFX + 1];
        }
        if (!Map[tmp][tmpY][tmpX] || !Map[tmp2][tmpY][tmpX2])
            IsStopJump = 1;
    }
    if (IsStopJump) {
        tmpF = py;
        py = tmpY * SIZE - 55.0f;
        yMap -= (py - tmpF);
        JumpStt = 1;
        v = 0;
    }
}

void FuncCollideBelow()
{
    tmpX = (int)(px + 630) / SIZE - 16;
    tmpX2 = (int)(px + 650) / SIZE - 16;
    tmpY = (int)(py + 400) / SIZE - 10;
    IsStopFall = 0;
    if (tmpY < 0) {
        tmp = CFY + 1;
        if (tmp < MAX_FRAG_Y) {
            tmp2 = FragMap[tmp][CFX];
            if (tmp2 != -1) {
                if (Map[tmp2][tmpY + MAX_Y][tmpX] && Map[tmp2][tmpY + MAX_Y][tmpX2]) {
                    py += 400.0f;
                    CM = tmp2;
                    CFY = tmp;
                } else
                    IsStopFall = 1;
            } else
                IsStopFall = 1;
        } else
            IsStopFall = 1;
    } else {
        tmp = tmp2 = CM;
        if (tmpX < 0) {
            tmpX += MAX_X;
            if (CFX > 0) {
                tmp = FragMap[CFY][CFX - 1];
                if (tmp == -1)
                    IsStopFall = 1;
            } else
                IsStopFall = 1;
        } else if (tmpX >= MAX_X) {
            tmpX -= MAX_X;
            if (CFX < MAX_FRAG_X - 1) {
                tmp = FragMap[CFY][CFX + 1];
                if (tmp == -1)
                    IsStopFall = 1;
            } else
                IsStopFall = 1;
        }
        if (tmpX2 < 0) {
            tmpX2 += MAX_X;
            if (CFX > 0) {
                tmp2 = FragMap[CFY][CFX - 1];
                if (tmp2 == -1)
                    IsStopFall = 1;
            } else
                IsStopFall = 1;
        } else if (tmpX2 >= MAX_X) {
            tmpX2 -= MAX_X;
            if (CFX < MAX_FRAG_X - 1) {
                tmp2 = FragMap[CFY][CFX + 1];
                if (tmp2 == -1)
                    IsStopFall = 1;
            } else
                IsStopFall = 1;
        }
        if (!Map[tmp][tmpY][tmpX] || !Map[tmp2][tmpY][tmpX2])
            IsStopFall = 1;
    }
    if (IsStopFall) {
        tmpF = py;
        py = (tmpY + 1) * SIZE;
        yMap -= (py - tmpF);
        JumpStt = -1;
    }
}

void Timer(int value)
{
    IsReDraw = 0;
    if (TimeSwitchMap < TIME_SWITCH_MAP) {
        TimeSwitchMap++;
        if (TimeSwitchMap == TIME_SWITCH_MAP) {
            if (SwitchMap == -1) {
                SwitchMap = 0;
                if (py < 1.0f) {
                    tmp = CFY + 1;
                    if (tmp < MAX_FRAG_Y) {
                        tmp2 = FragMap[tmp][CFX];
                        if (tmp2 != -1) {
                            tmpX = (int)(px + 630) / SIZE - 16;
                            tmpX2 = (int)(px + 650) / SIZE - 16;
                            if (Map[tmp2][9][tmpX] && Map[tmp2][9][tmpX2]) {
                                v = 0;
                                JumpStt = 2;
                            }
                        }
                    }
                }
                glutSpecialFunc(SpecialGame);
                glutSpecialUpFunc(SpecialUpGame);
            } else {
                glutSpecialFunc(SpecialSwitch);
            }
        }
        xMap += OffsetXSwitch;
        yMap += OffsetYSwitch;
        ScaleSwitch += OffsetScaleSwitch;
        IsReDraw = 1;
    }
    if (!SwitchMap) {
        if (IsRun) {
            FuncMove[Drt]();
            Stt = Loop20[Stt];
            IsReDraw = 1;
        }
        if (JumpStt > -1) {
            py += v;
            yMap -= v;
            v -= g;
            if (v < MIN_V)
                v = MIN_V;
            if (v < 0)
                JumpStt = 1;
            FuncCollide[JumpStt]();
            IsReDraw = 1;
        }
        if (!Win && Map[CM][(int)(py / SIZE)][(int)(px / SIZE)] == 2) {
        	if (!Mix_Playing(-1))
				Mix_PlayChannel(-1, Sound_Win, 0);
            glEnable(GL_LINE_SMOOTH);
            Win = 1;
            IsRun = 0;
            glutKeyboardFunc(NULL);
            glutSpecialFunc(NULL);
            glutSpecialUpFunc(NULL);
        }
    } else if (TimeSwitchFrag < TIME_SWITCH_FRAG) {
        TimeSwitchFrag++;
        if (TimeSwitchFrag == TIME_SWITCH_FRAG) {
            glutSpecialFunc(SpecialSwitch);
        }
        FragOffset[SSFY][SSFX].x += OffsetXFrag;
        FragOffset[SSFY][SSFX].y += OffsetYFrag;
        SaveXMap += OffsetSaveX;
        SaveYMap += OffsetSaveY;
        IsReDraw = 1;
    }
    if (Win) {
        if (Alpha < 1.0f) {
            Alpha += 0.05f;
            IsReDraw = 1;
        }
    }
    if (IsReDraw)
        glutPostRedisplay();
    glutTimerFunc(INTERVAL, Timer, 0);
}

void Keyboard(unsigned char key, int x, int y)
{
    if (key == 32 && TimeSwitchFrag == TIME_SWITCH_FRAG && TimeSwitchMap == TIME_SWITCH_MAP) {
        if (SwitchMap) {
            SwitchMap = -1;
            OffsetXSwitch = SaveXMap / TIME_SWITCH_MAP;
            OffsetYSwitch = (SaveYMap - 440.0f) / TIME_SWITCH_MAP;
            OffsetScaleSwitch = 0.5f / TIME_SWITCH_MAP;
            TimeSwitchMap = 0;
            glutSpecialFunc(NULL);
        } else {
            SwitchMap = 1;
            IsRun = 0;
            SaveXMap = xMap;
            SaveYMap = yMap;
            OffsetXSwitch = -xMap / TIME_SWITCH_MAP;
            OffsetYSwitch = (440.0f - yMap) / TIME_SWITCH_MAP;
            OffsetScaleSwitch = -0.5f / TIME_SWITCH_MAP;
            TimeSwitchMap = 0;
            glutSpecialFunc(NULL);
            glutSpecialUpFunc(NULL);
        }
    }
}

void SpecialGame(int key, int x, int y)
{
    switch (key) {
    case GLUT_KEY_LEFT:
        IsRun = 1;
        Drt = 0;
        Mx = -5.0f;
        break;
    case GLUT_KEY_RIGHT:
        IsRun = 1;
        Drt = 1;
        Mx = 5.0f;
        break;
    case GLUT_KEY_UP:
        if (JumpStt == -1) {
            JumpStt = 0;
            v = 19.0f;
            if (!Mix_Playing(-1))
				Mix_PlayChannel(-1, Sound_Jump, 0);
        }
        break;
    }
}

void SpecialUpGame(int key, int x, int y)
{
    if (key == DefDrt[Drt]) {
        Mx = 0.0f;
        IsRun = 0;
        glutPostRedisplay();
    }
}

void SpecialSwitch(int key, int x, int y)
{
    switch (key) {
    case GLUT_KEY_LEFT:
        if (SFX > 0) {
            TimeSwitchFrag = 0;
            glutSpecialFunc(NULL);
            SSFX = SFX;
            SSFY = SFY;
            FragOffset[SFY][SFX].x = -640.0f;
            FragOffset[SFY][SFX].y = 0.0f;
            OffsetXFrag = 640.0f / TIME_SWITCH_FRAG;
            OffsetYFrag = 0.0f;
            FragMap[SFY][SFX] = FragMap[SFY][SFX - 1];
            SFX--;
            FragMap[SFY][SFX] = -1;
            if (SFX == CFX && SFY == CFY) {
                OffsetSaveX = -640.0f / TIME_SWITCH_FRAG;
                OffsetSaveY = 0.0f;
                CFX++;
            } else
                OffsetSaveX = OffsetSaveY = 0.0f;
        }
        break;
    case GLUT_KEY_RIGHT:
        if (SFX < MAX_FRAG_X - 1) {
            TimeSwitchFrag = 0;
            glutSpecialFunc(NULL);
            SSFX = SFX;
            SSFY = SFY;
            FragOffset[SFY][SFX].x = 640.0f;
            FragOffset[SFY][SFX].y = 0.0f;
            OffsetXFrag = -640.0f / TIME_SWITCH_FRAG;
            OffsetYFrag = 0.0f;
            FragMap[SFY][SFX] = FragMap[SFY][SFX + 1];
            SFX++;
            FragMap[SFY][SFX] = -1;
            if (SFX == CFX && SFY == CFY) {
                OffsetSaveX = 640.0f / TIME_SWITCH_FRAG;
                OffsetSaveY = 0.0f;
                CFX--;
            } else
                OffsetSaveX = OffsetSaveY = 0.0f;
        }
        break;
    case GLUT_KEY_UP:
        if (SFY > 0) {
            TimeSwitchFrag = 0;
            glutSpecialFunc(NULL);
            SSFX = SFX;
            SSFY = SFY;
            FragOffset[SFY][SFX].x = 0.0f;
            FragOffset[SFY][SFX].y = 400.0f;
            OffsetXFrag = 0.0f;
            OffsetYFrag = -400.0f / TIME_SWITCH_FRAG;
            FragMap[SFY][SFX] = FragMap[SFY - 1][SFX];
            SFY--;
            FragMap[SFY][SFX] = -1;
            if (SFX == CFX && SFY == CFY) {
                OffsetSaveX = 0.0f;
                OffsetSaveY = 400.0f / TIME_SWITCH_FRAG;
                CFY++;
            } else
                OffsetSaveX = OffsetSaveY = 0.0f;
        }
        break;
    case GLUT_KEY_DOWN:
        if (SFY < MAX_FRAG_Y - 1) {
            TimeSwitchFrag = 0;
            glutSpecialFunc(NULL);
            SSFX = SFX;
            SSFY = SFY;
            FragOffset[SFY][SFX].x = 0.0f;
            FragOffset[SFY][SFX].y = -400.0f;
            OffsetXFrag = 0.0f;
            OffsetYFrag = 400.0f / TIME_SWITCH_FRAG;
            FragMap[SFY][SFX] = FragMap[SFY + 1][SFX];
            SFY++;
            FragMap[SFY][SFX] = -1;
            if (SFX == CFX && SFY == CFY) {
                OffsetSaveX = 0.0f;
                OffsetSaveY = -400.0f / TIME_SWITCH_FRAG;
                CFY--;
            } else
                OffsetSaveX = OffsetSaveY = 0.0f;
        }
        break;
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowPosition(POS_X, POS_Y);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Fragments");
    Init();
    glutTimerFunc(0, Timer, 0);
    glutKeyboardFunc(Keyboard);
    glutSpecialFunc(SpecialGame);
    glutSpecialUpFunc(SpecialUpGame);
    glutDisplayFunc(Display);
    glutReshapeFunc(Resize);
    glutMainLoop();
    Mix_CloseAudio();
    return 0;
}
