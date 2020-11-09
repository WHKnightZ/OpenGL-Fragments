#include "main.h"

#if 0
	Welcome to Fragments!
#endif

void Init_Main_Arrow() {
    Main_Arrow.Can_Draw[0] = 0;
    if (Num_Stage.Max > 1)
        Main_Arrow.Can_Draw[1] = 1;
    Main_Arrow.Stt = 0;
    Main_Arrow.Drt[0] = -1;
    Main_Arrow.Drt[1] = 1;
    float s = 0.0f, v = 0.0f, a = 0.2f;
    for (i = 0; i <= 10; i++) {
        Main_Arrow.Offset[i] = (int)s;
        v +=a;
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

void Change_Num_Stage() {
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

void Init_Num_Stage() {
    Num_Stage.Max = 1;
    FILE *f;
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

void Crop_Image(Image *Source, Image *Dest, int x, int y, int w, int h) {
    Dest->w = w;
    Dest->h = h;
    Dest->img = malloc(w * h * 4);
    unsigned char *imgSource = Source->img, *imgDest = Dest->img, *img;
    int wSource = Source->w;
    int start, i, j;
    start = wSource * 4 * y + x * 4;
    for (i = 0; i < h; i++) {
        img = imgSource + start;
        start += wSource * 4;
        for (j = 0; j < w; j++) {
            *(imgDest++) = *(img++);
            *(imgDest++) = *(img++);
            *(imgDest++) = *(img++);
            *(imgDest++) = *(img++);
        }
    }
}

void Load_Menu_Num() {
    int Pos[12][4] = {
        {0, 0, 34, 64},
        {34, 0, 26, 64},
        {60, 0, 36, 64},
        {96, 0, 37, 64},
        {133, 0, 39, 64},
        {172, 0, 38, 64},
        {210, 0, 38, 64},
        {248, 0, 38, 64},
        {286, 0, 36, 64},
        {322, 0, 37, 64},
        {359, 0, 16, 24},
        {375, 0, 16, 24}};
    Image Img_Menu_Num;
    Image Img_Tmp;
    Load_Png(&Img_Menu_Num.img, &Img_Menu_Num.w, &Img_Menu_Num.h, "Images/Menu_Num.png");
    for (i = 0; i < 10; i++) {
        Crop_Image(&Img_Menu_Num, &Img_Num[i], Pos[i][0], Pos[i][1], Pos[i][2], Pos[i][3]);
        Swap_Image(Img_Num[i].img, Img_Num[i].w, Img_Num[i].h);
    }
    Main_Arrow.Width = Pos[10][2];
    Main_Arrow.Height = Pos[10][3];
    Crop_Image(&Img_Menu_Num, &Img_Tmp, Pos[10][0], Pos[10][1], Pos[10][2], Pos[10][3]);
    Swap_Image(Img_Tmp.img, Main_Arrow.Width, Main_Arrow.Height);
    Img_Main_Arrow[0] = Img_Tmp.img;
    Crop_Image(&Img_Menu_Num, &Img_Tmp, Pos[11][0], Pos[11][1], Pos[11][2], Pos[11][3]);
    Swap_Image(Img_Tmp.img, Main_Arrow.Width, Main_Arrow.Height);
    Img_Main_Arrow[1] = Img_Tmp.img;
    free(Img_Menu_Num.img);
}

void Load_Char() {
    int Pos[16][4] = {
        {0, 0, 39, 62},
        {39, 0, 26, 62},
        {78, 0, 39, 62},
        {117, 0, 36, 62},
        {0, 62, 39, 62},
        {39, 62, 34, 62},
        {78, 62, 33, 62},
        {117, 62, 34, 62},
        {0, 124, 36, 62},
        {39, 124, 34, 62},
        {78, 124, 36, 62},
        {117, 124, 31, 62},
        {0, 186, 38, 62},
        {39, 186, 36, 62},
        {78, 186, 36, 62},
        {117, 186, 18, 62}};
    Image Img_Font;
    Load_Png(&Img_Font.img, &Img_Font.w, &Img_Font.h, "Images/Stage_Font.png");
    for (i = 0; i < 16; i++) {
        Crop_Image(&Img_Font, &Img_Char[i], Pos[i][0], Pos[i][1], Pos[i][2], Pos[i][3]);
        Swap_Image(Img_Char[i].img, Img_Char[i].w, Img_Char[i].h);
    }
    free(Img_Font.img);
}

void Set_Anim_Stage_Text() {
    if (Num_Stage.Current < 10) {
        Anim_Stage.Text[6] = Num_Stage.Current;
        Anim_Stage.Length = 7;
    } else {
        Anim_Stage.Text[6] = Num_Stage.Current / 10;
        Anim_Stage.Text[7] = Num_Stage.Current % 10;
        Anim_Stage.Length = 8;
    }
    int Size = 4;
    for (i = 0; i < Anim_Stage.Length; i++)
        Size += Img_Char[Anim_Stage.Text[i]].w - 4;
    Anim_Stage.Left[0] = (WIDTH - Size) / 2 - Anim_Stage.Offset;
    Anim_Stage.Right[0] = Anim_Stage.Left[0] + Img_Char[Anim_Stage.Text[0]].w;
    for (i = 1; i < Anim_Stage.Length; i++) {
        Anim_Stage.Left[i] = Anim_Stage.Right[i - 1] - 4.0f;
        Anim_Stage.Right[i] = Anim_Stage.Left[i] + Img_Char[Anim_Stage.Text[i]].w;
    }
}

void Init_Anim_Stage() {
    int i;
    for (i = 0; i < 6; i++)
        Anim_Stage.Text[i] = 10 + i;
    Anim_Stage.Stt = 0;
    Anim_Stage.Done[0] = 80;
    Anim_Stage.Done[1] = 40;
    Anim_Stage.Speed[0] = BASE_ANIM_SPEED;
    int n = 0;
    while ((int)Anim_Stage.Speed[n] > 0) {
        Anim_Stage.Speed[n + 1] = Anim_Stage.Speed[n] * 0.92f;
        n++;
    }
    Anim_Stage.Max = n;
    Anim_Stage.Bottom = (int)(HEIGHT - Img_Char[0].h) / 2;
    Anim_Stage.Top = Anim_Stage.Bottom + Img_Char[0].h;
    Anim_Stage.Offset = 0;
    int tmp;
    for (i = 0; i < n; i++) {
        tmp = (int)Anim_Stage.Speed[i];
        Anim_Stage.Speed[i] = tmp;
        Anim_Stage.Offset += tmp;
    }
}

void Action_Anim_Stage() {
    int j;
    for (i = 0; i < Anim_Stage.Length; i++) {
        j = Anim_Stage.Stt + i - Anim_Stage.Length + 1;
        if (j >= 0 && j <= Anim_Stage.Max) {
            Anim_Stage.Left[i] += Anim_Stage.Speed[j];
            Anim_Stage.Right[i] = Anim_Stage.Left[i] + Img_Char[Anim_Stage.Text[i]].w;
        }
    }
    Anim_Stage.Stt++;
    if (Anim_Stage.Stt == Anim_Stage.Done[Anim_Stage.Count]) {
        Anim_Stage.Count++;
        if (Anim_Stage.Count < 2)
            Anim_Stage.Stt = 0;
        else {
            Anim_Stage.Draw = 0;
            Set_Walkthrough();
        }
    }
}

void Draw_Anim_Stage() {
    for (i = 0; i < Anim_Stage.Length; i++) {
        Ptr_Img = &Img_Char[Anim_Stage.Text[i]];
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Ptr_Img->w, Ptr_Img->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, Ptr_Img->img);
        glBegin(GL_POLYGON);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(Anim_Stage.Left[i], Anim_Stage.Bottom);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(Anim_Stage.Right[i], Anim_Stage.Bottom);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(Anim_Stage.Right[i], Anim_Stage.Top);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(Anim_Stage.Left[i], Anim_Stage.Top);
        glEnd();
    }
}

void Load_Hero() {
    Image Img_Hero;
    Image Img_Tmp;
    Load_Png(&Img_Hero.img, &Img_Hero.w, &Img_Hero.h, "Images/Hero.png");
    for (i = 0; i < 20; i++) {
        Crop_Image(&Img_Hero, &Img_Tmp, (i % 5) * 60, (i / 5) * 60, 60, 60);
        Swap_Image(Img_Tmp.img, 60, 60);
        Img_Run[0][i] = Img_Tmp.img;
        Crop_Image(&Img_Hero, &Img_Tmp, (i % 5) * 60, (i / 5) * 60 + 240, 60, 60);
        Swap_Image(Img_Tmp.img, 60, 60);
        Img_Run[1][i] = Img_Tmp.img;
    }
    Crop_Image(&Img_Hero, &Img_Tmp, 0, 480, 60, 60);
    Swap_Image(Img_Tmp.img, 60, 60);
    Img_Stand = Img_Tmp.img;
    Crop_Image(&Img_Hero, &Img_Tmp, 60, 480, 60, 60);
    Swap_Image(Img_Tmp.img, 60, 60);
    Img_Jump[0][0] = Img_Tmp.img;
    Crop_Image(&Img_Hero, &Img_Tmp, 120, 480, 60, 60);
    Swap_Image(Img_Tmp.img, 60, 60);
    Img_Jump[1][0] = Img_Tmp.img;
    Crop_Image(&Img_Hero, &Img_Tmp, 180, 480, 60, 60);
    Swap_Image(Img_Tmp.img, 60, 60);
    Img_Jump[0][1] = Img_Tmp.img;
    Crop_Image(&Img_Hero, &Img_Tmp, 240, 480, 60, 60);
    Swap_Image(Img_Tmp.img, 60, 60);
    Img_Jump[1][1] = Img_Tmp.img;
    free(Img_Hero.img);
}

void Main_Menu() {
    glColor3f(1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    Main_Arrow.Can_Draw[0] = 0;
    Main_Arrow.Can_Draw[1] = 1;
    Num_Stage.Current = 1;
    Change_Num_Stage();
    Func_Timer = &Timer_Menu;
    glutSpecialFunc(Special_Menu);
    glutSpecialUpFunc(NULL);
    glutKeyboardFunc(Keyboard_Menu);
    glutDisplayFunc(Display_Menu);
}

void In_Game() {
    if (Num_Stage.Current <= Num_Stage.Max) {
        Import_Map();
        glDisable(GL_TEXTURE_2D);
        Win_Alpha = 0.0f;
        Win_Stt = 0;
        px = Start_X;
        py = Start_Y;
        l = WIDTH / 2 - 30.0f;
        r = l + 60.0f;
        b = 120.0f;
        t = b + 60.0f;
        CFX = 0;
        CFY = 0;
        SFX = 0;
        SFY = 1;
        Frag_Map[0][0] = 0;
        Frag_Map[0][1] = 1;
        Frag_Map[1][0] = -1;
        Frag_Map[1][1] = 2;
        xMap = WIDTH / 2 - px - CFX * SIZE_FULL_X;
        yMap = b - py - CFY * SIZE_FULL_Y;
        CM = Frag_Map[CFY][CFX];
        Is_Win = 0;
        Jump_Stt = 1;
        v = 0.0f;
        Drt = 1;
        Stt = 0;
        Is_Run = 0;
        Anim_Stage.Stt = -20;
        Anim_Stage.Draw = 1;
        Anim_Stage.Count = 0;
        WT_In_Game();
        Set_Anim_Stage_Text();
        Func_Timer = &Timer_Game;
        glutDisplayFunc(Display_Game);
    } else
        Main_Menu();
}

void Draw_Path_Wall(int x, int y) {
    glBegin(GL_POLYGON);
    glVertex2f(x * SIZE, y * SIZE);
    glVertex2f(x * SIZE + SIZE, y * SIZE);
    glVertex2f(x * SIZE + SIZE, y * SIZE + SIZE);
    glVertex2f(x * SIZE, y * SIZE + SIZE);
    glEnd();
}

void Draw_Path_Nothing(int x, int y) {}

void Draw_Path_Door(int x, int y) {
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

int Import_Map() {
    sprintf(Str, "Maps/%02d.txt", Num_Stage.Current);
    FILE *f = fopen(Str, "r");
    if (f == NULL)
        return 0;
    fscanf(f, "%f%f", &Start_X, &Start_Y);
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

int Check_Nothing_Up(int *x, int *y, int *u, int *v) {
    if (*y == 0)
        return 1;
    if (*x == *u && *y - 1 == *v)
        return 1;
    return 0;
}

int Check_Nothing_Down(int *x, int *y, int *u, int *v) {
    if (*y + 1 == MAX_FRAG_Y)
        return 1;
    if (*x == *u && *y + 1 == *v)
        return 1;
    return 0;
}

int Check_Nothing_Left(int *x, int *y, int *u, int *v) {
    if (*x == 0)
        return 1;
    if (*x - 1 == *u && *y == *v)
        return 1;
    return 0;
}

int Check_Nothing_Right(int *x, int *y, int *u, int *v) {
    if (*x + 1 == MAX_FRAG_X)
        return 1;
    if (*x + 1 == *u && *y == *v)
        return 1;
    return 0;
}

void Init_Frag_Offset() {
    for (i = 0; i < MAX_FRAG_Y; i++)
        for (j = 0; j < MAX_FRAG_X; j++) {
            Frag_Pos[i][j].x = j * SIZE_FULL_X;
            Frag_Pos[i][j].y = i * SIZE_FULL_Y;
            Frag_Offset[i][j].x = 0.0f;
            Frag_Offset[i][j].y = 0.0f;
        }
}

void Set_Virtual_Wall(int *x, int *y, int *u, int *v, int Drt) {
    if (Func_Check_Nothing[Drt](x, y, u, v)) {
        Ptr_List_Rect->Rect[Ptr_List_Rect->Count].Left = *x * SIZE_FULL_X + Rect_Virtual_Wall[Drt].Left;
        Ptr_List_Rect->Rect[Ptr_List_Rect->Count].Right = *x * SIZE_FULL_X + Rect_Virtual_Wall[Drt].Right;
        Ptr_List_Rect->Rect[Ptr_List_Rect->Count].Bottom = *y * SIZE_FULL_Y + Rect_Virtual_Wall[Drt].Bottom;
        Ptr_List_Rect->Rect[Ptr_List_Rect->Count].Top = *y * SIZE_FULL_Y + Rect_Virtual_Wall[Drt].Top;
        Ptr_List_Rect->Count++;
    }
}

void Init_Virtual_Wall() {
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

void Display_Menu() {
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

void Draw_Hero() {
    if (Jump_Stt > -1)
        Img_Tmp = Img_Jump[Drt][Jump_Stt];
    else if (Is_Run)
        Img_Tmp = Img_Run[Drt][Stt];
    else
        Img_Tmp = Img_Stand;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, HERO_WIDTH, HERO_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, Img_Tmp);
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
}

void Display_Switch() {
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
    glLoadIdentity();
    glTranslatef(xMap - Save_Map_X, yMap - Save_Map_Y, 0.0f);
    Draw_Hero();
    glDisable(GL_TEXTURE_2D);
}

void Display_No_Switch() {
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
    glLoadIdentity();
    Draw_Hero();
    if (Anim_Stage.Draw) {
        glColor3f(1.0f, 1.0f, 1.0f);
        Draw_Anim_Stage();
        glColor3f(0.0f, 0.0f, 0.0f);
    }
    glDisable(GL_TEXTURE_2D);
    if (Is_Win) {
        glColor4f(0.2f, 0.6f, 0.8f, Win_Alpha);
        glTranslatef(250, 200, 0);
        glScalef(0.6f, 0.6f, 1.0f);
        glLineWidth(8.0f);
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, 87);
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, 73);
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, 78);
        glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
    }
}

void Display_Game() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (Switch_Map)
        Display_Switch();
    else
        Display_No_Switch();
    glutSwapBuffers();
}

void Resize() {
    glutPositionWindow(POS_X, POS_Y);
    glutReshapeWindow(WIDTH, HEIGHT);
}

void Set_View() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, Current_Width, 0.0, Current_Height);
    glViewport(0, 0, WIDTH, HEIGHT);
    glMatrixMode(GL_MODELVIEW);
}

void Init_Game() {
    // SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != -1) {
        Music_BackGround = Mix_LoadMUS("Sounds/BackGround.mp3");
        Sound_Jump = Mix_LoadWAV("Sounds/Jump.wav");
        Sound_Win = Mix_LoadWAV("Sounds/Win.wav");
        if (!Mix_PlayingMusic())
        	Mix_PlayMusic(Music_BackGround, -1);
    }
    // Init Image
    Load_Png_Swap(&Img_Main, &Tmp, &Tmp_2, "Images/Main.png");
    Load_Menu_Num();
    Load_Hero();
    Load_Png_Swap(&Img_Arrow, &Tmp, &Tmp_2, "Images/Arrow.png");
    Load_Png_Swap(&Img_Door, &Tmp, &Tmp_2, "Images/Door.png");
    // GL
    glClearColor(0.8f, 0.9f, 1.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glEnable(GL_LINE_SMOOTH);
    // Game
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
    Load_Char();
    Init_Anim_Stage();
    Init_Frag_Offset();
    Init_Virtual_Wall();
    Import_Map();
    Current_Width = WIDTH;
    Current_Height = HEIGHT;
    Set_View();
    Set_Walkthrough();
    for (i = 0; i < 19; i++)
        Loop20[i] = i + 1;
    Loop20[19] = 0;
}

void Move_Left() {
    Tmp_X = (int)(px + ADD_LEFT_2) / SIZE - MAX_X;
    Tmp_Y = (py + 10) / SIZE;
    Tmp_Y2 = (py + 50) / SIZE;
    if (Tmp_Y2 >= MAX_Y) {
        Tmp_Y2 -= MAX_Y;
        Tmp_CM_1 = CM;
        Tmp_CM_2 = Frag_Map[CFY + 1][CFX];
    } else if (py < -10) {
        Tmp_Y += MAX_Y;
        Tmp_CM_1 = Frag_Map[CFY - 1][CFX];
        Tmp_CM_2 = CM;
    } else
        Tmp_CM_1 = Tmp_CM_2 = CM;
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
    } else if (Map[Tmp_CM_1][Tmp_Y][Tmp_X] && Map[Tmp_CM_2][Tmp_Y2][Tmp_X]) {
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

void Move_Right() {
    Tmp_X = (px + 28) / SIZE;
    Tmp_Y = (py + 10) / SIZE;
    Tmp_Y2 = (py + 50) / SIZE;
    if (Tmp_Y2 >= MAX_Y) {
        Tmp_Y2 -= MAX_Y;
        Tmp_CM_1 = CM;
        Tmp_CM_2 = Frag_Map[CFY + 1][CFX];
    } else if (py < -10) {
        Tmp_Y += MAX_Y;
        Tmp_CM_1 = Frag_Map[CFY - 1][CFX];
        Tmp_CM_2 = CM;
    } else
        Tmp_CM_1 = Tmp_CM_2 = CM;
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
    } else if (Map[Tmp_CM_1][Tmp_Y][Tmp_X] && Map[Tmp_CM_2][Tmp_Y2][Tmp_X]) {
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

void Collide_Above() {
    Tmp_X = (int)(px + ADD_LEFT) / SIZE - MAX_X;
    Tmp_X2 = (int)(px + ADD_RIGHT) / SIZE - MAX_X;
    Tmp_Y = (py + 50) / SIZE;
    Is_Stop_Jump = 0;
    if (Tmp_Y >= MAX_Y) {
        Tmp = CFY + 1;
        if (Tmp < MAX_FRAG_Y) {
            Tmp_2 = Frag_Map[Tmp][CFX];
            if (Tmp_2 != -1) {
                if (!Map[Tmp_2][Tmp_Y - MAX_Y][Tmp_X] || !Map[Tmp_2][Tmp_Y - MAX_Y][Tmp_X2])
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
    if (py >= SIZE_FULL_Y) {
        py -= SIZE_FULL_Y;
        CFY += 1;
        CM = Frag_Map[CFY][CFX];
    }
}

void Collide_Below() {
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

void Action_Switch_Map() {
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

void Action_Switch_Frag() {
    if (Time_Switch_Frag < TIME_SWITCH_FRAG) {
        Time_Switch_Frag++;
        Frag_Offset[SSFY][SSFX].x += Offset_Frag_X;
        Frag_Offset[SSFY][SSFX].y += Offset_Frag_Y;
        Save_Map_X += Offset_Save_X;
        Save_Map_Y += Offset_Save_Y;
        Is_Redraw = 1;
    }
}

void Timer_Menu() {
    Main_Arrow.Stt = Main_Arrow.Loop[Main_Arrow.Stt];
    glutPostRedisplay();
}

void Win() {
    Mix_PlayChannel(-1, Sound_Win, 0);
    WT_Happen = 0;
    Is_Win = 1;
    Is_Run = 0;
    glutKeyboardFunc(NULL);
    glutSpecialFunc(NULL);
    glutSpecialUpFunc(NULL);
}

void Timer_Game() {
    if (WT_Happen) {
        if (WT_Stt == WT_Wait) {
            WT_Func[WT_Action[Num_Stage.Current][WT_Count].Action]();
            WT_Stt = 0;
            WT_Count++;
            WT_Wait = WT_Action[Num_Stage.Current][WT_Count].Wait;
        } else
            WT_Stt++;
    }
    Is_Redraw = 0;
    Action_Switch_Map();
    if (!Switch_Map) {
        if (Is_Run) {
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
        if (!Is_Win && Map[CM][(int)(py / SIZE)][(int)(px / SIZE)] == 2) {
            Win();
        }
    } else
        Action_Switch_Frag();
    if (Is_Win) {
        if (Win_Alpha < 1.0f) {
            Win_Alpha += 0.05f;
            Is_Redraw = 1;
        }
        Win_Stt++;
        if (Win_Stt == 50) {
            Num_Stage.Current++;
            In_Game();
        }
    }
    if (Anim_Stage.Draw) {
        Action_Anim_Stage();
        Is_Redraw = 1;
    }
    if (Is_Redraw)
        glutPostRedisplay();
}

void Timer(int value) {
    Func_Timer();
    glutTimerFunc(INTERVAL, Timer, 0);
}

void Special_Menu(int key, int x, int y) {
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

void Keyboard_Menu(unsigned char key, int x, int y) {
    if (key == 13) {
        glutKeyboardFunc(NULL);
        glutSpecialFunc(NULL);
        In_Game();
    }
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    int POS_X = (glutGet(GLUT_SCREEN_WIDTH) - WIDTH) / 2;
    int POS_Y = (glutGet(GLUT_SCREEN_HEIGHT) - HEIGHT) / 2;
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
