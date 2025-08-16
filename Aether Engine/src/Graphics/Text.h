#pragma once
#include <string>
#include "../Core/Camera.h"
#include "../Core/Geometry.h"
#include "Font.h"

using std::string;

class Text {
    friend class Font;
    friend class Renderer;
public:
    struct Info {
        string str = "";
        Vec2i pos;
        Vec2i str_size;
        uint max_width = 640;
        Color color{ 1 };
        Vec2f origin{};
        float rot = 0.f;
    };
    static inline uchar res_scale = 1;
    Font* font = nullptr;

    Text() = default;
    Text(Font* f, const Info& i = {}) : font(f), info(i) {
        MoveTo(info.pos);
        SetLogicalMaxW(info.max_width);
    }
    ~Text() {}
    void Init(Font* f, const Info& i = {}) {
        font = f;
        info = i;

        MoveTo(info.pos);
        SetLogicalMaxW(info.max_width);
    }

    static inline void SetResScale(uchar new_scale) { res_scale = new_scale; }
    static inline void SetCam(Camera* c) { cam = c; }

    //Should just be used for rendering
    inline Info GetInfo() const { return info; }

    inline void SetStr(const string s) { info.str = s; }
    inline void ConcatStr(const string s) { info.str += s; }
    inline string GetStr() const { return info.str; }

    inline void MoveTo(const Vec2i new_pos) { info.pos = Vec2i{ new_pos.x - cam->viewport.x, new_pos.y - cam->viewport.y } *res_scale; }
    inline void MoveBy(const Vec2i offset) { info.pos += Vec2i{ offset.x - cam->viewport.x, offset.y - cam->viewport.y } *res_scale; }
    inline Vec2i GetPos() const { return info.pos / res_scale; }

    inline Vec2i GetLogicalStrSize() const { return info.str_size / res_scale; }
    inline Vec2i GetPhysicalStrSize() const { return info.str_size; }

    inline void SetLogicalMaxW(const uint new_max) { info.max_width = new_max * res_scale; }
    inline uint GetLogicalMaxW() const { return info.max_width / res_scale; }
    inline void SetPhysicalMaxW(const uint new_phys_max) { info.max_width = new_phys_max; }
    inline uint GetPhysicalMaxW() const { return info.max_width; }

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
    static inline Camera* cam = nullptr;
};