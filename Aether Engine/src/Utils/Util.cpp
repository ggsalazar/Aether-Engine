#include "Util.h"

void Util::Clamp(float &value, const float min, const float max) {
    if (value < min) value = min;
    else if (value > max) value = max;
}

float Util::Lerp(const float a, const float b, const float t, const bool angle) {
    if (angle) {
        //Get the difference between the angles
        float diff = b - a;

        //Adjust the difference to ensure the intest path is taken
        if (diff > 180) diff -= 360;
        else if (diff < -180) diff += 360;

        return a + diff * t;
    }
    else
        return a + t * (b - a);
}

void Util::InitText(sf::Text& text, sf::Font& font, const uint char_size, const float pos_x, const float pos_y, std::string str, float ori_x, float ori_y) {
    text.setFont(font);
    text.setCharacterSize(char_size);
    text.setString(str);
    SetTextOrigin(text, ori_x, ori_y);
    text.setPosition({ pos_x, pos_y });
}

void Util::SetTextOrigin(sf::Text &text, float ori_x, float ori_y) {
    //Text object origin is based on CURRENT string of the text, ergo must be updated constantly
    ori_x = ori_x <= 1.f ? ori_x : 0.f;
    ori_y = ori_y <= 1.f ? ori_y : 0.f;


    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin({ bounds.position.x + bounds.size.x * ori_x, bounds.position.y + bounds.size.y * ori_y });
}
