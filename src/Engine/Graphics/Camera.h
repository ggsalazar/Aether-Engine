#pragma once
#include "../Math/Geometry.h" //Vec2 (iostream)

class Camera {
public:
	//Viewport x/y is camera pos, pos+size is current view area
	Rect viewport;

	Camera() = default;
	Camera(Vec2f pos) { viewport.x = pos.x; viewport.y = pos.y; }
	Camera(Vec2f pos, Vec2f size) { viewport.x = pos.x; viewport.y = pos.y; viewport.w = size.x; viewport.h = size.y; }

	inline void MoveTo(Vec2f new_pos) { viewport.x = new_pos.x; viewport.y = new_pos.y; }
	inline void MoveCenterTo(Vec2f new_pos) { viewport.x = new_pos.x - viewport.w * .5f; viewport.y = new_pos.y - viewport.h * .5f; }
	inline void MoveBy(Vec2f offset) { viewport.x += offset.x; viewport.y += offset.y; }

	[[nodiscard]] inline Vec2f GetPos() const { return { viewport.x, viewport.y }; }
	[[nodiscard]] inline Vec2f GetCenter() const { return Round((viewport.x + viewport.w * .5f), (viewport.y + viewport.h * .5f)); }

};