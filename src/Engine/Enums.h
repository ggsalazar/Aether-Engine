#pragma once
#include <string>
#include "Math/Geometry.h"

using std::string;

//Static casting helper function
inline constexpr size_t scst(auto e) { return static_cast<size_t>(e); }

enum class ColorName {
	NONE,
	Black,
	Blue,
	Brown,
	Green,
	Orange,
	Purple,
	Red,
	Turquoise,
	White,
	Yellow
};
inline Color StringToColor(const string c) {
	if (c == "Black") return {0};
	if (c == "Blue") return {0,0,1};
	if (c == "Brown") return {.5,.3,.1};
	if (c == "Green") return {0,1,0};
	if (c == "Orange") return {1,.6,0};
	if (c == "Purple") return {1,0,1};
	if (c == "Red") return {1,0,0};
	if (c == "Turquoise") return {0,1,1};
	if (c == "White") return {1};
	if (c == "Yellow") return {1,1,0};
	if (c == "NONE" or c == "None") return {0, 0};
}

enum class FontName {
	NONE,
	Normal
};

enum class LayerName {
	NONE = 0,
	Objects,
	UI,
	COUNT
};

enum class MenuName {
	Confirm_Quit,
	GO,
	Main,
	Options,
	Pause,
};

enum class Scene {
	NONE,
	Game,
	Title
};

enum class SFX {

};

enum class Song {
	NONE
};

enum class Terrain {
	NONE,
	Normal,
	Water
};

enum class Widget {
	NONE,
	//Buttons
	Apply,
	Back,
	Close,
	Fullscreen,
	Options,
	Quit,
	Resume,
	Title,
	//Pickers
	Resolution,
	//Sliders
	Msc_V,
	SFX_V
};