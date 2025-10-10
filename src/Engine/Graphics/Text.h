#pragma once
#include <string>
#include "../Engine.h" //Camera, Font, Sprite (Geometry)

using std::string;

class Text {
    friend class Font;
    friend class Renderer;
public:
    struct Info {
        string str = "";
        Vec2i pos;
        uint font_size = 36;
        Vec2i str_size;
        uint max_width = 640;
        Color color{ 1 };
        Vec2f origin{};
        float rot = 0.f;
    };
    static inline uchar res_scale = 1;
    Font* font = nullptr;

    Text(const Info& i = {}) {
        Init(i);
    }
    Text(const uint i_size) {
        info.font_size = i_size;
        Init(info);
    }
    ~Text() {}
    void Init(const Info& i = {}) {
        info = i;

        SetFont();
    }

    static inline void SetResScale(uchar new_scale) { res_scale = new_scale; }
    static inline void SetEngine(Engine* e) { engine = e; }
    static inline void SetCam(Camera* c) { cam = c; }

    //Should just be used for rendering
    inline Info GetInfo() const { return info; }

    inline void SetFont(uint new_font_size = -1) {
        info.font_size = new_font_size == -1 ? info.font_size : new_font_size;

        font = &engine->fonts[info.font_size * res_scale];
    }
    inline uint GetFontSize() const { return info.font_size; }

    inline void SetStr(const string s) { info.str = s; }
    inline void ConcatStr(const string s) { info.str += s; }
    inline string GetStr() const { return info.str; }

    inline void MoveTo(const Vec2i new_pos) { info.pos = new_pos; }
    inline void MoveBy(const Vec2i offset) { info.pos += offset; }
    inline Vec2i GetPos() const { return info.pos; }

    inline Vec2i GetStrSize(bool physical = false) {
        TTF_GetStringSizeWrapped(font->GetFont(), info.str.c_str(), strlen(info.str.c_str()), GetMaxW(true), &info.str_size.x, &info.str_size.y);
        if (physical) return info.str_size;
        else return info.str_size / res_scale;
    }

    inline void SetMaxW(const uint new_max) { info.max_width = new_max; }
    inline uint GetMaxW(const bool physical = false) const {
        return physical ? info.max_width * res_scale : info.max_width;
    }

    inline void SetColor(const Color& c) { info.color = c; }
    inline Color GetColor() const { return info.color; }

    inline void SetOrigin(Vec2f ori = { .5f, .5f }) {
        if (ori.x < 0.f or 1.f < ori.x) ori.x = 0.f;
        if (ori.y < 0.f or 1.f < ori.y) ori.y = 0.f;
        info.origin = ori;
    }
    inline Vec2f GetOrigin() const { return info.origin; }

    //Rotation in degrees
    inline void SetRotD(const float angle) { info.rot = angle; }
    inline float GetRotD() const { return info.rot; }
    //Rotation in radians
    inline void SetRotR(const float rad) { info.rot = rad * 57.2958; }
    inline float GetRotR() const { return info.rot / 57.2958; }

private:
    Info info = {};
    inline static Engine* engine = nullptr;
    inline static Camera* cam = nullptr;
};