#include "../../Library/loadpng.h"
#include <GL/freeglut.h>
#include <SDL2/SDL_mixer.h>

#define WIDTH 640.0f
#define HEIGHT 440.0f
#define POS_X 300
#define POS_Y 160
#define HERO_WIDTH 60
#define HERO_HEIGHT 60
#define SIZE 40
#define MAX_X 16
#define MAX_Y 11
#define LAST_X 15          // MAX_X - 1
#define LAST_Y 10          // MAX_Y - 1
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
#define BASE_ANIM_SPEED 38.0f

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
    GLubyte *img;
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

struct Anim_Stage {
    int Draw, Count;
    float Left[8], Right[8];
    float Bottom, Top;
    float Speed[45];
    float Offset;
    int Text[8];
    int Length, Stt, Max, Done[2];
} Anim_Stage;

void (*Func_Draw_Path[3])(int x, int y);
void (*Func_Move[2])();
void (*Func_Collide[2])();
void (*Func_Timer)();
int (*Func_Check_Nothing[4])(int *x, int *y, int *u, int *v);

// Init
void Decode_Image(Image *img, const char *path);
void Crop_Image(Image *Source, Image *Dest, int x, int y, int w, int h);
void Load_Menu_Num();
void Load_Char();
void Load_Hero();
void Init_Image();
void Init_Main_Arrow();
void Change_Num_Stage();
void Init_Num_Stage();
void Set_Anim_Stage_Text();
void Init_Anim_Stage();
void Action_Anim_Stage();
void Init_Frag_Offset();
void Init_Virtual_Wall();
void Set_Virtual_Wall(int *x, int *y, int *u, int *v, int Drt);
void Init_Game();
void Main_Menu();
int Import_Map();
void In_Game();
void Win_Game();
int Check_Nothing_Up(int *x, int *y, int *u, int *v);
int Check_Nothing_Down(int *x, int *y, int *u, int *v);
int Check_Nothing_Left(int *x, int *y, int *u, int *v);
int Check_Nothing_Right(int *x, int *y, int *u, int *v);

// Display
void Draw_Anim_Stage();
void Draw_Path_Wall(int x, int y);
void Draw_Path_Nothing(int x, int y);
void Draw_Path_Door(int x, int y);
void Draw_Hero();
void Display_Menu();
void Display_Switch();
void Display_No_Switch();
void Display_Game();
void Resize();
void Set_View();

// Move
void Move_Left();
void Move_Right();
void Collide_Above();
void Collide_Below();

// Timer
void Action_Switch_Map();
void Action_Switch_Frag();
void Timer_Menu();
void Timer_Game();
void Timer(int value);

// Keyboard
void Special_Menu(int key, int x, int y);
void Keyboard_Menu(unsigned char key, int x, int y);
void Special_Game(int key, int x, int y);
void Special_Up_Game(int key, int x, int y);
void Keyboard_Game(unsigned char key, int x, int y);
void Special_Switch(int key, int x, int y);

Struct_List_Rect List_Virtual_Wall[MAX_FRAG_Y][MAX_FRAG_X];
Struct_Rect Rect_Virtual_Wall[4] = {
    {-SIZE_VIRTUAL_WALL, SIZE_FULL_X + SIZE_VIRTUAL_WALL, -SIZE_VIRTUAL_WALL, 0.0f},
    {-SIZE_VIRTUAL_WALL, SIZE_FULL_X + SIZE_VIRTUAL_WALL, SIZE_FULL_Y, SIZE_FULL_Y + SIZE_VIRTUAL_WALL},
    {-SIZE_VIRTUAL_WALL, 0.0f, 0.0f, SIZE_FULL_Y},
    {SIZE_FULL_X, SIZE_FULL_X + SIZE_VIRTUAL_WALL, 0.0f, SIZE_FULL_Y}};
Struct_Rect *Ptr_Rect;
Struct_List_Rect *Ptr_List_Rect;
Struct_Pos Frag_Pos[MAX_FRAG_Y][MAX_FRAG_X];
Struct_Pos Frag_Offset[MAX_FRAG_Y][MAX_FRAG_X];

Image Img_Num[10];
Image Img_Char[16];
Image *Ptr_Img;

GLubyte *Img_Main;
GLubyte *Img_Main_Arrow[2];
GLubyte *Img_Run[2][20];
GLubyte *Img_Jump[2][2];
GLubyte *Img_Stand;
GLubyte *Img_Arrow;
GLubyte *Img_Door;
GLubyte *Img_Tmp;

Mix_Music *Music_BackGround = NULL;
Mix_Chunk *Sound_Jump = NULL;
Mix_Chunk *Sound_Win = NULL;

GLuint Def_Drt[] = {GLUT_KEY_LEFT, GLUT_KEY_RIGHT};

int Tmp_L, Tmp_R;
int w, h, i, j, k;
float l, r, b, t, px, py, Start_X, Start_Y;
float v, g = 1.4f;
float Mx = 0.0f, xMap, yMap, Tmp_F;
int Drt, Stt, Is_Run, Jump_Stt, Is_Win, Win_Stt;
int Loop20[20];
int Tmp, Tmp_2, Tmp_3, Tmp_X, Tmp_X2, Tmp_Y, Tmp_Y2, Tmp_CM_1, Tmp_CM_2;
int CM, CFX, CFY, SFX, SFY, SSFX, SSFY;
char Switch_Map, Is_Stop_Jump, Is_Stop_Fall, Is_Redraw;
float Save_Map_X, Save_Map_Y, Offset_Switch_X, Offset_Switch_Y, Offset_View_Switch_X, Offset_View_Switch_Y;
float Offset_Frag_X, Offset_Frag_Y, Offset_Save_X, Offset_Save_Y;
int Time_Switch_Map = TIME_SWITCH_MAP, Time_Switch_Frag = TIME_SWITCH_FRAG;
char Str[40];
float Win_Alpha;
char Frag_Map[MAX_FRAG_Y][MAX_FRAG_X];
int Map[NUM_FRAG][MAX_Y][MAX_X];
float Current_Width, Current_Height;
char Arr_Map[530];

#include "init.c"
#include "display.c"
#include "move.c"
#include "time.c"
#include "keyboard.c"
