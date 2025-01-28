#pragma once
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


using uint = unsigned int;

//Resolution
constexpr uint MINRESW = 320;
constexpr uint MINRESH = 180;
inline uint SCREENW() { return sf::VideoMode::getDesktopMode().size.x; }
inline uint SCREENH() { return sf::VideoMode::getDesktopMode().size.y; }

//Mouse input
#define MOUSEPOS sf::Mouse::getPosition(window)
constexpr sf::Mouse::Button LMB = sf::Mouse::Button::Left;
constexpr sf::Mouse::Button RMB = sf::Mouse::Button::Right;
inline bool LMBDOWN() { return sf::Mouse::isButtonPressed(LMB); }
inline bool RMBDOWN() { return sf::Mouse::isButtonPressed(RMB); }

//Letters
constexpr sf::Keyboard::Key A_K = sf::Keyboard::Key::A;
constexpr sf::Keyboard::Key D_K = sf::Keyboard::Key::D;
inline bool ADOWN() { return sf::Keyboard::isKeyPressed(A_K); }
inline bool DDOWN() { return sf::Keyboard::isKeyPressed(D_K); }

//Special keys
//Other
constexpr sf::Keyboard::Key TAB = sf::Keyboard::Key::Tab;
inline bool TABDOWN() { return sf::Keyboard::isKeyPressed(TAB); }
//Arrows
inline bool LADOWN() { return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left); }
inline bool RADOWN() { return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right); }