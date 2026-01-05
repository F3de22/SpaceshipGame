#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include "log.h"
#include <string>

#define FT_CKECK(call)                                                  \
    do                                                                  \
    {                                                                   \
        if(int error = call; error)                                     \
            SPACE_ENGINE_FATAL("FreeType error: {} for " #call, error); \
    }while(0)                               

namespace SpaceEngine
{
    class FontLoader
    {
        public:

            static void LoadFont(const std::string& nameFont); 

    };
}
