#pragma once

#include <juce_graphics/juce_graphics.h>

namespace UI
{
    namespace Utils
    {
        static const juce::Font getCustomFont()
        {
            static auto typeface = juce::Typeface::Ptr();
            try
            {
                typeface = juce::Typeface::createSystemTypefaceFor (BinaryData::Rovaney_ttf, BinaryData::Rovaney_ttfSize);
                if (typeface != nullptr)
                {
                    juce::Font font (typeface);
                    font.setHeight (20.0f);

                    return font;
                }
            } catch (...)
            {
                // If loading the custom font fails, fall back to a default font
            }
            return juce::Font ("Default", 20.0f, juce::Font::plain);
        }

        // class CustomFontLookAndFeel : public juce::LookAndFeel_V4
        // {
        // public:
        //     CustomFontLookAndFeel()
        //     {
        //         customFont = UI::Utils::getCustomFont();
        //     }

        //     ~CustomFontLookAndFeel() = default;

        // private:
        //     juce::Font customFont;

        //     JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomFontLookAndFeel)
        // } customFontLookAndFeel;
    }
}
