#pragma once
#include "Vec2.h" //iostream

struct Color {
	//Values between 0 and 1
	float r = 1.f, g = 1.f, b = 1.f, a = 1.f;

	Color() = default;
	Color(float rgb, float a = 1) : r(rgb), g(rgb), b(rgb), a(a) {}
	Color(float r, float g, float b, float a = 1) : r(r), g(g), b(b), a(a) {}
};
//ostream operator
inline std::ostream& operator<<(std::ostream& os, const Color& c) { return os << "r: " << c.r << " g: " << c.g << " b: " << c.b << " a: " << c.a; }

struct Line {
	Vec2f pos1, pos2;

	Line() = default;

	Line(Vec2f p1, Vec2f p2) : pos1(p1), pos2(p2) {}
	[[nodiscard]] inline float Length() const { return sqrt((pos2.x - pos1.x) * (pos2.x - pos1.x) + (pos2.y - pos1.y) * (pos2.y - pos1.y)); }
};
//ostream operator
inline std::ostream& operator<<(std::ostream& os, const Line& l) { return os << "p1: " << l.pos1 << " p2: " << l.pos2; }

struct Circle {
	Vec2f pos;
	float r = 0;

	Circle(Vec2f p, float r) : pos(p), r(r) {}

	[[nodiscard]] inline float Area() const { return r * r * 3.14159; }
	[[nodiscard]] inline float Circ() const { return 6.28318 * r; }
};
//ostream operator
inline std::ostream& operator<<(std::ostream& os, const Circle& c) { return os << "Circle pos: " << c.pos << "; Radius: " << c.r; }

struct Tri {
	Vec2f pos1, pos2, pos3;

	Tri(Vec2f p1, Vec2f p2, Vec2f p3) : pos1(p1), pos2(p2), pos3(p3) {}

	[[nodiscard]] inline float Area() const { return (pos1.x * (pos2.y - pos3.y) + pos2.x * (pos3.y - pos1.y) + pos3.x * (pos1.y - pos2.y)) * .5; }
};
//ostream operator
inline std::ostream& operator<<(std::ostream& os, const Tri& t) { return os << "p1: " << t.pos1 << " p2: " << t.pos2 << " p3: " << t.pos3; }

struct Rect {
	float x = 0, y = 0, w = 0, h = 0;

	Rect() = default;
	Rect(Vec2f p, int s) : x(p.x), y(p.y), w(s), h(s) {}
	Rect(Vec2f p, Vec2f s) : x(p.x), y(p.y), w(s.x), h(s.y) {}

	[[nodiscard]] inline float Area() const { return w * h; }
};
//ostream operator
inline std::ostream& operator<<(std::ostream& os, const Rect& r) { return os << "pos: " << Vec2f{ r.x, r.y } << " size: " << Vec2f{ r.w, r.h }; }