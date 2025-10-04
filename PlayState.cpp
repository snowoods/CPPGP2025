#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include "PlayState.h"
#include "MenuState.h"
#include "CPPGP2025.h"
#include "Bitmap.h"

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "gdiplus.lib")


extern void ChangeState(GameState* newState);

PlayState::PlayState()
	: m_pBitmap(nullptr)
{
}

PlayState::~PlayState()
{
	if (m_pBitmap)
	{
		delete m_pBitmap;
		m_pBitmap = nullptr;
	}
}

void PlayState::Enter(HWND hWnd)
{
	std::cout << "Enter PlayState" << std::endl;

	mciSendString(L"open \"lostark_gameplay.mp3\" type mpegvideo alias mp3", NULL, 0, NULL);
	mciSendString(L"play mp3", NULL, 0, NULL);

	m_pBitmap = new BmpImage();
	if (!m_pBitmap->Load(L"lostark_gameplay.bmp")) {
		MessageBox(hWnd, L"BMP 파일 로드 실패!", L"Error", MB_ICONERROR);
	}
}

void PlayState::Exit()
{
	std::cout << "Exit PlayState" << std::endl;

	mciSendString(L"stop mp3", NULL, 0, NULL);
	mciSendString(L"close mp3", NULL, 0, NULL);

	if (m_pBitmap)
	{
		delete m_pBitmap;
		m_pBitmap = nullptr;
	}
}

void PlayState::Update(float elapsedTime)
{

}

void PlayState::Render(HDC hdc)
{
    // 화면을 검은색으로 지운다.
    //RECT rect;
    //GetClientRect(g_hWnd, &rect);
    //FillRect(hdc, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));

    if (m_pBitmap)
        m_pBitmap->Draw(hdc, 0, 0);
	/*
	RECT rect;
	GetClientRect(GetActiveWindow(), &rect);
	const TCHAR* text = L"Title Screen - Press Enter to Start";
	DrawText(hdc, text, -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	*/
}

void PlayState::OnKeyDown(WPARAM wParam)
{
	if (wParam == 'M')
	{
		std::cout << "Goto MenuState" << std::endl;
		ChangeState(new MenuState());
	}
}

void PlayState::OnKeyUp(WPARAM wParam)
{

}

void PlayState::OnMouseDown(int x, int y, int button)
{

}

void PlayState::OnMouseUp(int x, int y, int button)
{

}

void PlayState::OnMouseMove(int x, int y)
{

}
