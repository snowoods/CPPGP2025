#include "ZD3D11.h"
#include "Sheet.h"
#include "SampleBox.h"
#include "BasicRenderState.h"
#include "GraphicsThrowMacros.h"
#include <d3dcompiler.h>
#include <DirectXMath.h> // dx math

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

extern void ChangeState(GameState* newState, ZGraphics& gfx);

BasicRenderState::BasicRenderState()
    : 
    _elapsedTime(0.0), 
    _deltaTime(0.0),
    _curX(0),
    _curY(0)
{
}

BasicRenderState::~BasicRenderState()
{
}

void BasicRenderState::Enter(ZGraphics& gfx)
{
    std::cout << "Enter BasicRenderState" << std::endl;

    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
    std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 2.0f);
    std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.3f);
    std::uniform_real_distribution<float> rdist(6.0f, 20.0f);
    for (auto i = 0; i < 80; i++)
    {
        boxes.push_back(std::make_unique<SampleBox>(
            gfx, rng, adist,
            ddist, odist, rdist
        ));

        sheets.push_back(std::make_unique<Sheet>(
            gfx, rng, adist,
            ddist, odist, rdist
        ));
    }

    pSpriteBatch = std::make_unique<dx::SpriteBatch>(GetContext(gfx));
    pTexture = std::make_unique<Bind::ZTexture>(gfx, L"dxlogo_256.bmp");
}

void BasicRenderState::Exit()
{
    std::cout << "Exit BasicRenderState" << std::endl;
}

void BasicRenderState::Update(float deltaTime)
{
    _elapsedTime += deltaTime;
    _deltaTime = deltaTime;

    for (auto& b : boxes) b->Update(deltaTime);
    for (auto& b : sheets) b->Update(deltaTime);
}

void BasicRenderState::Render(ZGraphics& gfx)
{
    //DrawTriangle(gfx);
    //DrawIndexedTriangle(gfx);
    //DrawConstTriangle(gfx, _elapsedTime);

    //DrawDepthCube(gfx,
    //    float(-_elapsedTime),
    //    0.0f,
    //    0.0f
    //);
    //DrawDepthCube(gfx, 
    //    float(_elapsedTime), 
    //    (((float)_curX / ((float)GetClientWidth(gfx) / 2.0f)) - 1.0f) * 2,
    //    ((-(float)_curY / ((float)GetClientHeight(gfx) / 2.0f)) + 1.0f) * 2
    //); // using face color

    //DrawTexture(gfx);

    for (auto& b : boxes)
    {
        b->Render(gfx);
    }

    for (auto& b : sheets)
    {
        b->Render(gfx);
    }
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
    _curX = x;
    _curY = y;
    std::cout << _curX << " " << _curY << std::endl;
}
