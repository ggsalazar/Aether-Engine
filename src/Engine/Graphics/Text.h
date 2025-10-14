#pragma once
#include <string>
#include <unordered_map>
#include "../Math/Geometry.h" //Vec2 (iostream)
#include "Font.h"

using std::string, std::unordered_map;

class Text {
    friend class Font;
    friend class Renderer;
public:
    struct Info {
        string str;
        Vec2i pos;
        uint font_size = 36;
        Vec2i str_size;
        uint max_width = 640;
        Color color{ 1 };
        Vec2f origin{};
        float rot = 0.f;
        Info() = default;
    };
    static inline uchar res_scale = 1;
    static inline unordered_map<int, Font> fonts;
    Font font;

    Text() { Init(info); }
    Text(const Info& i) { Init(i); }
    Text(const uint i_size) {
        info.font_size = i_size;
        Init(info);
    }
    ~Text() = default;
    void Init(const Info& i) {
        info = i;
        SetFont();
    }

    static inline void InitFonts() {
        for (int i = 12; i <= 300; i += 2)
            fonts.insert({ i, Font("m5x7", i) });
    }

    static inline void SetResScale(const uchar new_scale) { res_scale = new_scale; }

    [[nodiscard]] inline Info GetInfo() const { return info; }

    inline void SetFont(const uint new_font_size = 0) {
        info.font_size = new_font_size == 0 ? info.font_size : new_font_size;

        font = fonts[info.font_size * res_scale];
    }
    [[nodiscard]] inline uint GetFontSize() const { return info.font_size; }

    inline void SetStr(const string s) { info.str = s; }
    inline void ConcatStr(const string s) { info.str += s; }
    [[nodiscard]] inline string GetStr() const { return info.str; }

    inline void MoveTo(const Vec2i new_pos) { info.pos = new_pos; }
    inline void MoveBy(const Vec2i offset) { info.pos += offset; }
    [[nodiscard]] inline Vec2i GetPos() const { return info.pos; }

    inline Vec2i GetStrSize(const bool physical = false) {
        TTF_GetStringSizeWrapped(font.GetFont(), info.str.c_str(), strlen(info.str.c_str()), GetMaxW(true), &info.str_size.x, &info.str_size.y);
        if (physical) return info.str_size;
        return info.str_size / res_scale;
    }

    inline void SetMaxW(const uint new_max) { info.max_width = new_max; }
    [[nodiscard]] inline uint GetMaxW(const bool physical = false) const {
        return physical ? info.max_width * res_scale : info.max_width;
    }

    inline void SetColor(const Color& c) { info.color = c; }
    [[nodiscard]] inline Color GetColor() const { return info.color; }

    inline void SetOrigin(Vec2f ori = { .5f, .5f }) {
        if (ori.x < 0.f or 1.f < ori.x) ori.x = 0.f;
        if (ori.y < 0.f or 1.f < ori.y) ori.y = 0.f;
        info.origin = ori;
    }
    [[nodiscard]] inline Vec2f GetOrigin() const { return info.origin; }

    //Rotation in degrees
    inline void SetRotD(const float angle) { info.rot = angle; }
    [[nodiscard]] inline float GetRotD() const { return info.rot; }
    //Rotation in radians
    inline void SetRotR(const float rad) { info.rot = rad * 57.2958; }
    [[nodiscard]] inline float GetRotR() const { return info.rot / 57.2958; }

private:
    Info info = {};
};