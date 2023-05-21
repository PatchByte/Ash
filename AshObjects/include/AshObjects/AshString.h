#pragma once
#include <Ash/AshStreamableObject.h>
#include <iostream>

namespace ash::objects
{
    using AshStringLength = unsigned short;

    // Secure String
    class AshString : public AshStreamableObject
    {
    public:
        AshString();
        AshString(std::string Text);
        AshString(std::string_view Text);
        AshString(const char* Text);
        
        inline void operator=(std::string Text)
        {
            text = Text;
        }

        inline void operator=(std::string_view Text)
        {
            text = Text;
        }

        inline void operator=(const char* Text)
        {
            text = Text;
        }

        inline AshStringLength GetTextLimitLength()
        {
            return textLimitLength;
        }

        inline std::string GetText()
        {
            return text;
        }

        ASH_STREAMABLE_OBJECT_IMPLEMENT_SIMPLE("AshString");
    private:
        std::string text;
        AshStringLength textLimitLength;
    };

}