void Decode_Image(Image *img, const char *path) {
    FILE *f = fopen(path, "rb");
    if (f == NULL)
        return;
    fseek(f, 0, SEEK_END);
    long index = 0;
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    GLubyte *v = (GLubyte *)malloc(size * sizeof(char)), *w = v;
    *w++ = fgetc(f) + 62;
    *w++ = fgetc(f) - 10;
    *w++ = fgetc(f) + 5;
    *w++ = fgetc(f) + 39;
    *w++ = fgetc(f);
    *w++ = fgetc(f);
    index += 6;
    while (index < size) {
        *w = fgetc(f) - 1;
        index += 1;
        w++;
    }
    fclose(f);
    lodepng_decode_memory(&img->img, &img->w, &img->h, v, size, LCT_RGBA, 8);
    free(v);
}

void Crop_Image(Image *Source, Image *Dest, int x, int y, int w, int h) {
    Dest->w = w;
    Dest->h = h;
    Dest->img = (GLubyte*)malloc(w * h * 4);
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
    Decode_Image(&Img_Menu_Num, "Images/Menu_Num.kzi");
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
    Decode_Image(&Img_Font, "Images/Stage_Font.kzi");
    for (i = 0; i < 16; i++) {
        Crop_Image(&Img_Font, &Img_Char[i], Pos[i][0], Pos[i][1], Pos[i][2], Pos[i][3]);
        Swap_Image(Img_Char[i].img, Img_Char[i].w, Img_Char[i].h);
    }
    free(Img_Font.img);
}

void Load_Hero() {
    Image Img_Hero;
    Image Img_Tmp;
    Decode_Image(&Img_Hero, "Images/Hero.kzi");
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

void Init_Image() {
    Image Img_Tmp;
    Decode_Image(&Img_Tmp, "Images/Main.kzi");
    Swap_Image(Img_Tmp.img, Img_Tmp.w, Img_Tmp.h);
    Img_Main = Img_Tmp.img;
    Decode_Image(&Img_Tmp, "Images/Arrow.kzi");
    Swap_Image(Img_Tmp.img, Img_Tmp.w, Img_Tmp.h);
    Img_Arrow = Img_Tmp.img;
    Decode_Image(&Img_Tmp, "Images/Door.kzi");
    Swap_Image(Img_Tmp.img, Img_Tmp.w, Img_Tmp.h);
    Img_Door = Img_Tmp.img;
    Load_Menu_Num();
    Load_Hero();
}

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
        sprintf(Str, "Maps/%02d.kzm", Num_Stage.Max);
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
            glutSpecialFunc(Special_Game);
            glutSpecialUpFunc(Special_Up_Game);
            glutKeyboardFunc(Keyboard_Game);
        }
    }
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
        Switch_Map = 0;
        Init_Frag_Offset();
        Anim_Stage.Stt = -20;
        Anim_Stage.Draw = 1;
        Anim_Stage.Count = 0;
        Set_Anim_Stage_Text();
        Func_Timer = &Timer_Game;
        glutDisplayFunc(Display_Game);
    } else
        Main_Menu();
}

int Import_Map() {
    sprintf(Str, "Maps/%02d.kzm", Num_Stage.Current);
    FILE *f = fopen(Str, "rb");
    if (f == NULL)
        return 0;
    unsigned char s1 = 0, s2 = 0, c1, c2, c3;
    int ch;
    c1 = fgetc(f);
    c2 = fgetc(f);
    c3 = fgetc(f);
    int n = -1, m = 0;
    while (1) {
        ch = fgetc(f);
        if (ch == EOF)
            break;
        ch -= 1;
        s1 += ch;
        s2 += ch * n;
        n = -n;
        Arr_Map[m++] = ch;
    }
    fclose(f);
    m %= 256;
    if (c1 != s1 || c2 != s2 || c3 != m)
        return 0;
    m = 0;
    Start_X = Arr_Map[m++];
    Start_Y = Arr_Map[m++];
    Start_X *= 20;
    Start_Y *= 20;
    for (i = 0; i < NUM_FRAG; i++)
        for (j = LAST_Y; j >= 0; j--)
            for (k = 0; k < MAX_X; k++)
                Map[i][j][k] = Arr_Map[m++];
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
    Init_Image();
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
    Init_Virtual_Wall();
    Import_Map();
    Current_Width = WIDTH;
    Current_Height = HEIGHT;
    Set_View();
    for (i = 0; i < 19; i++)
        Loop20[i] = i + 1;
    Loop20[19] = 0;
}

void Win_Game() {
    Mix_PlayChannel(-1, Sound_Win, 0);
    Is_Win = 1;
    Is_Run = 0;
    glutKeyboardFunc(NULL);
    glutSpecialFunc(NULL);
    glutSpecialUpFunc(NULL);
}
