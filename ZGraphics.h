﻿#pragma once

// D3D 11의 초기화 및 핵심 인터페이스 관리

class ZGraphics
{
private:
	ID3D11Device* pDevice = nullptr;			// D3D11 장치, 리소스 생성 및 관리
	IDXGISwapChain* pSwap = nullptr;			// 스왑 체인, 후면 버퍼와 화면 출력을 교체
	ID3D11DeviceContext* pContext = nullptr;	// D3D11 장치 컨텍스트, 렌더링 명령을 GPU에 전달
	ID3D11RenderTargetView* pTarget = nullptr;	// 렌더 타겟 뷰, 렌더링 결과가 저장되는 곳

public:
	ZGraphics(HWND hWnd);

	// 복사 생성자와 대입 연산자를 사용하지 않는다. (객체 복사 방지)
	// -- D3D 리소스 고유하게 유지
	ZGraphics(const ZGraphics&) = delete;
	ZGraphics& operator=(const ZGraphics&) = delete;

	~ZGraphics();

	// 현재 프레임의 렌더링을 끝내고 후면 버퍼를 화면에 표시한다.
	void EndFrame();
	// 후면 버퍼를 지정된 색상으로 초기화
	// red (0.0f ~ 1.0f)
	// green (0.0f ~ 1.0f)
	// blue (0.0f ~ 1.0f)
	void ClearBuffer(float red, float green, float blue) noexcept;
};
