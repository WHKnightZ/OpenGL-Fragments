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
