#pragma once
#include <d2d1.h>
#include <dwrite.h>
#include <wrl/client.h>
#include "../../Engine/Graphics/Renderer.h"

namespace Engine {

class Renderer_D2D : public Renderer {
public:
	Renderer_D2D() {}
	~Renderer_D2D() { Shutdown(); }

	void Init(void* window_handle) override;

	void BeginFrame() override { render_target->BeginDraw(); Clear(0, 0, 0, 1); }
	void EndFrame() override { render_target->EndDraw(); }

	void Clear(float r, float g, float b, float a) override { render_target->Clear(D2D1::ColorF(r, g, b, a)); }
	void Shutdown() override { if (render_target) render_target->Release(); if (factory) factory->Release(); }

private:
	ID2D1Factory* factory = nullptr;
	ID2D1HwndRenderTarget* render_target = nullptr;
};

}