#pragma once
#include <Windows.h>
#include "../Core/Vector2.h"

namespace Display {
	inline Vector2u ScreenSize() {
		return {
			(uint)GetSystemMetrics(SM_CXSCREEN), // Screen width in pixels
			(uint)GetSystemMetrics(SM_CYSCREEN)  // Screen height in pixels
		};
	}

	inline Vector2u MinRes() { return { 640, 320 }; }
}