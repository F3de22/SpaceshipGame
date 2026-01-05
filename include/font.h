#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include "log.h"
#include <string>

                             

namespace SpaceEngine
{
    class FontLoader
    {
        public:
            static void LoadFont(const std::string& nameFont); 
    };
}
