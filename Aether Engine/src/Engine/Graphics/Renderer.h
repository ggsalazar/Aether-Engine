#pragma once

namespace Engine {
class Renderer {
public:
	virtual ~Renderer() {}

	virtual void Init(void* window_handle) = 0;
	virtual void BeginFrame() = 0;
	virtual void EndFrame() = 0;
	virtual void Clear(float r, float g, float b, float a) = 0;
	virtual void Shutdown() = 0;
};
}