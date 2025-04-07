#include <windowsx.h>
#include "Window_Windows.h"

namespace Engine {

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

bool Window_Windows::Create(Vector2u size, const char* title, uint monitor_index, bool borderless) {
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = GetModuleHandle(nullptr);
	wc.lpszClassName = L"AetherEngineWindowClass";

	RegisterClass(&wc);

	//Get the information of the specified monitor
	auto monitors = EnumerateMonitors();
	monitor_index = monitor_index >= monitors.size() ? 0 : monitor_index;
	RECT r = monitors[monitor_index].rect;

	//Convert the title from a const char* to a wide string
	std::wstring wtitle = std::wstring(title, title + strlen(title));

	//Make system aware of DPI scaling (no manual adjustment needed)
	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE);

	hwnd = CreateWindowEx(0, wc.lpszClassName, wtitle.c_str(), WS_OVERLAPPEDWINDOW, r.left, r.top, size.x, size.y, 0, 0, wc.hInstance, 0);
	if (!hwnd) {
		std::cout << "Window creation failed" << std::endl;
		return false;
	}

	//Cannot create the window borderless for some fucking reason that I can't figure out, so have to change it after the fact
	if (borderless)
		SetWindowLong(hwnd, GWL_STYLE, WS_POPUP);

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	hdc = GetDC(hwnd);

	return true;
}

void Window_Windows::Destroy() {
	if (hwnd) {
		ReleaseDC(hwnd, hdc);
		DestroyWindow(hwnd);
		hwnd = nullptr;
	}
}

void Window_Windows::PollEvents() {
	MSG msg = {};
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) should_close = true;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void Window_Windows::Clear() {
	RECT rect;
	GetClientRect(hwnd, &rect);
	HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
	FillRect(hdc, &rect, brush);
	DeleteObject(brush);
}

void Window_Windows::Display() {}

Vector2u Window_Windows::GetSize() const {
	RECT rect;
	GetClientRect(hwnd, &rect);
	return { (uint)(rect.right - rect.left), (uint)(rect.bottom - rect.top) };
}

}