#pragma once
#include "GameState.h"
#include "Bitmap.h"

class TitleState : public GameState
{
private:
	BmpImage* m_pBitmap;
public:
	TitleState();
	~TitleState();

	void Enter(HWND hWnd) override;
	void Exit() override;
	void Update(float elapsedTime) override;
	void Render(HDC hdc) override;
	void OnKeyDown(WPARAM wParam) override;
	void OnKeyUp(WPARAM wParam) override;
	void OnMouseDown(int x, int y, int button) override;
	void OnMouseUp(int x, int y, int button) override;
	void OnMouseMove(int x, int y) override;
};