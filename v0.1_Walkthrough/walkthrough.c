// Space
void WT_Func_0() {
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
}

// Left
void WT_Func_1() {
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

// Right
void WT_Func_2() {
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

// Up
void WT_Func_3() {
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

// Down
void WT_Func_4() {
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

// Jump
void WT_Func_5() {
    if (Jump_Stt == -1) {
        Jump_Stt = 0;
        v = BASE_V;
        if (!Mix_Playing(-1))
            Mix_PlayChannel(-1, Sound_Jump, 0);
    }
}

// Hold Left
void WT_Func_6() {
    Is_Run = 1;
    Drt = 0;
    Mx = -5.0f;
}

// Hold Right
void WT_Func_7() {
    Is_Run = 1;
    Drt = 1;
    Mx = 5.0f;
}

// Release
void WT_Func_8() {
    Is_Run = 0;
    glutPostRedisplay();
}

void Set_Walkthrough(){
	WT_Func[0]=&WT_Func_0;
	WT_Func[1]=&WT_Func_1;
	WT_Func[2]=&WT_Func_2;
	WT_Func[3]=&WT_Func_3;
	WT_Func[4]=&WT_Func_4;
	WT_Func[5]=&WT_Func_5;
	WT_Func[6]=&WT_Func_6;
	WT_Func[7]=&WT_Func_7;
	WT_Func[8]=&WT_Func_8;
}

void WT_In_Game(){
	WT_Happen=1;
	WT_Stt=0;
	WT_Count=0;
	WT_Wait=WT_Action[Num_Stage.Current][WT_Count].Wait;
}
