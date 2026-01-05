#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include <string>

                             

namespace SpaceEngine
{
    class FontLoader
    {
        public:
            static void LoadFont(const std::string& nameFont); 
    };
}
