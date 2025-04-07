#include <Windows.h>
#include "Renderer_D2D.h"

namespace Engine {

void Renderer_D2D::Init(void* window_handle) {
	//Create Factory
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);

	//Get client rect for window size
	RECT r;
	GetClientRect((HWND)window_handle, &r);

	//Create render target
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties();
	D2D1_HWND_RENDER_TARGET_PROPERTIES hwnd_props = D2D1::HwndRenderTargetProperties((HWND)window_handle, D2D1::SizeU(r.right - r.left, r.bottom - r.top));

	factory->CreateHwndRenderTarget(props, hwnd_props, &render_target);
}

}