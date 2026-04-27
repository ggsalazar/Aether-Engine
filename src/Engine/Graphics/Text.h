#pragma once
#include <string>
#include <unordered_map>
#include "Font.h"
#include "Renderer.h"
#include "../Enums.h"
#include "../Math/Geometry.h" //Vec2 (iostream)

using std::string;

class Text {
    friend class Font;
    friend class Renderer;
public:
    struct Info {
    	FontName font_name = FontName::Fabled;
    	LayerName default_layer = LayerName::UI;
    	int layer_order = 0;
        string str;
        Vec2i pos;
        uint font_size = 0;
        uchar line_height_offset = 0;
        Vec2i str_size;
        uint max_width = 640;
        Color color{ 1 };
        Vec2f origin{}; //Origin and alignment are, for now, one and the same
        Info() = default;
    };
    static inline std::unordered_map<int, Font> normal_fonts, fabled_fonts;
    Font font;

    Text() { Init(info); }
    Text(const Info& i) { Init(i); }
    Text(const FontName i_name, const uint i_size) {
		info.font_name = i_name;
    	info.font_size = i_size;
        Init(info);
    }
    ~Text() = default;
    void Init(const Info& i) {
        info = i;
        SetFont();
    }

	static inline void SetRenderer(Renderer* r) { renderer = r; }

    static inline void InitFonts()  {
        for (int i = 2; i <= 100; i += 2) {
	        normal_fonts.insert({ i, Font("Normal", i) });
        	if (i%10 == 0) fabled_fonts.insert({i, Font("Fabled_Font", i)});
        }
    }

	void Draw(const Rect clip_rect = Rect({0}, 0), LayerName l = LayerName::NONE) {
    	l = l == LayerName::NONE ? info.default_layer : l;
	    renderer->SubmitText(l, *this, clip_rect);
    }

    [[nodiscard]] inline Info GetInfo() const { return info; }

	void SetDefaultLayer(const LayerName layer) { info.default_layer = layer; }
	[[nodiscard]] inline LayerName GetDefaultLayer() const { return info.default_layer; }

	inline void SetLayerOrder(const int new_lo = 0) { info.layer_order = new_lo; }
	[[nodiscard]] inline int GetLayerOrder() const { return info.layer_order; }

    inline void SetFont(const float new_font_size = 0, const FontName new_font_name = FontName::NONE) {
        info.font_size = new_font_size == 0 ? info.font_size : new_font_size;
    	info.font_name = new_font_name == FontName::NONE ? info.font_name : new_font_name;

    	switch (info.font_name) {
    		case FontName::Fabled:
    			//Fabled font only works in multiples of 10
				if (info.font_size % 10 != 0) {
					info.font_size -= info.font_size % 10;
					info.font_size = info.font_size == 0 ? 10 : info.font_size;
				}
    			font = fabled_fonts[info.font_size];
    		break;

    		case FontName::Normal:
    			//Honestly not sure what multiples Normal works with
    			info.font_size -= info.font_size % 2;
    			font = normal_fonts[info.font_size];
    		break;

    		case FontName::NONE:
    			std::cerr << "Forgot to set a font name! Defaulting to Fabled\n";
    			SetFont(0, FontName::Fabled);
    		break;
    	}
    }
    [[nodiscard]] inline uint GetFontSize() const { return info.font_size; }

    //Set the line height offset
    inline void SetLHO(const uchar new_lho) { info.line_height_offset = new_lho; }
    [[nodiscard]] inline uchar GetLHO() const { return info.line_height_offset; }

    inline void SetStr(const string& s) { info.str = s; }
    inline void ConcatStr(const string& s) { info.str += s; }
    [[nodiscard]] inline string GetStr() const { return info.str; }

	template<typename T=float>
    inline void MoveTo(const Vec2<T>& new_pos) { info.pos = Round(new_pos); }
	template<typename T=float>
    inline void MoveBy(const Vec2<T>& offset) { info.pos += Round(offset); }
    [[nodiscard]] inline Vec2f GetPos() const { return info.pos; }

    inline Vec2i GetStrSize() {
        //Have to manually calculate the width & height of the string
        std::vector<string> lines;
		std::istringstream full_stream(info.str);
		string full_text;
		string curr_line;
		string test;
		string word;
		int line_w = 0;
    	info.str_size.x = 0;

		//Wrap the text
		while (std::getline(full_stream, full_text, '\n')) {
			std::istringstream line_stream(full_text);
			word = "";
			curr_line = "";
			while (line_stream >> word) {
				test = curr_line.empty() ? word : curr_line + " " + word;
				TTF_GetStringSize(font.GetFont(), test.c_str(), test.length(), &line_w, nullptr);
				if (line_w > GetMaxW() and !curr_line.empty()) {
					lines.push_back(curr_line);
					curr_line = word;
				}
				else {
					curr_line = test;
					info.str_size.x = line_w > info.str_size.x ? line_w : info.str_size.x;
				}
			}
			if (!curr_line.empty()) lines.push_back(curr_line);
		}

    	info.str_size.y = (TTF_GetFontLineSkip(font.GetFont()) - info.line_height_offset) * lines.size();

        return info.str_size;
    }

    inline void SetMaxW(const uint new_max) { info.max_width = new_max; }
    [[nodiscard]] inline uint GetMaxW() const { return info.max_width; }

    inline void SetColor(const Color& c) { info.color = c; }
    [[nodiscard]] inline Color GetColor() const { return info.color; }

    inline void SetOrigin(Vec2f ori = { .5f, .5f }) {
        if (ori.x < 0.f or 1.f < ori.x) ori.x = 0.f;
        if (ori.y < 0.f or 1.f < ori.y) ori.y = 0.f;
        info.origin = ori;
    }
    [[nodiscard]] inline Vec2f GetOrigin() const { return info.origin; }

private:
    Info info = {};
    static inline Renderer* renderer;
};