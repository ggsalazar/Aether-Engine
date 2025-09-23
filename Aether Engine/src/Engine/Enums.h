#pragma once

//Engine
enum class Scene {
	Game,
	Title
};

enum class MenuName {
	Confirm_Quit,
	GO,
	Main,
	Options,
	Pause,
};

enum class UIElem {
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
	Music_V,
	SFX_V
};