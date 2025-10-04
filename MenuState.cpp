#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include "MenuState.h"
#include "PlayState.h"
#include "CPPGP2025.h"
#include "Bitmap.h"

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "gdiplus.lib")

extern void ChangeState(GameState* newState);

MenuState::MenuState()
	: m_pBitmap(nullptr)
{
}

MenuState::~MenuState()
{
	if (m_pBitmap)
	{
		delete m_pBitmap;
		m_pBitmap = nullptr;
	}
}

void MenuState::Enter(HWND hWnd)
{
	std::cout << "Enter MenuState" << std::endl;

	mciSendString(L"open \"lostark_menu.mp3\" type mpegvideo alias mp3", NULL, 0, NULL);
	mciSendString(L"play mp3", NULL, 0, NULL);

	m_pBitmap = new BmpImage();
	if (!m_pBitmap->Load(L"lostark_menu.bmp")) {
		MessageBox(hWnd, L"BMP 파일 로드 실패!", L"Error", MB_ICONERROR);
	}
}

void MenuState::Exit()
{
	std::cout << "Exit MenuState" << std::endl;

	mciSendString(L"stop mp3", NULL, 0, NULL);
	mciSendString(L"close mp3", NULL, 0, NULL);

	if (m_pBitmap)
	{
		delete m_pBitmap;
		m_pBitmap = nullptr;
	}
}

void MenuState::Update(float elapsedTime)
{

}

void MenuState::Render(HDC hdc)
{
	// 화면을 검은색으로 지운다.
	//HWND hWnd = GetActiveWindow();
	//RECT rect;
	//GetClientRect(hWnd, &rect);
	//FillRect(hdc, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));

	if(m_pBitmap)
		m_pBitmap->Draw(hdc, 0, 0);
	/*
	RECT rect;
	GetClientRect(GetActiveWindow(), &rect);
	const TCHAR* text = L"Title Screen - Press Enter to Start";
	DrawText(hdc, text, -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	*/
}

void MenuState::OnKeyDown(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		std::cout << "Goto PlayState" << std::endl;
		ChangeState(new PlayState());
	}
}

void MenuState::OnKeyUp(WPARAM wParam)
{

}

void MenuState::OnMouseDown(int x, int y, int button)
{

}

void MenuState::OnMouseUp(int x, int y, int button)
{

}

void MenuState::OnMouseMove(int x, int y)
{

}