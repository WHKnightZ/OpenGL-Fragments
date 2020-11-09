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

void Action_Switch_Frag() {
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

void Timer_Menu() {
    Main_Arrow.Stt = Main_Arrow.Loop[Main_Arrow.Stt];
    glutPostRedisplay();
}

void Timer_Game() {
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
            Win_Game();
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
