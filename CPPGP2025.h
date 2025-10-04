#pragma once

#include <windows.h> // HWND 타입을 위해 추가

class GameState;
void ChangeState(GameState* newState);

extern HWND g_hWnd; // g_hWnd를 외부 변수로 선언