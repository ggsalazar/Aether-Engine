#pragma once
#include <iostream>
#include "../Core/Vector2.h"

namespace Text {
    void Init(Text& text, Font& font, const uint char_size = 36, const Vector2u pos = { 0, 0 }, const std::string str = "DEFAULT", const Vector2f ori = { .5f, .5f }, const float max_width = 1920);
    void SetStr(Text& text, std::string str, const float max_width = 1920.f);
    void SetCharSize(Text& text, uint char_size);
    void SetOrigin(Text& text, Vector2f ori = { .5f, .5f });
    Vector2f GetOrigin(Text& text);
}