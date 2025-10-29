#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include "ZD3D11.h"
#include "BasicRenderState.h"
#include <sstream>
#include "GraphicsThrowMacros.h"
#include <d3dcompiler.h>
#include <DirectXMath.h> // dx math

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

extern void ChangeState(GameState* newState, ZGraphics& gfx);

BasicRenderState::BasicRenderState()
{
}

BasicRenderState::~BasicRenderState()
{
}

void BasicRenderState::Enter(ZGraphics& gfx)
{
    std::cout << "Enter BasicRenderState" << std::endl;
}

void BasicRenderState::Exit()
{
    std::cout << "Exit BasicRenderState" << std::endl;
}

void BasicRenderState::Update(float elapsedTime)
{

}

void BasicRenderState::Render(ZGraphics& gfx)
{
    DrawTriangle(gfx);
}

void BasicRenderState::OnKeyDown(WPARAM wParam)
{
    if (wParam == VK_RETURN)
    {
        std::cout << "Goto PlayState" << std::endl;
        //ChangeState(new PlayState());
    }
}

void BasicRenderState::OnKeyUp(WPARAM wParam)
{

}

void BasicRenderState::OnMouseDown(int x, int y, int button)
{

}

void BasicRenderState::OnMouseUp(int x, int y, int button)
{

}

void BasicRenderState::OnMouseMove(int x, int y)
{

}

//-----

void BasicRenderState::DrawTriangle(ZGraphics& gfx)
{
    INFOMAN(gfx);

    struct Vertex
    {
        struct
        {
            float x;
            float y;
        } pos;
    };

    // NDC 화면 가운데가 (0,0), X(-1,1), Y(-1,1)
    // 왼손좌표계 앞면 : CW (Clock Wise)
    Vertex vertices[] =
    {
        { 0.0f, 0.5f },
        { 0.5f, -0.5f },
        { -0.5f, -0.5f },
    };

    // Create VertexBuffer
    wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
    D3D11_BUFFER_DESC bd = {};
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.CPUAccessFlags = 0u;
    bd.MiscFlags = 0u;
    bd.ByteWidth = sizeof(vertices);
    bd.StructureByteStride = sizeof(Vertex);

    D3D11_SUBRESOURCE_DATA sd = {};
    sd.pSysMem = vertices;
    GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&bd, &sd, &pVertexBuffer));

    // Bind vertex buffer to pipeline
    const UINT stride = sizeof(Vertex);
    const UINT offset = 0u;
    GetContext(gfx)->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

    // Create pixel shader
    wrl::ComPtr<ID3D11PixelShader> pPixelShader;
    wrl::ComPtr<ID3DBlob> pBlob;
    GFX_THROW_INFO(D3DReadFileToBlob(L"./x64/Debug/PixelShader.cso", &pBlob));
    GFX_THROW_INFO(GetDevice(gfx)->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));

    // bind pixel shader
    GetContext(gfx)->PSSetShader(pPixelShader.Get(), nullptr, 0u);

    // Create vertex shader
    wrl::ComPtr<ID3D11VertexShader> pVertexShader;
    GFX_THROW_INFO(D3DReadFileToBlob(L"./x64/Debug/VertexShader.cso", &pBlob));
    GFX_THROW_INFO(GetDevice(gfx)->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));

    // Bind vertex shader
    GetContext(gfx)->VSSetShader(pVertexShader.Get(), nullptr, 0u);

    // VertexShader의 바이트코드(pBlob)가 필요하기 때문에 PixelShader를 먼저 생성한다.
    wrl::ComPtr<ID3D11InputLayout> pInputLayout;
    const D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        { "Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    GFX_THROW_INFO(GetDevice(gfx)->CreateInputLayout(
        ied, (UINT)std::size(ied),
        pBlob->GetBufferPointer(),
        pBlob->GetBufferSize(),
        &pInputLayout
    ));
    // Bind vertex layout
    GetContext(gfx)->IASetInputLayout(pInputLayout.Get());

    // Bind render target
    // GetTarget(gfx)는 ID3D11RenderTargetView* 포인터를 반환하므로
    // OMSetRenderTargets에 전달하기 위해 주소 연산자(&)를 사용합니다.
    ID3D11RenderTargetView* pTarget = GetTarget(gfx);
    GetContext(gfx)->OMSetRenderTargets(1u, &pTarget, nullptr);

    // Set primitive topology to triangle list (groups of 3 vertices)
    GetContext(gfx)->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Configure viewport
    D3D11_VIEWPORT vp;
    vp.Width = (float)GetClientWidth(gfx);
    vp.Height = (float)GetClientHeight(gfx);
    vp.MinDepth = 0;
    vp.MaxDepth = 1;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    GetContext(gfx)->RSSetViewports(1u, &vp);

    GFX_THROW_INFO_ONLY(GetContext(gfx)->Draw((UINT)std::size(vertices), 0u));
}
