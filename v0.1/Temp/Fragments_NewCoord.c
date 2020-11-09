#include "../Library/loadpng.h"
#include <GL/glut.h>
#include <SDL2/SDL_mixer.h>

#define WIDTH 640.0f
#define HEIGHT 440.0f
#define POS_X 300
#define POS_Y 160
#define SIZE 40
#define MAX_X 16
#define MAX_Y 11
#define LAST_X 15 // MAX_X - 1
#define LAST_Y 10 // MAX_Y - 1
#define SIZE_FULL_X 640.0f // MAX_X * SIZE
#define SIZE_FULL_Y 440.0f // MAX_Y * SIZE
#define MAX_FRAG_X 2
#define MAX_FRAG_Y 2
#define NUM_FRAG 3
#define TIME_SWITCH_MAP 10
#define TIME_SWITCH_FRAG 10
#define INTERVAL 15
#define BASE_V 19.0f
#define MIN_V -38.0f
#define SIZE_VIRTUAL_WALL 10.0f
#define START_NUM_STAGE_X 28.0f
#define START_NUM_STAGE_Y -80.0f

float ADD_LEFT = SIZE_FULL_X - 10.0f, ADD_RIGHT = SIZE_FULL_X + 10.0f, ADD_LEFT_2 = SIZE_FULL_X - 28.0f;

typedef struct Struct_Pos {
    float x, y;
} Struct_Pos;

typedef struct Struct_Rect {
    float Left, Right, Bottom, Top;
} Struct_Rect;

typedef struct Struct_List_Rect {
    int Count;
    Struct_Rect Rect[8];
} Struct_List_Rect;

typedef struct Image {
    GLubyte* img;
    GLuint w, h;
} Image;

struct Number {
    int Current, Length, Size, Max;
    int Array[2];
    float Start, Left, Right, Bottom, Top;
} Num_Stage;

struct Arrow {
    int Drt[2];
    int Width, Height, Stt, Can_Draw[2];
    int Loop[20];
    float Offset[20];
    float Left[2], Bottom, Top;
} Main_Arrow;

void (*Func_Draw_Path[3])(int, int);
void (*Func_Move[2])();
void (*Func_Collide[2])();
void (*Func_Timer)();
int (*Func_Check_Nothing[4])(int*, int*, int*, int*);
int Import_Map();
void Display_Menu();
void Display_Game();
void Special_Menu(int, int, int);
void Special_Game(int, int, int);
void Special_Up_Game(int, int, int);
void Special_Switch(int, int, int);
void Keyboard_Menu(unsigned char, int, int);
void Keyboard_Game(unsigned char, int, int);
void Move_Left();
void Move_Right();
void Collide_Above();
void Collide_Below();
void Timer_Menu();
void Timer_Game();

Struct_List_Rect List_Virtual_Wall[MAX_FRAG_Y][MAX_FRAG_X];
Struct_Rect Rect_Virtual_Wall[4] = {
    { -SIZE_VIRTUAL_WALL, SIZE_FULL_X + SIZE_VIRTUAL_WALL, -SIZE_VIRTUAL_WALL, 0.0f },
    { -SIZE_VIRTUAL_WALL, SIZE_FULL_X + SIZE_VIRTUAL_WALL, SIZE_FULL_Y, SIZE_FULL_Y + SIZE_VIRTUAL_WALL },
    { -SIZE_VIRTUAL_WALL, 0.0f, 0.0f, SIZE_FULL_Y },
    { SIZE_FULL_X, SIZE_FULL_X + SIZE_VIRTUAL_WALL, 0.0f, SIZE_FULL_Y }
};
Struct_Rect* Ptr_Rect;
Struct_List_Rect* Ptr_List_Rect;
Struct_Pos Frag_Pos[MAX_FRAG_Y][MAX_FRAG_X];
Struct_Pos Frag_Offset[MAX_FRAG_Y][MAX_FRAG_X];

Image Img_Num[10];

GLubyte* Img_Main;
GLubyte* Img_Main_Arrow[2];
GLubyte* Img_Run[2][20];
GLubyte* Img_Jump[2][2];
GLubyte* Img_Stand;
GLubyte* Img_Arrow;
GLubyte* Img_Door;

Mix_Music* Music_BackGround = NULL;
Mix_Chunk* Sound_Jump = NULL;
Mix_Chunk* Sound_Win = NULL;

GLuint Def_Drt[] = { GLUT_KEY_LEFT, GLUT_KEY_RIGHT };

int Tmp_L, Tmp_R;
int w, h, i, j, k;
float l, r, b, t, px, py;
float v, g = 1.4f;
float Mx = 0.0f, xMap, yMap, Tmp_F;
int Drt = 1, Stt = 0, IsRun = 0, Jump_Stt = -1, Win = 0;
int Loop20[20];
int Tmp, Tmp_2, Tmp_3, Tmp_X, Tmp_X2, Tmp_Y, Tmp_Y2;
int CM, CFX = 0, CFY = 0, SFX = 0, SFY = 1, SSFX, SSFY;
char Switch_Map = 0, Is_Stop_Jump, Is_Stop_Fall, Is_Redraw;
float Save_Map_X, Save_Map_Y, Offset_Switch_X, Offset_Switch_Y, Offset_View_Switch_X, Offset_View_Switch_Y;
float Offset_Frag_X, Offset_Frag_Y, Offset_Save_X, Offset_Save_Y;
int Time_Switch_Map = TIME_SWITCH_MAP, Time_Switch_Frag = TIME_SWITCH_FRAG;
char Str[40];
float Alpha = 0.0f;
char Frag_Map[MAX_FRAG_Y][MAX_FRAG_X] = { { 0, 1 }, { -1, 2 } };
int Map[NUM_FRAG][MAX_Y][MAX_X];
float Current_Width, Current_Height;

void Init_Main_Arrow()
{
    Main_Arrow.Can_Draw[0] = 0;
    if (Num_Stage.Max > 1)
        Main_Arrow.Can_Draw[1] = 1;
    Main_Arrow.Stt = 0;
    Main_Arrow.Drt[0] = -1;
    Main_Arrow.Drt[1] = 1;
    float s = 0.0f, v = 0.0f, a = 0.2f;
    for (i = 0; i <= 10; i++) {
        Main_Arrow.Offset[i] = (int)s;
        v += a;
        s += v;
    }
    for (i = 11; i < 20; i++) {
        Main_Arrow.Offset[i] = Main_Arrow.Offset[20 - i];
    }
    for (i = 0; i < 19; i++)
        Main_Arrow.Loop[i] = i + 1;
    Main_Arrow.Loop[19] = 0;
    Main_Arrow.Left[0] = (int)WIDTH / 2 + START_NUM_STAGE_X - 46.0f - Main_Arrow.Width;
    Main_Arrow.Left[1] = (int)WIDTH / 2 + START_NUM_STAGE_X + 46.0f;
    Main_Arrow.Bottom = (int)((HEIGHT - Main_Arrow.Height) / 2 + START_NUM_STAGE_Y);
    Main_Arrow.Top = Main_Arrow.Bottom + Main_Arrow.Height;
}

void Change_Num_Stage()
{
    int n = 0;
    int x = Num_Stage.Current;
    Num_Stage.Size = 0.0f;
    do {
        Num_Stage.Array[n] = x % 10;
        Num_Stage.Size += Img_Num[Num_Stage.Array[n]].w;
        x /= 10;
        n++;
    } while (x > 0);
    Num_Stage.Start = (int)((WIDTH - Num_Stage.Size) / 2 + START_NUM_STAGE_X);
    Num_Stage.Length = n - 1;
}

void Init_Num_Stage()
{
    Num_Stage.Max = 1;
    FILE* f;
    while (1) {
        sprintf(Str, "Maps/%02d.txt", Num_Stage.Max);
        f = fopen(Str, "r");
        if (f == NULL)
            break;
        fclose(f);
        Num_Stage.Max++;
    }
    Num_Stage.Max--;
    Num_Stage.Current = 1;
    Num_Stage.Bottom = (int)((HEIGHT - Img_Num[0].h) / 2 + START_NUM_STAGE_Y);
    Num_Stage.Top = Num_Stage.Bottom + Img_Num[0].h;
    Change_Num_Stage();
}

void Main_Menu()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    Func_Timer = &Timer_Menu;
    glutSpecialFunc(Special_Menu);
    glutSpecialUpFunc(NULL);
    glutKeyboardFunc(Keyboard_Menu);
    glutDisplayFunc(Display_Menu);
}

void In_Game()
{
    Import_Map();
    glDisable(GL_TEXTURE_2D);
    px = 200.0f;
    py = 160.0f;
    l = WIDTH / 2 - 30.0f;
    r = l + 60.0f;
    b = 120.0f;
    t = b + 60.0f;
    xMap = WIDTH / 2 - px - CFX * SIZE_FULL_X;
    yMap = b - py - CFY * SIZE_FULL_Y;
    CM = Frag_Map[CFY][CFX];
    Jump_Stt = 1;
    Func_Timer = &Timer_Game;
    glutSpecialFunc(Special_Game);
    glutSpecialUpFunc(Special_Up_Game);
    glutKeyboardFunc(Keyboard_Game);
    glutDisplayFunc(Display_Game);
}

void Draw_Path_Wall(int x, int y)
{
    glBegin(GL_POLYGON);
    glVertex2f(x * SIZE, y * SIZE);
    glVertex2f(x * SIZE + SIZE, y * SIZE);
    glVertex2f(x * SIZE + SIZE, y * SIZE + SIZE);
    glVertex2f(x * SIZE, y * SIZE + SIZE);
    glEnd();
}

void Draw_Path_Nothing(int x, int y) {}

void Draw_Path_Door(int x, int y)
{
    glEnable(GL_TEXTURE_2D);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 48, 70, 0, GL_RGBA, GL_UNSIGNED_BYTE, Img_Door);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(x * SIZE - 4, y * SIZE);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(x * SIZE + 4 + SIZE, y * SIZE);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(x * SIZE + 4 + SIZE, y * SIZE + SIZE + 30);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(x * SIZE - 4, y * SIZE + SIZE + 30);
    glEnd();
    glColor3f(0.0f, 0.0f, 0.0f);
    glDisable(GL_TEXTURE_2D);
}

int Import_Map()
{
    sprintf(Str, "Maps/%02d.txt", Num_Stage.Current);
    FILE* f = fopen(Str, "r");
    if (f == NULL)
        return 0;
    for (i = 0; i < NUM_FRAG; i++)
        for (j = LAST_Y; j >= 0; j--)
            for (k = 0; k < MAX_X; k++)
                fscanf(f, "%d", &Map[i][j][k]);
    fclose(f);
    for (i = 0; i < NUM_FRAG; i++) {
        glNewList(i + 1, GL_COMPILE);
        for (j = 0; j < MAX_Y; j++) {
            for (k = 0; k < MAX_X; k++) {
                Func_Draw_Path[Map[i][j][k]](k, j);
            }
        }
        glEndList();
    }
    return 1;
}

int Check_Nothing_Up(int* x, int* y, int* u, int* v)
{
    if (*y == 0)
        return 1;
    if (*x == *u && *y - 1 == *v)
        return 1;
    return 0;
}

int Check_Nothing_Down(int* x, int* y, int* u, int* v)
{
    if (*y + 1 == MAX_FRAG_Y)
        return 1;
    if (*x == *u && *y + 1 == *v)
        return 1;
    return 0;
}

int Check_Nothing_Left(int* x, int* y, int* u, int* v)
{
    if (*x == 0)
        return 1;
    if (*x - 1 == *u && *y == *v)
        return 1;
    return 0;
}

int Check_Nothing_Right(int* x, int* y, int* u, int* v)
{
    if (*x + 1 == MAX_FRAG_X)
        return 1;
    if (*x + 1 == *u && *y == *v)
        return 1;
    return 0;
}

void Init_Frag_Offset()
{
    for (i = 0; i < MAX_FRAG_Y; i++)
        for (j = 0; j < MAX_FRAG_X; j++) {
            Frag_Pos[i][j].x = j * SIZE_FULL_X;
            Frag_Pos[i][j].y = i * SIZE_FULL_Y;
            Frag_Offset[i][j].x = 0.0f;
            Frag_Offset[i][j].y = 0.0f;
        }
}

void Set_Virtual_Wall(int* x, int* y, int* u, int* v, int Drt)
{
    if (Func_Check_Nothing[Drt](x, y, u, v)) {
        Ptr_List_Rect->Rect[Ptr_List_Rect->Count].Left = *x * SIZE_FULL_X + Rect_Virtual_Wall[Drt].Left;
        Ptr_List_Rect->Rect[Ptr_List_Rect->Count].Right = *x * SIZE_FULL_X + Rect_Virtual_Wall[Drt].Right;
        Ptr_List_Rect->Rect[Ptr_List_Rect->Count].Bottom = *y * SIZE_FULL_Y + Rect_Virtual_Wall[Drt].Bottom;
        Ptr_List_Rect->Rect[Ptr_List_Rect->Count].Top = *y * SIZE_FULL_Y + Rect_Virtual_Wall[Drt].Top;
        Ptr_List_Rect->Count++;
    }
}

void Init_Virtual_Wall()
{
    int i, j, k, l, n;
    for (i = 0; i < MAX_FRAG_Y; i++) {
        for (j = 0; j < MAX_FRAG_X; j++) {
            Ptr_List_Rect = &List_Virtual_Wall[i][j];
            Ptr_List_Rect->Count = 0;
            for (k = 0; k < MAX_FRAG_Y; k++) {
                for (l = 0; l < MAX_FRAG_X; l++) {
                    if (l != j || k != i) {
                        for (n = 0; n < 4; n++)
                            Set_Virtual_Wall(&l, &k, &j, &i, n);
                    }
                }
            }
        }
    }
}

void Display_Menu()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 640, 440, 0, GL_RGBA, GL_UNSIGNED_BYTE, Img_Main);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(640.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(640.0f, 440.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(0.0f, 440.0f);
    glEnd();
    Num_Stage.Right = Num_Stage.Start;
    for (i = Num_Stage.Length; i >= 0; i--) {
        Num_Stage.Left = Num_Stage.Right;
        Num_Stage.Right = Num_Stage.Left + Img_Num[Num_Stage.Array[i]].w;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Img_Num[Num_Stage.Array[i]].w, Img_Num[Num_Stage.Array[i]].h, 0, GL_RGBA, GL_UNSIGNED_BYTE, Img_Num[Num_Stage.Array[i]].img);
        glBegin(GL_POLYGON);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(Num_Stage.Left, Num_Stage.Bottom);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(Num_Stage.Right, Num_Stage.Bottom);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(Num_Stage.Right, Num_Stage.Top);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(Num_Stage.Left, Num_Stage.Top);
        glEnd();
    }
    for (i = 0; i < 2; i++) {
        if (Main_Arrow.Can_Draw[i]) {
            Tmp_L = Main_Arrow.Left[i] + Main_Arrow.Offset[Main_Arrow.Stt] * Main_Arrow.Drt[i];
            Tmp_R = Tmp_L + Main_Arrow.Width;
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Main_Arrow.Width, Main_Arrow.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Img_Main_Arrow[i]);
            glBegin(GL_POLYGON);
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(Tmp_L, Main_Arrow.Bottom);
            glTexCoord2f(1.0f, 0.0f);
            glVertex2f(Tmp_R, Main_Arrow.Bottom);
            glTexCoord2f(1.0f, 1.0f);
            glVertex2f(Tmp_R, Main_Arrow.Top);
            glTexCoord2f(0.0f, 1.0f);
            glVertex2f(Tmp_L, Main_Arrow.Top);
            glEnd();
        }
    }
    glutSwapBuffers();
}

void Display_Switch()
{
    for (i = 0; i < MAX_FRAG_Y; i++) {
        for (j = 0; j < MAX_FRAG_X; j++) {
            if (Frag_Map[i][j] != -1) {
                glLoadIdentity();
                glTranslatef(xMap + Frag_Pos[i][j].x + Frag_Offset[i][j].x, yMap + Frag_Pos[i][j].y + Frag_Offset[i][j].y, 0.0f);
                glCallList(Frag_Map[i][j] + 1);
            }
        }
    }
    glEnable(GL_TEXTURE_2D);
    if (Time_Switch_Map == TIME_SWITCH_MAP && Time_Switch_Frag == TIME_SWITCH_FRAG) {
        glLoadIdentity();
        glTranslatef(Frag_Pos[SFY][SFX].x, Frag_Pos[SFY][SFX].y, 0.0f);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 100, 100, 0, GL_RGBA, GL_UNSIGNED_BYTE, Img_Arrow);
        glBegin(GL_POLYGON);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(220.0f, 120.0f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(420.0f, 120.0f);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(420.0f, 320.0f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(220.0f, 320.0f);
        glEnd();
    }
    if (Jump_Stt > -1)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, Img_Jump[Drt][Jump_Stt]);
    else if (IsRun)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, Img_Run[Drt][Stt]);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, Img_Stand);
    glLoadIdentity();
    glTranslatef(xMap - Save_Map_X, yMap - Save_Map_Y, 0.0f);
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

void Display_No_Switch()
{
    glLoadIdentity();
    glColor3f(0.2f, 0.2f, 0.2f);
    glTranslatef(xMap, yMap, 0.0f);
    for (i = 0; i < List_Virtual_Wall[SFY][SFX].Count; i++) {
        Ptr_Rect = &List_Virtual_Wall[SFY][SFX].Rect[i];
        glBegin(GL_POLYGON);
        glVertex2f(Ptr_Rect->Left, Ptr_Rect->Bottom);
        glVertex2f(Ptr_Rect->Right, Ptr_Rect->Bottom);
        glVertex2f(Ptr_Rect->Right, Ptr_Rect->Top);
        glVertex2f(Ptr_Rect->Left, Ptr_Rect->Top);
        glEnd();
    }
    glColor3f(0.0f, 0.0f, 0.0f);
    for (i = 0; i < MAX_FRAG_Y; i++) {
        for (j = 0; j < MAX_FRAG_X; j++) {
            if (Frag_Map[i][j] != -1) {
                glLoadIdentity();
                glTranslatef(xMap + Frag_Pos[i][j].x, yMap + Frag_Pos[i][j].y, 0.0f);
                glCallList(Frag_Map[i][j] + 1);
            }
        }
    }
    glEnable(GL_TEXTURE_2D);
    if (Jump_Stt > -1)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, Img_Jump[Drt][Jump_Stt]);
    else if (IsRun)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, Img_Run[Drt][Stt]);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, Img_Stand);
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

void Display_Game()
{
    glClear(GL_COLOR_BUFFER_BIT);
    if (Switch_Map)
        Display_Switch();
    else
        Display_No_Switch();
    glutSwapBuffers();
}

void Resize()
{
    glutPositionWindow(POS_X, POS_Y);
    glutReshapeWindow(WIDTH, HEIGHT);
}

void Set_View()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, Current_Width, 0.0, Current_Height);
    glViewport(0, 0, WIDTH, HEIGHT);
    glMatrixMode(GL_MODELVIEW);
}

void Init_Game()
{
    // SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != -1) {
        Music_BackGround = Mix_LoadMUS("Sounds/BackGround.mp3");
        Sound_Jump = Mix_LoadWAV("Sounds/Jump.wav");
        Sound_Win = Mix_LoadWAV("Sounds/Win.wav");
        if (!Mix_PlayingMusic())
            Mix_PlayMusic(Music_BackGround, -1);
    }
    // Init Image
    loadPngSwap(&Img_Main, &Tmp, &Tmp_2, "Images/Main.png");
    for (i = 0; i < 10; i++) {
        sprintf(Str, "Images/%d.png", i);
        loadPngSwap(&Img_Num[i].img, &Img_Num[i].w, &Img_Num[i].h, Str);
    }
    loadPngSwap(&Img_Main_Arrow[0], &Main_Arrow.Width, &Main_Arrow.Height, "Images/Left.png");
    loadPngSwap(&Img_Main_Arrow[1], &Main_Arrow.Width, &Main_Arrow.Height, "Images/Right.png");
    for (i = 0; i < 20; i++) {
        sprintf(Str, "Images/RL%02d.png", i);
        loadPngSwap(&Img_Run[0][i], &w, &h, Str);
        sprintf(Str, "Images/RR%02d.png", i);
        loadPngSwap(&Img_Run[1][i], &w, &h, Str);
    }
    loadPngSwap(&Img_Jump[0][0], &w, &h, "Images/JL.png");
    loadPngSwap(&Img_Jump[0][1], &w, &h, "Images/FL.png");
    loadPngSwap(&Img_Jump[1][0], &w, &h, "Images/JR.png");
    loadPngSwap(&Img_Jump[1][1], &w, &h, "Images/FR.png");
    loadPngSwap(&Img_Stand, &w, &h, "Images/Stand.png");
    loadPngSwap(&Img_Arrow, &Tmp, &Tmp_2, "Images/Arrow.png");
    loadPngSwap(&Img_Door, &Tmp, &Tmp_2, "Images/Door.png");
    // GLUT and Game
    Func_Check_Nothing[0] = &Check_Nothing_Up;
    Func_Check_Nothing[1] = &Check_Nothing_Down;
    Func_Check_Nothing[2] = &Check_Nothing_Left;
    Func_Check_Nothing[3] = &Check_Nothing_Right;
    Func_Draw_Path[0] = &Draw_Path_Wall;
    Func_Draw_Path[1] = &Draw_Path_Nothing;
    Func_Draw_Path[2] = &Draw_Path_Door;
    Func_Move[0] = &Move_Left;
    Func_Move[1] = &Move_Right;
    Func_Collide[0] = &Collide_Above;
    Func_Collide[1] = &Collide_Below;
    Init_Num_Stage();
    Init_Main_Arrow();
    Init_Frag_Offset();
    Init_Virtual_Wall();
    Import_Map();
    glClearColor(0.8f, 0.9f, 1.0f, 1.0f);
    Current_Width = WIDTH;
    Current_Height = HEIGHT;
    Set_View();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    for (i = 0; i < 19; i++)
        Loop20[i] = i + 1;
    Loop20[19] = 0;
}

void Move_Left()
{
    Tmp_X = (int)(px + ADD_LEFT_2) / SIZE - MAX_X;
    Tmp_Y = (py + 10) / SIZE;
    Tmp_Y2 = (py + 50) / SIZE;
    if (Tmp_X < 0) {
        Tmp = CFX - 1;
        if (Tmp > -1) {
            Tmp_2 = Frag_Map[CFY][Tmp];
            if (Tmp_2 != -1) {
                Tmp_X += MAX_X;
                if (Map[Tmp_2][Tmp_Y][Tmp_X] && Map[Tmp_2][Tmp_Y2][Tmp_X]) {
                    xMap -= Mx;
                    px += Mx + SIZE_FULL_X;
                    CM = Tmp_2;
                    CFX = Tmp;
                }
            }
        }
    } else if (Map[CM][Tmp_Y][Tmp_X] && Map[CM][Tmp_Y2][Tmp_X]) {
        xMap -= Mx;
        px += Mx;
        Tmp = CM;
        Tmp_2 = (px + 10) / SIZE;
        if (Tmp_2 >= MAX_X) {
            Tmp_2 -= MAX_X;
            Tmp = Frag_Map[CFY][CFX + 1];
        }
        if (Jump_Stt == -1 && Tmp_Y > 0 && Map[Tmp][Tmp_Y - 1][Tmp_2]) {
            Jump_Stt = 1;
            v = 0.0f;
        }
    }
}

void Move_Right()
{
    Tmp_X = (px + 28) / SIZE;
    Tmp_Y = (py + 10) / SIZE;
    Tmp_Y2 = (py + 50) / SIZE;
    if (Tmp_X >= MAX_X) {
        Tmp = CFX + 1;
        if (Tmp < MAX_FRAG_X) {
            Tmp_2 = Frag_Map[CFY][Tmp];
            if (Tmp_2 != -1) {
                Tmp_X -= MAX_X;
                if (Map[Tmp_2][Tmp_Y][Tmp_X] && Map[Tmp_2][Tmp_Y2][Tmp_X]) {
                    xMap -= Mx;
                    px += Mx - SIZE_FULL_X;
                    CM = Tmp_2;
                    CFX = Tmp;
                }
            }
        }
    } else if (Map[CM][Tmp_Y][Tmp_X] && Map[CM][Tmp_Y2][Tmp_X]) {
        xMap -= Mx;
        px += Mx;
        Tmp = CM;
        Tmp_2 = (int)(px + ADD_LEFT) / SIZE - MAX_X;
        if (Tmp_2 < 0) {
            Tmp_2 += MAX_X;
            Tmp = Frag_Map[CFY][CFX - 1];
        }
        if (Jump_Stt == -1 && Tmp_Y > 0 && Map[Tmp][Tmp_Y - 1][Tmp_2]) {
            Jump_Stt = 1;
            v = 0.0f;
        }
    }
}

void Collide_Above()
{
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
}

void Collide_Below()
{
    Tmp_X = (int)(px + ADD_LEFT) / SIZE - MAX_X;
    Tmp_X2 = (int)(px + ADD_RIGHT) / SIZE - MAX_X;
    Tmp_Y = (int)(py + SIZE_FULL_Y) / SIZE - MAX_Y;
    Is_Stop_Fall = 0;
    if (Tmp_Y < 0) {
        Tmp = CFY - 1;
        if (Tmp >= 0) {
            Tmp_2 = Frag_Map[Tmp][CFX];
            if (Tmp_2 != -1) {
                if (Map[Tmp_2][Tmp_Y + MAX_Y][Tmp_X] && Map[Tmp_2][Tmp_Y + MAX_Y][Tmp_X2]) {
                    py += SIZE_FULL_Y;
                    CM = Tmp_2;
                    CFY = Tmp;
                } else
                    Is_Stop_Fall = 1;
            } else
                Is_Stop_Fall = 1;
        } else
            Is_Stop_Fall = 1;
    } else {
        Tmp = Tmp_2 = CM;
        if (Tmp_X < 0) {
            Tmp_X += MAX_X;
            if (CFX > 0) {
                Tmp = Frag_Map[CFY][CFX - 1];
                if (Tmp == -1)
                    Is_Stop_Fall = 1;
            } else
                Is_Stop_Fall = 1;
        } else if (Tmp_X >= MAX_X) {
            Tmp_X -= MAX_X;
            if (CFX < MAX_FRAG_X - 1) {
                Tmp = Frag_Map[CFY][CFX + 1];
                if (Tmp == -1)
                    Is_Stop_Fall = 1;
            } else
                Is_Stop_Fall = 1;
        }
        if (Tmp_X2 < 0) {
            Tmp_X2 += MAX_X;
            if (CFX > 0) {
                Tmp_2 = Frag_Map[CFY][CFX - 1];
                if (Tmp_2 == -1)
                    Is_Stop_Fall = 1;
            } else
                Is_Stop_Fall = 1;
        } else if (Tmp_X2 >= MAX_X) {
            Tmp_X2 -= MAX_X;
            if (CFX < MAX_FRAG_X - 1) {
                Tmp_2 = Frag_Map[CFY][CFX + 1];
                if (Tmp_2 == -1)
                    Is_Stop_Fall = 1;
            } else
                Is_Stop_Fall = 1;
        }
        if (!Map[Tmp][Tmp_Y][Tmp_X] || !Map[Tmp_2][Tmp_Y][Tmp_X2])
            Is_Stop_Fall = 1;
    }
    if (Is_Stop_Fall) {
        Tmp_F = py;
        py = (Tmp_Y + 1) * SIZE;
        yMap -= (py - Tmp_F);
        Jump_Stt = -1;
    }
}

void Action_Switch_Map()
{
    if (Time_Switch_Map < TIME_SWITCH_MAP) {
        Time_Switch_Map++;
        if (Time_Switch_Map == TIME_SWITCH_MAP) {
            if (Switch_Map == -1) {
                Switch_Map = 0;
                if (py < 1.0f) {
                    Tmp = CFY - 1;
                    if (Tmp >= 0) {
                        Tmp_2 = Frag_Map[Tmp][CFX];
                        if (Tmp_2 != -1) {
                            Tmp_X = (int)(px + ADD_LEFT) / SIZE - MAX_X;
                            Tmp_X2 = (int)(px + ADD_RIGHT) / SIZE - MAX_X;
                            if (Map[Tmp_2][LAST_Y][Tmp_X] && Map[Tmp_2][LAST_Y][Tmp_X2]) {
                                v = 0.0f;
                                Jump_Stt = 2;
                            }
                        }
                    }
                }
                glutSpecialFunc(Special_Game);
                glutSpecialUpFunc(Special_Up_Game);
            } else {
                glutSpecialFunc(Special_Switch);
            }
        }
        Current_Width += Offset_View_Switch_X;
        Current_Height += Offset_View_Switch_Y;
        Set_View();
        xMap += Offset_Switch_X;
        yMap += Offset_Switch_Y;
        Is_Redraw = 1;
    }
}

void Action_Switch_Frag()
{
    if (Time_Switch_Frag < TIME_SWITCH_FRAG) {
        Time_Switch_Frag++;
        if (Time_Switch_Frag == TIME_SWITCH_FRAG) {
            glutSpecialFunc(Special_Switch);
        }
        Frag_Offset[SSFY][SSFX].x += Offset_Frag_X;
        Frag_Offset[SSFY][SSFX].y += Offset_Frag_Y;
        Save_Map_X += Offset_Save_X;
        Save_Map_Y += Offset_Save_Y;
        Is_Redraw = 1;
    }
}

void Timer_Menu()
{
    Main_Arrow.Stt = Main_Arrow.Loop[Main_Arrow.Stt];
    glutPostRedisplay();
}

void Timer_Game()
{
    Is_Redraw = 0;
    Action_Switch_Map();
    if (!Switch_Map) {
        if (IsRun) {
            Func_Move[Drt]();
            Stt = Loop20[Stt];
            Is_Redraw = 1;
        }
        if (Jump_Stt > -1) {
            py += v;
            yMap -= v;
            v -= g;
            if (v < MIN_V)
                v = MIN_V;
            if (v < 0)
                Jump_Stt = 1;
            Func_Collide[Jump_Stt]();
            Is_Redraw = 1;
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
    } else
        Action_Switch_Frag();
    if (Win) {
        if (Alpha < 1.0f) {
            Alpha += 0.05f;
            Is_Redraw = 1;
        }
    }
    if (Is_Redraw)
        glutPostRedisplay();
}

void Timer(int value)
{
    Func_Timer();
    glutTimerFunc(INTERVAL, Timer, 0);
}

void Special_Menu(int key, int x, int y)
{
    switch (key) {
    case GLUT_KEY_LEFT:
        if (Main_Arrow.Can_Draw[0]) {
            Main_Arrow.Can_Draw[1] = 1;
            Num_Stage.Current--;
            if (Num_Stage.Current == 1)
                Main_Arrow.Can_Draw[0] = 0;
            Change_Num_Stage();
            glutPostRedisplay();
        }
        break;
    case GLUT_KEY_RIGHT:
        if (Main_Arrow.Can_Draw[1]) {
            Main_Arrow.Can_Draw[0] = 1;
            Num_Stage.Current++;
            if (Num_Stage.Current == Num_Stage.Max)
                Main_Arrow.Can_Draw[1] = 0;
            Change_Num_Stage();
            glutPostRedisplay();
        }
        break;
    }
}

void Keyboard_Menu(unsigned char key, int x, int y)
{
    if (key == 13) {
        In_Game();
    }
}

void Special_Game(int key, int x, int y)
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
        if (Jump_Stt == -1) {
            Jump_Stt = 0;
            v = BASE_V;
            if (!Mix_Playing(-1))
                Mix_PlayChannel(-1, Sound_Jump, 0);
        }
        break;
    }
}

void Special_Up_Game(int key, int x, int y)
{
    if (key == Def_Drt[Drt]) {
        IsRun = 0;
        glutPostRedisplay();
    }
}

void Keyboard_Game(unsigned char key, int x, int y)
{
    if (key == 32 && Time_Switch_Frag == TIME_SWITCH_FRAG && Time_Switch_Map == TIME_SWITCH_MAP) {
        if (Switch_Map) {
            Switch_Map = -1;
            Offset_Switch_X = Save_Map_X / TIME_SWITCH_MAP;
            Offset_Switch_Y = Save_Map_Y / TIME_SWITCH_MAP;
            Offset_View_Switch_X = -WIDTH / TIME_SWITCH_MAP;
            Offset_View_Switch_Y = -HEIGHT / TIME_SWITCH_MAP;
            Time_Switch_Map = 0;
            glutSpecialFunc(NULL);
        } else {
            Switch_Map = 1;
            IsRun = 0;
            Save_Map_X = xMap;
            Save_Map_Y = yMap;
            Offset_Switch_X = -xMap / TIME_SWITCH_MAP;
            Offset_Switch_Y = -yMap / TIME_SWITCH_MAP;
            Offset_View_Switch_X = WIDTH / TIME_SWITCH_MAP;
            Offset_View_Switch_Y = HEIGHT / TIME_SWITCH_MAP;
            Time_Switch_Map = 0;
            glutSpecialFunc(NULL);
            glutSpecialUpFunc(NULL);
        }
    }
}

void Special_Switch(int key, int x, int y)
{
    switch (key) {
    case GLUT_KEY_LEFT:
        if (SFX > 0) {
            Time_Switch_Frag = 0;
            glutSpecialFunc(NULL);
            SSFX = SFX;
            SSFY = SFY;
            Frag_Offset[SFY][SFX].x = -SIZE_FULL_X;
            Frag_Offset[SFY][SFX].y = 0.0f;
            Offset_Frag_X = SIZE_FULL_X / TIME_SWITCH_FRAG;
            Offset_Frag_Y = 0.0f;
            Frag_Map[SFY][SFX] = Frag_Map[SFY][SFX - 1];
            SFX--;
            Frag_Map[SFY][SFX] = -1;
            if (SFX == CFX && SFY == CFY) {
                Offset_Save_X = -SIZE_FULL_X / TIME_SWITCH_FRAG;
                Offset_Save_Y = 0.0f;
                CFX++;
            } else
                Offset_Save_X = Offset_Save_Y = 0.0f;
        }
        break;
    case GLUT_KEY_RIGHT:
        if (SFX < MAX_FRAG_X - 1) {
            Time_Switch_Frag = 0;
            glutSpecialFunc(NULL);
            SSFX = SFX;
            SSFY = SFY;
            Frag_Offset[SFY][SFX].x = SIZE_FULL_X;
            Frag_Offset[SFY][SFX].y = 0.0f;
            Offset_Frag_X = -SIZE_FULL_X / TIME_SWITCH_FRAG;
            Offset_Frag_Y = 0.0f;
            Frag_Map[SFY][SFX] = Frag_Map[SFY][SFX + 1];
            SFX++;
            Frag_Map[SFY][SFX] = -1;
            if (SFX == CFX && SFY == CFY) {
                Offset_Save_X = SIZE_FULL_X / TIME_SWITCH_FRAG;
                Offset_Save_Y = 0.0f;
                CFX--;
            } else
                Offset_Save_X = Offset_Save_Y = 0.0f;
        }
        break;
    case GLUT_KEY_UP:
        if (SFY < MAX_FRAG_Y - 1) {
            Time_Switch_Frag = 0;
            glutSpecialFunc(NULL);
            SSFX = SFX;
            SSFY = SFY;
            Frag_Offset[SFY][SFX].x = 0.0f;
            Frag_Offset[SFY][SFX].y = SIZE_FULL_Y;
            Offset_Frag_X = 0.0f;
            Offset_Frag_Y = -SIZE_FULL_Y / TIME_SWITCH_FRAG;
            Frag_Map[SFY][SFX] = Frag_Map[SFY + 1][SFX];
            SFY++;
            Frag_Map[SFY][SFX] = -1;
            if (SFX == CFX && SFY == CFY) {
                Offset_Save_X = 0.0f;
                Offset_Save_Y = SIZE_FULL_Y / TIME_SWITCH_FRAG;
                CFY--;
            } else
                Offset_Save_X = Offset_Save_Y = 0.0f;
        }
        break;
    case GLUT_KEY_DOWN:
        if (SFY > 0) {
            Time_Switch_Frag = 0;
            glutSpecialFunc(NULL);
            SSFX = SFX;
            SSFY = SFY;
            Frag_Offset[SFY][SFX].x = 0.0f;
            Frag_Offset[SFY][SFX].y = -SIZE_FULL_Y;
            Offset_Frag_X = 0.0f;
            Offset_Frag_Y = SIZE_FULL_Y / TIME_SWITCH_FRAG;
            Frag_Map[SFY][SFX] = Frag_Map[SFY - 1][SFX];
            SFY--;
            Frag_Map[SFY][SFX] = -1;
            if (SFX == CFX && SFY == CFY) {
                Offset_Save_X = 0.0f;
                Offset_Save_Y = -SIZE_FULL_Y / TIME_SWITCH_FRAG;
                CFY++;
            } else
                Offset_Save_X = Offset_Save_Y = 0.0f;
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
    Init_Game();
    Main_Menu();
    glutReshapeFunc(Resize);
    glutTimerFunc(0, Timer, 0);
    glutMainLoop();
    Mix_CloseAudio();
    return 0;
}
