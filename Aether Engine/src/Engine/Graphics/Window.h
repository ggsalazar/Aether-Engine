#pragma once
#include "../Core/Vector2.h"

namespace Engine {

class Window {
public:
	virtual ~Window() = default;

	virtual bool Create(Vector2u size, const char* title, uint monitor_index, bool borderless) = 0;
	virtual void Destroy() = 0;

	virtual void PollEvents() = 0;

	virtual void Clear() = 0;
	virtual void Display() = 0;

	virtual Vector2u GetSize() const = 0;
};

}