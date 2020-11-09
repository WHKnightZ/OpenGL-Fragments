void Move_Left() {
    Tmp_X = (int)(px + ADD_LEFT_2) / SIZE - MAX_X;
    Tmp_Y = (py + 10) / SIZE;
    Tmp_Y2 = (py + 50) / SIZE;
    if (Tmp_Y2 >= MAX_Y) {
        Tmp_Y2 -= MAX_Y;
        Tmp_CM_1 = CM;
        Tmp_CM_2 = Frag_Map[CFY + 1][CFX];
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
