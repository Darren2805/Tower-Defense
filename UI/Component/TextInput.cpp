#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include <allegro5/allegro_font.h>
#include <memory>
#include <stdio.h>
#include <iostream>

#include "Engine/Collider.hpp"
#include "Engine/GameEngine.hpp"
#include "TextInput.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "Engine/IObject.hpp"

using std::cout;
using std::endl;

namespace Engine
{
    TextInput::TextInput(const std::string &font, int fontSize, float _x, float _y, float _w, float _h, unsigned char r, unsigned char g, unsigned char b, unsigned char a, float anchorX, float anchorY) : IObject(_x, _y, _w, _h, anchorX, anchorY), font(Resources::GetInstance().GetFont(font, fontSize)), Text(""), Color(al_map_rgba(r, g, b, a))
    {
    }

    void TextInput::Draw() const
    {
        al_draw_filled_rectangle(Position.x - Anchor.x * Size.x, Position.y - Anchor.y * Size.y, Position.x + Size.x - Anchor.x * Size.x, Position.y + Size.y - Anchor.y * Size.y, al_map_rgba(255 - !isFocus * 55, 255 - !isFocus * 55, 255 - !isFocus * 55, 255));
        if (isFocus)
            al_draw_filled_rectangle(Position.x - Anchor.x * Size.x + GetTextWidth() + 5, Position.y - Anchor.y * Size.y + 5, Position.x + GetTextWidth() + 20 - Anchor.x * Size.x - 5, Position.y + Size.y - Anchor.y * Size.y - 5, al_map_rgba(30, 30, 30, 255));
        al_draw_text(font.get(), Color, Position.x - Anchor.x * GetTextWidth(), Position.y - Anchor.y * GetTextHeight(), 0, Text.c_str());
    }

    void TextInput::OnMouseMove(int mx, int my)
    {
        mouseIn = mx >= Position.x && mx <= Position.x + Size.x && my >= Position.y && my <= Position.y + Size.y;
    }

    void TextInput::OnMouseDown(int button, int mx, int my)
    {
        isFocus = mouseIn;
    }

    void TextInput::OnKeyDown(int keycode)
    {
        if (keycode >= 1 && keycode <= 26)
        {
            // If keycode A --> Z
            keycode += 64;
            Text += (char)keycode;
            // cout << "Typed: " << Text << endl;
        }
        else if (keycode == 63)
        {
            // Backspace
            if (Text.length() > 0)
                Text = Text.substr(0, Text.length() - 1);
            // cout << "Typed: " << Text << endl;
        }
    }

    int TextInput::GetTextWidth() const
    {
        return al_get_text_width(font.get(), Text.c_str());
    }

    int TextInput::GetTextHeight() const
    {
        return al_get_font_line_height(font.get());
    }
}