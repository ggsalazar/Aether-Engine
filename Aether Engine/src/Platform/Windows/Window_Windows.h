#pragma once
#include <Windows.h>
#include <vector>
#include "../../Engine/Graphics/Window.h"

namespace Engine {

class Window_Windows : public Window {
public:
	bool should_close = false;

	Window_Windows() {}
	~Window_Windows() { Destroy(); }

	bool Create(Vector2u size, const char* title, uint monitor_index = 0, bool borderless = true) override;
	void Destroy() override;

	void PollEvents() override;

	void Clear() override;
	void Display() override;

	Vector2u GetSize() const override;
	HWND GetHandle() const { return hwnd; }

private:
	HWND hwnd = nullptr;
	HDC hdc = nullptr;

	struct MonitorInfo {
		HMONITOR handle;
		RECT rect;
	};

	std::vector<MonitorInfo> EnumerateMonitors() {
		std::vector<MonitorInfo> monitors;

		EnumDisplayMonitors(nullptr, nullptr,
			[](HMONITOR hMon, HDC, LPRECT, LPARAM data) -> BOOL {
				MONITORINFO info = {};
				info.cbSize = sizeof(info);
				if (GetMonitorInfo(hMon, &info)) {
					auto& out = *reinterpret_cast<std::vector<MonitorInfo>*>(data);
					out.push_back({ hMon, info.rcMonitor });
				}
				return TRUE;
			},
			reinterpret_cast<LPARAM>(&monitors)
		);

		return monitors;
	}

};

}