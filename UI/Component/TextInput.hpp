#ifndef TEXTINPUT_HPP
#define TEXTINPUT_HPP
#include <allegro5/allegro_font.h>
#include <allegro5/color.h>
#include <memory>
#include <string>

#include "Engine/IControl.hpp"
#include "Engine/IObject.hpp"

namespace Engine
{
    class TextInput : public IObject, public IControl
    {
    protected:
        std::shared_ptr<ALLEGRO_FONT> font;

        bool isFocus = false;
        bool mouseIn = false;

    public:
        std::string Text;

        ALLEGRO_COLOR Color;

        explicit TextInput(const std::string &font, int fontSize, float x, float y, float w, float h, unsigned char r = 0, unsigned char g = 0, unsigned char b = 0, unsigned char a = 255, float anchorX = 0, float anchorY = 0);

        void OnMouseMove(int mx, int my) override;

        void OnMouseDown(int button, int mx, int my) override;

        void OnKeyDown(int keycode) override;

        void Draw() const override;

        int GetTextWidth() const;

        int GetTextHeight() const;
    };
}

#endif