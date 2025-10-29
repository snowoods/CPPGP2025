﻿#include "GameGlobal.h"
#include "Sheet.h"
#include "GameState.h"
#include "BasicRenderState.h"
#include <mmsystem.h> // timeGetTime()

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "gdiplus.lib")

GameState* g_currentState = nullptr;

void ChangeState(GameState* newState, ZGraphics& gfx)
{
    if (g_currentState)
    {
        g_currentState->Exit();
        SAFE_DELETE(g_currentState)
    }
    g_currentState = newState;
    g_currentState->Enter(gfx);
}

void SetupConsole()
{
    AllocConsole();
    FILE* pConsole;
    freopen_s(&pConsole, "CONOUT$", "w", stdout);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    SetupConsole();

    // 클라이언트 영역 크기
    const int clientWidth = 1024;
    const int clientHeight = 600;

    RECT windowRect = { 0, 0, clientWidth, clientHeight };
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    int windowWidth = windowRect.right - windowRect.left;
    int windowHeight = windowRect.bottom - windowRect.top;

    std::cout << "windowRect: " << windowWidth << " " << windowHeight << std::endl;

    // 화면 중앙 위치 계산
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    int x = (screenWidth - windowWidth) / 2;
    int y = (screenHeight - windowHeight) / 2;

	ZApp Game(_T("My D3D Test"), x, y, windowWidth, windowHeight, clientWidth, clientHeight);
    try
    {
        BOOL result = Game.Run();
        // 프로그램 종료 전 콘솔 해제
        FreeConsole();
        return result;
    }
    catch (const ChiliException& e)
    {
        MessageBoxA(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
    }
    catch (const std::exception& e)
    {
        MessageBoxA(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
    }
    catch (...)
    {
        MessageBoxA(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
    }

    return -1;
}

ZApp::ZApp(const TCHAR* pszCaption,
	DWORD XPos, DWORD YPos, DWORD Width, DWORD Height, DWORD ClientWidth, DWORD ClientHeight)
	: ZApplication(XPos, YPos, Width, Height, ClientWidth, ClientHeight)
{
    m_pGraphics = nullptr;
	_tcscpy_s(m_Caption, pszCaption);
}

ZApp::~ZApp()
{
	SAFE_DELETE(m_pGraphics);
}

DWORD ZApp::GetWidth()
{
	return m_Width;
}

DWORD ZApp::GetHeight()
{
	return m_Height;
}

LRESULT CALLBACK ZApp::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        timeBeginPeriod(1); // 시간 정밀도를 1ms로 설정
        break;
        
    case WM_DESTROY:
        timeEndPeriod(1); // 해제
        PostQuitMessage(0); // 메시지 루프를 종료하도록 요청
        break;

    case WM_KEYDOWN:
        if (g_currentState)
        {
            g_currentState->OnKeyDown(wParam);
        }
        break;

    case WM_KEYUP:
        if (g_currentState)
        {
            g_currentState->OnKeyUp(wParam);
        }
        break;

    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_MBUTTONDOWN:
        if (g_currentState)
        {
            g_currentState->OnMouseDown(LOWORD(lParam), HIWORD(lParam), (int)wParam);
        }
        break;

    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    case WM_MBUTTONUP:
        if (g_currentState)
        {
            g_currentState->OnMouseUp(LOWORD(lParam), HIWORD(lParam), (int)wParam);
        }
        break;

    case WM_MOUSEMOVE:
        if (g_currentState)
        {
            g_currentState->OnMouseMove(LOWORD(lParam), HIWORD(lParam));
        }
        break;        
    }

	return ZApplication::MsgProc(hWnd, uMsg, wParam, lParam);
}

BOOL ZApp::Shutdown()
{
	return TRUE;
}

BOOL ZApp::Init()
{
	m_pGraphics = new ZGraphics(GetHWnd(), 
        (float)m_ClientHeight / (float)m_ClientWidth
        , m_ClientWidth, m_ClientHeight);

    ChangeState(new BasicRenderState(), *m_pGraphics);

    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
    std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 2.0f);
    std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.3f);
    std::uniform_real_distribution<float> rdist(6.0f, 20.0f);
    for (auto i = 0; i < 80; i++)
    {
        boxes.push_back(std::make_unique<SampleBox>(
            *m_pGraphics, rng, adist,
            ddist, odist, rdist
        ));

        sheets.push_back(std::make_unique<Sheet>(
            *m_pGraphics, rng, adist,
            ddist, odist, rdist
        ));
    }
    m_pGraphics->SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, (float)m_ClientHeight / (float)m_ClientWidth, 0.5f, 40.0f));
    m_lastTime = timeGetTime();

	ShowMouse(TRUE);

	return TRUE;
}

BOOL ZApp::Frame()
{
    // timeGetTime() 함수는 시스템이 시작된 후 경과된 시간을 밀리초(ms) 단위로 반환합니다.
    DWORD timeValue = timeGetTime();
    // 경과 시간을 초 단위로 변환합니다. (1000ms = 1s)
    double dValue = timeValue / 1000.0;
    // sin 함수를 사용하여 시간의 흐름에 따라 0.0 ~ 1.0 사이를 부드럽게 왕복하는 값을 계산합니다.
    // sin(dValue)의 결과는 -1.0 ~ 1.0 이므로, 이를 0.0 ~ 1.0 범위로 정규화합니다.
    const float c = (float)sin(dValue) / 2.0f + 0.5f;
    // 계산된 'c' 값을 사용하여 화면 배경색을 동적으로 변경합니다.
    // Red와 Green 채널이 'c' 값에 따라 변하므로, 배경색이 파란색(0,0,1)과 청록색(1,1,1) 사이를 오가게 됩니다.
    m_pGraphics->ClearBuffer(c, c, 1.0f);
    m_pGraphics->SetViewport();

    // delta time
    DWORD currentTime = timeGetTime();
    float dt = (currentTime - m_lastTime) / 10000.0f;
    m_lastTime = currentTime;

    if (g_currentState)
    {
        g_currentState->Update(dt);
        g_currentState->Render(*m_pGraphics);
    }


	//m_pGraphics->DrawTriangle();
    //m_pGraphics->DrawIndexedTriangle();
    //m_pGraphics->DrawConstTriangle((float)dValue);


    // 고정위치
//    m_pGraphics->DrawDepthCube(
//        -(float)dValue,
//        0.0f,
//        0.0f
//    );
    
    // 윈도우 내에서 현재 마우스 위치
    RECT rect;
    GetWindowRect(GetHWnd(), &rect);
    POINT pt;
    GetCursorPos(&pt);
    pt.x -= rect.left;
    pt.y -= rect.top;
    std::cout << pt.x << " " << pt.y << std::endl;
    //m_pGraphics->DrawDepthCube((float)dValue, pt.x, pt.y);

//    m_pGraphics->DrawDepthCube(
//        (float)dValue,
//        ((float)pt.x / ((float)m_ClientWidth / 2.0f)) - 1.0f,
//        (-(float)pt.y / ((float)m_ClientHeight / 2.0f)) + 1.0f
//    );





    //for (auto& b : boxes)
    //{
    //    b->Update(dt);
    //    b->Render(*m_pGraphics);
    //}

    //for (auto& b : sheets)
    //{
    //    b->Update(dt);
    //    b->Render(*m_pGraphics);
    //}


    //m_pGraphics->DrawTexture();


	// 렌더링된 후면 버퍼를 화면에 표시합니다.
	m_pGraphics->EndFrame();
	return TRUE;
}