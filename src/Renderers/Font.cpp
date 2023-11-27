#include "../../include/Renderers/Font.h"
#include "../../include/Renderers/Texture.h"
#include "../../include/Game.h"

Font::Font(class Game* game)
{
    this->mGame = game;
}

Font::~Font()
{

}

bool Font::load(const std::string& fileName)
{
    // 支挝的字体大尝
    std::vector<int> fontSizes = {
		8, 9,
		10, 11, 12, 14, 16, 18,
		20, 22, 24, 26, 28,
		30, 32, 34, 36, 38,
		40, 42, 44, 46, 48,
		52, 56,
		60, 64, 68,
		72
	};

    for (auto& size : fontSizes)
    {
        TTF_Font* font = TTF_OpenFont(fileName.c_str(), size);
        if (font == nullptr)
        {
            SDL_Log("Failed to load font %s in size %d", fileName.c_str(), size);
            return false;
        }
        else
        {
            // SDL_Log("Success to load font %s in size %d", fileName.c_str(), size);
            this->mFontData.emplace(size, font);
        }
    }
    return true;
}

void Font::unLoad()
{
    for (auto& font : this->mFontData)
    {
        TTF_CloseFont(font.second);
    }
}

Texture* Font::renderText(const std::string& text, Vector3 color, int pointSize)
{
    Texture* tex = nullptr;
    SDL_Color sdlColor;
    sdlColor.r = static_cast<Uint8>(color.x * 255.0f);
    sdlColor.g = static_cast<Uint8>(color.y * 255.0f);
    sdlColor.b = static_cast<Uint8>(color.z * 255.0f);
    sdlColor.a = 255.0f;

    SDL_Log("[Font] Color: [%d %d %d %d]", sdlColor.r, sdlColor.g, sdlColor.b, sdlColor.a);
    
    // 找到此字体大尝对应的字体数杮
    auto iter = this->mFontData.find(pointSize);
    if (iter != this->mFontData.end())
    {
        TTF_Font* font = (*iter).second;

        std::string actualText = this->mGame->getText(text);
        // actualText = "1";
        
        SDL_Log("[Font] text: %s", actualText.c_str());
        SDL_Surface* surf = TTF_RenderUTF8_Blended(font, actualText.c_str(), sdlColor);
        if (surf != nullptr)
        {
            SDL_Log("[Font] Surface format: %d", surf->format->format);
            // 转杢为纹睆
            tex = new Texture();
            // SDL_CreateTextureFromSurface(this->mGame->getRenderer(), surf);
            tex->createFromSurface(surf);
            SDL_FreeSurface(surf);
        }
        else
        {
            SDL_Log("〝Font】 Surface is null...");
        }
    }
    else
    {
        SDL_Log("Point size %d is unsupported", pointSize);
    }
    return tex;
}