#pragma once
#include "Vec2.h" //iostream

struct Color {
	//Values between 0 and 1
	float r = 0.f, g = 0.f, b = 0.f, a = 0.f;

	Color() = default;
	Color(float rgb, float a = 1) : r(rgb), g(rgb), b(rgb), a(a) {}
	Color(float r, float g, float b, float a = 1) : r(r), g(g), b(b), a(a) {}
};
//ostream operator
template <typename T>
std::ostream& operator<<(std::ostream& os, const Color& c) { return os << "r: " << c.r << " g: " << c.g << " b: " << c.b << " a: " << c.a; }

struct Line {
	int x1 = 0, x2 = 0, y1 = 0, y2 = 0;

	Line() = default;

	Line(int x_1, int x_2, int y_1, int y_2) : x1(x_1), x2(x_2), y1(y_1), y2(y_2) {}
	Line(Vec2i p1, Vec2i p2) : x1(p1.x), x2(p2.x), y1(p1.y), y2(p2.y) {}
	inline float Length() const { return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)); }
};
//ostream operator
template <typename T>
std::ostream& operator<<(std::ostream& os, const Line& l) { return os << "p1: " << Vec2i(l.x1, l.y1) << " p2: " << Vec2i(l.x2, l.y2); }

struct Circle {
	int x = 0, y = 0;
	float r = 0;

	Circle(Vec2i p, float r) : x(p.x), y(p.y), r(r) {}

	inline float Area() const { return r * r * 3.14159; }
	inline float Circ() const { return 6.28318 * r; }
};
//ostream operator
template <typename T>
std::ostream& operator<<(std::ostream& os, const Circle& c) { return os << "Circle pos: " << Vec2i{ c.x, c.y } << "; Radius: " << c.r; }

struct Tri {
	Vec2i pos1, pos2, pos3;

	Tri(Vec2i p1, Vec2i p2, Vec2i p3) : pos1(p1), pos2(p2), pos3(p3) {}

	inline float Area() const { return (pos1.x * (pos2.y - pos3.y) + pos2.x * (pos3.y - pos1.y) + pos3.x * (pos1.y - pos2.y)) * .5; }
};
//ostream operator
template <typename T>
std::ostream& operator<<(std::ostream& os, const Tri& t) { return os << "p1: " << t.pos1 << " p2: " << t.pos2 << " p3: " << t.pos3; }

struct Rect {
	int x = 0, y = 0, w = 0, h = 0;

	Rect() = default;
	Rect(Vec2i p, int s) : x(p.x), y(p.y), w(s), h(s) {}
	Rect(Vec2i p, Vec2i s) : x(p.x), y(p.y), w(s.x), h(s.y) {}

	inline float Area() const { return w * h; }
};
//ostream operator
template <typename T>
std::ostream& operator<<(std::ostream& os, const Rect& r) { return os << "pos: " << Vec2i{ r.x, r.y } << " size: " << Vec2i{ r.w, r.h }; }