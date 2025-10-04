#include <windows.h>
#include <gdiplus.h>
#include <mmsystem.h>
#include <iostream>
#include "TitleState.h"
#include "MenuState.h"
#include "CPPGP2025.h"
#include "Bitmap.h"

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "gdiplus.lib")

extern void ChangeState(GameState* newState);

TitleState::TitleState()
	: m_pBitmap(nullptr)
{
}

TitleState::~TitleState()
{
	if (m_pBitmap)
	{
		delete m_pBitmap;
		m_pBitmap = nullptr;
	}
}

void TitleState::Enter(HWND hWnd)
{
	std::cout << "Enter TitleState" << std::endl;

	// MCI(Media Control Interface)를 사용하여 음악 파일을 엽니다.
	// open: 미디어 파일을 엽니다.
	// \"lostark_title.mp3\": 열고자 하는 파일의 경로입니다. (실행 파일과 같은 경로에 있어야 함)
	// type mpegvideo: 미디어 파일의 타입을 지정합니다. mp3의 경우 mpegvideo를 사용합니다.
	// alias mp3: 이 미디어에 "mp3"라는 별칭을 부여합니다. 이후 이 별칭으로 미디어를 제어할 수 있습니다.
	mciSendString(L"open \"lostark_title.mp3\" type mpegvideo alias mp3", NULL, 0, NULL);

	// "mp3" 별칭으로 열린 미디어를 재생합니다.
	mciSendString(L"play mp3", NULL, 0, NULL);

	m_pBitmap = new BmpImage();
	if (!m_pBitmap->Load(L"lostark_title1.bmp")) {
		MessageBox(hWnd, L"BMP 파일 로드 실패!", L"Error", MB_ICONERROR);
	}

	InvalidateRect(hWnd, NULL, FALSE);
}

void TitleState::Exit()
{
	std::cout << "Exit TitleState" << std::endl;

	// "mp3" 별칭으로 열린 미디어의 재생을 멈춥니다.
	mciSendString(L"stop mp3", NULL, 0, NULL);

	// "mp3" 별칭으로 열린 미디어를 닫고 관련 리소스를 해제합니다.
	mciSendString(L"close mp3", NULL, 0, NULL);

	if (m_pBitmap)
	{
		delete m_pBitmap;
		m_pBitmap = nullptr;
	}
}

void TitleState::Update(float elapsedTime)
{

}

void TitleState::Render(HDC hdc)
{
	// 화면을 검은색으로 지운다.
	//HWND hWnd = GetActiveWindow();
	//RECT rect;
	//GetClientRect(hWnd, &rect);
	//FillRect(hdc, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));

	if (m_pBitmap)
		m_pBitmap->Draw(hdc, 0, 0);
}

void TitleState::OnKeyDown(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		ChangeState(new MenuState());
	}
}

void TitleState::OnKeyUp(WPARAM wParam)
{

}

void TitleState::OnMouseDown(int x, int y, int button)
{

}

void TitleState::OnMouseUp(int x, int y, int button)
{

}

void TitleState::OnMouseMove(int x, int y)
{

}
