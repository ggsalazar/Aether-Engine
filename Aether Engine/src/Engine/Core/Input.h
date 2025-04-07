#pragma once
#include <iostream>

//Mouse input
#define MOUSEPOS Mouse::getPosition(window)
#define MOUSEPOS_E Mouse::getPosition(engine.window)
#define MOUSEPOS_W Vector2i(window.mapPixelToCoords(Mouse::getPosition(window)))
#define MOUSEPOS_W_E Vector2i(engine.window.mapPixelToCoords(Mouse::getPosition(engine.window)))
constexpr Mouse::Button LMB = Mouse::Button::Left;
constexpr Mouse::Button RMB = Mouse::Button::Right;
constexpr Mouse::Button MID = Mouse::Button::Middle;
inline bool BUTTONDOWN(Mouse::Button button) { return Mouse::isButtonPressed(button); }

//Letters
constexpr Keyboard::Key A_K = Keyboard::Key::A;
constexpr Keyboard::Key C_K = Keyboard::Key::C;
constexpr Keyboard::Key D_K = Keyboard::Key::D;
constexpr Keyboard::Key I_K = Keyboard::Key::I;
constexpr Keyboard::Key J_K = Keyboard::Key::J;
constexpr Keyboard::Key M_K = Keyboard::Key::M;
constexpr Keyboard::Key O_K = Keyboard::Key::O;
constexpr Keyboard::Key S_K = Keyboard::Key::S;
constexpr Keyboard::Key W_K = Keyboard::Key::W;
//Special keys
//Other
constexpr Keyboard::Key LCTRL = Keyboard::Key::LControl; constexpr Keyboard::Key RCTRL = Keyboard::Key::RControl;
constexpr Keyboard::Key LSHIFT = Keyboard::Key::LShift; constexpr Keyboard::Key RSHIFT = Keyboard::Key::RShift;
constexpr Keyboard::Key TAB = Keyboard::Key::Tab;
//Arrows
constexpr Keyboard::Key UP = Keyboard::Key::Up;
constexpr Keyboard::Key DOWN = Keyboard::Key::Down;
constexpr Keyboard::Key LEFT = Keyboard::Key::Left;
constexpr Keyboard::Key RIGHT = Keyboard::Key::Right;
inline bool BUTTONDOWN(Keyboard::Key button) { return Keyboard::isKeyPressed(button); }


namespace Input {
	//Variables for when keys are up vs down
	//Mouse
	static bool lmb_was_down = false;
	static bool lmb_was_up = true;
	static bool rmb_was_down = false;
	static bool rmb_was_up = true;
	static bool mid_was_down = false;
	static bool mid_was_up = true;
	//Letters
	//C
	static bool c_was_down = false;
	static bool c_was_up = true;
	//I
	static bool i_was_down = false;
	static bool i_was_up = true;
	//J
	static bool j_was_down = false;
	static bool j_was_up = true;
	//M
	static bool m_was_down = false;
	static bool m_was_up = true;
	//O
	static bool o_was_down = false;
	static bool o_was_up = true;
	//Special keys
	static bool tab_was_down = false;
	static bool tab_was_up = true;


	void UpdateVars();

	//Key pressed this frame
	bool KeyPressed(Keyboard::Key key);
	bool KeyPressed(Mouse::Button mb);

	//Key released this frame
	bool KeyReleased(Keyboard::Key key);
	bool KeyReleased(Mouse::Button mb);
}