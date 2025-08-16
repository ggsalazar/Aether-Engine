#pragma once
#include "Geometry.h"

class Camera {
public:
	//Viewport x/y is camera pos, pos+size is current view area
	Rect viewport;

	Camera() = default;
	Camera(Vec2i pos) { viewport.x = pos.x; viewport.y = pos.y; }
	Camera(Vec2i pos, Vec2i size) { viewport.x = pos.x; viewport.y = pos.y; viewport.w = size.x; viewport.h = size.y; }

	inline void MoveTo(Vec2i new_pos) { viewport.x = new_pos.x; viewport.y = new_pos.y; }
	inline void MoveCenterTo(Vec2i new_pos) { viewport.x = new_pos.x - viewport.w * .5f; viewport.y = new_pos.y - viewport.h * .5f; }
	inline void MoveBy(Vec2i offset) { viewport.x += offset.x; viewport.y += offset.y; }

	inline Vec2i GetPos() const { return { viewport.x, viewport.y }; }
	inline Vec2i GetCenter() const { return Round((viewport.x + viewport.w * .5f), (viewport.y + viewport.h * .5f)); }

};