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

void Special_Game(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_LEFT:
        Is_Run = 1;
        Drt = 0;
        Mx = -5.0f;
        break;
    case GLUT_KEY_RIGHT:
        Is_Run = 1;
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

void Special_Up_Game(int key, int x, int y) {
    if (key == Def_Drt[Drt]) {
        Is_Run = 0;
        glutPostRedisplay();
    }
}

void Keyboard_Game(unsigned char key, int x, int y) {
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
            Is_Run = 0;
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
    } else if (key == 27) {
        Current_Width = WIDTH;
        Current_Height = HEIGHT;
        Time_Switch_Frag = TIME_SWITCH_FRAG;
        Time_Switch_Map = TIME_SWITCH_MAP;
        Set_View();
        Main_Menu();
    }
}

void Special_Switch(int key, int x, int y) {
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
