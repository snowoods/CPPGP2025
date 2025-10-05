#include "ZD3D11.h"
#include <iostream>
#include <d3d12.h> // D3D12 헤더 추가

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib, "d3d12.lib") // D3D12 라이브러리 추가

void ZGraphics::CheckMaxD3D12FeatureLevel()
{
	// --- 시스템의 최대 D3D 기능 수준 확인 ---
	// D3D12를 포함하여 시스템이 지원하는 가장 높은 기능 수준을 확인합니다.
	// 실제 렌더링은 D3D11로 진행되지만, 시스템의 D3D12 지원 현황을 파악하기 위한 목적입니다.
	const D3D_FEATURE_LEVEL featureLevelsToCheck[] = {
		D3D_FEATURE_LEVEL_12_2,
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
	};

	bool d3d12Supported = false;
	for (D3D_FEATURE_LEVEL level : featureLevelsToCheck)
	{
		ID3D12Device* pDevice12 = nullptr;
		HRESULT hr12 = D3D12CreateDevice(nullptr, level, __uuidof(ID3D12Device), (void**)&pDevice12);
		if (SUCCEEDED(hr12))
		{
			switch (level)
			{
				case D3D_FEATURE_LEVEL_12_2: std::cout << "Max Supported D3D Feature Level: 12.2" << std::endl; break;
				case D3D_FEATURE_LEVEL_12_1: std::cout << "Max Supported D3D Feature Level: 12.1" << std::endl; break;
				case D3D_FEATURE_LEVEL_12_0: std::cout << "Max Supported D3D Feature Level: 12.0" << std::endl; break;
			}
			pDevice12->Release();
			d3d12Supported = true;
			break; // 가장 높은 지원 수준을 찾았으므로 루프를 종료합니다.
		}
	}

	if (!d3d12Supported)
	{
		std::cout << "D3D12 is not supported. The highest feature level will be determined by D3D11." << std::endl;
	}
}

ZGraphics::ZGraphics(HWND hWnd)
{
	CheckMaxD3D12FeatureLevel();

	// --- D3D11 초기화 시작 ---

	// 스왑 체인을 설정하기 위한 구조체입니다.
	DXGI_SWAP_CHAIN_DESC sd = {};
	// 후면 버퍼의 너비와 높이를 설정합니다. 0으로 설정하면 윈도우 크기에 맞춰 자동으로 조절됩니다.
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	// 후면 버퍼의 픽셀 포맷을 설정합니다. (B8G8R8A8_UNORM: 32비트, 채널당 8비트, BGRA 순서)
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	// 화면 주사율(Refresh Rate)을 설정합니다. 0으로 설정하면 시스템 기본값을 사용합니다.
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	// 디스플레이 스케일링 모드를 설정합니다.
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	// 스캔 라인 렌더링 순서를 설정합니다.
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	// 멀티샘플링(MSAA) 설정을 합니다. Count=1, Quality=0은 멀티샘플링을 사용하지 않음을 의미합니다.
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	// 후면 버퍼의 사용 용도를 설정합니다. 렌더 타겟으로 사용됩니다.
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// 후면 버퍼의 개수를 설정합니다. 일반적으로 1개(더블 버퍼링) 또는 2개(트리플 버퍼링)를 사용합니다.
	sd.BufferCount = 1;
	// 렌더링 결과를 출력할 윈도우의 핸들입니다.
	sd.OutputWindow = hWnd;
	// 창 모드(TRUE) 또는 전체 화면 모드(FALSE)를 설정합니다.
	sd.Windowed = TRUE;
	// 후면 버퍼를 화면에 표시한 후의 처리 방식을 설정합니다. (DISCARD: 내용을 버림)
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	// 추가적인 플래그를 설정합니다.
	sd.Flags = 0;

	// D3D11CreateDeviceAndSwapChain은 D3D12 기능 수준을 직접 생성할 수 없지만,
	// 최신 하드웨어에서 지원하는 가장 높은 D3D11 수준부터 오래된 D3D9 수준까지 순서대로 시도합니다.
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	D3D_FEATURE_LEVEL featureLevel;

	UINT creationFlags = 0;
#if defined(_DEBUG)
	// 디버그 빌드에서는 D3D11 디버그 레이어를 활성화합니다.
	// 이를 통해 API 사용 오류, 메모리 누수 등 다양한 디버깅 정보를 얻을 수 있습니다.
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// D3D11 장치(Device), 장치 컨텍스트(Context), 스왑 체인(Swap Chain)을 생성합니다.
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr,						// pAdapter: 기본 어댑터(그래픽 카드)를 사용합니다.
		D3D_DRIVER_TYPE_HARDWARE,		// DriverType: 하드웨어 가속을 사용합니다.
		nullptr,						// Software: 하드웨어 드라이버를 사용하므로 NULL입니다.
		creationFlags,					// Flags: 추가적인 생성 플래그 (예: 디버그 모드)
		featureLevels,				// pFeatureLevels: 지원할 기능 수준 배열입니다.
		numFeatureLevels,				// FeatureLevels: 기능 수준 배열의 크기입니다.
		D3D11_SDK_VERSION,			// SDKVersion: 항상 D3D11_SDK_VERSION으로 설정합니다.
		&sd,						// pSwapChainDesc: 위에서 설정한 스왑 체인 설정 구조체입니다.
		&pSwap,						// ppSwapChain: 생성된 스왑 체인 인터페이스를 받을 포인터입니다.
		&pDevice,					// ppDevice: 생성된 장치 인터페이스를 받을 포인터입니다.
		&featureLevel,				// pFeatureLevel: 실제로 선택된 기능 수준을 받을 포인터입니다.
		&pContext					// ppImmediateContext: 생성된 장치 컨텍스트를 받을 포인터입니다.
	);

	if (SUCCEEDED(hr))
	{
		switch (featureLevel)
		{
			case D3D_FEATURE_LEVEL_11_1:
				std::cout << "D3D Feature Level: 11.1" << std::endl;
				break;
			case D3D_FEATURE_LEVEL_11_0:
				std::cout << "D3D Feature Level: 11.0" << std::endl;
				break;
			case D3D_FEATURE_LEVEL_10_1:
				std::cout << "D3D Feature Level: 10.1" << std::endl;
				break;
			case D3D_FEATURE_LEVEL_10_0:
				std::cout << "D3D Feature Level: 10.0" << std::endl;
				break;
			case D3D_FEATURE_LEVEL_9_3:
				std::cout << "D3D Feature Level: 9.3" << std::endl;
				break;
			case D3D_FEATURE_LEVEL_9_2:
				std::cout << "D3D Feature Level: 9.2" << std::endl;
				break;
			case D3D_FEATURE_LEVEL_9_1:
				std::cout << "D3D Feature Level: 9.1" << std::endl;
				break;
			default:
				std::cout << "D3D Feature Level: Unknown or lower than 9.1" << std::endl;
				break;
		}
	}

	// 스왑 체인으로부터 후면 버퍼에 접근하기 위한 리소스를 가져옵니다.
	ID3D11Resource* pBackBuffer = nullptr;
	pSwap->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer));

	// 후면 버퍼에 대한 렌더 타겟 뷰를 생성합니다.
	// 렌더링 파이프라인의 최종 출력은 이 렌더 타겟 뷰에 쓰여집니다.
	pDevice->CreateRenderTargetView(
		pBackBuffer,		// pResource: 렌더 타겟으로 사용할 리소스 (후면 버퍼)
		nullptr,			// pDesc: 렌더 타겟 뷰 설정, NULL이면 리소스의 기본 설정을 따릅니다.
		&pTarget			// ppRTView: 생성된 렌더 타겟 뷰를 받을 포인터입니다.
	);

	// 렌더 타겟 뷰를 생성했으므로 후면 버퍼 리소스는 더 이상 필요 없으므로 해제합니다.
	pBackBuffer->Release();
}

ZGraphics::~ZGraphics()
{
	// 생성된 순서의 역순으로 COM 객체들을 안전하게 해제합니다.
	// SAFE_RELEASE 매크로를 사용하면 더 안전하게 처리할 수 있습니다.
	if (pTarget != nullptr)
	{
		pTarget->Release();
	}
	if (pContext != nullptr)
	{
		pContext->Release();
	}
	if (pSwap != nullptr)
	{
		pSwap->Release();
	}
	if (pDevice != nullptr)
	{
		pDevice->Release();
	}
}

void ZGraphics::EndFrame()
{
	// 후면 버퍼의 내용을 화면에 표시(Present)합니다.
	// 첫 번째 인자(SyncInterval): 수직 동기화 옵션. 1은 수직 동기화를 켭니다.
	// 두 번째 인자(Flags): 추가적인 프레젠테이션 옵션.
	pSwap->Present(1u, 0u);
}

void ZGraphics::ClearBuffer(float red, float green, float blue) noexcept
{
	// 렌더 타겟 뷰를 지정된 색상으로 초기화합니다.
	const float color[] = { red,green,blue,1.0f }; // RGBA 순서
	pContext->ClearRenderTargetView(pTarget, color);
}