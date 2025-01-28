#pragma once
#include <iostream>
#include "Macros.h"

namespace Util {

    //Math/Numbers
    void Clamp(float &value, const float min, const float max);
    float Lerp(const float a, const float b, const float t, const bool angle = false);
    //sf::Vector2f Lerp(const sf::Vector2f a, const sf::Vector2f b, const float t);

    //Text
    void InitText(sf::Text& text, sf::Font& font, const uint char_size, const float pos_x, const float pos_y, const std::string str, float ori_x = .5f, float ori_y = .5f);
    void SetTextOrigin(sf::Text &text, float ori_x = .5f, float ori_y = .5f);
};