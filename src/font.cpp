#include "font.h"
#include "log.h"

namespace SpaceEngine
{
    void FontLoader::LoadFont(const std::string& nameFont)
    {
        FT_Library ft;

        FT_CHECK(FT_Init_FreeType(&ft));
        
    }
}