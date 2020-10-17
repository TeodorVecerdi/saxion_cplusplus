#pragma once
#include <string>
#include <map>
#include <SFML/Graphics/Font.hpp>
#include "Utils/Debug.hpp"

class FontCache {
public:
    static sf::Font& GetFont(const std::string& fontName) {
        ASSERT(fonts.contains(fontName), "Font [" << fontName.c_str() << "] does not exist");
        return *fonts[fontName];
    }
    
    static void RegisterFont(const std::string& fontName, sf::Font& font) {
        ASSERT(!fonts.contains(fontName), "Cannot register font with name [" << fontName.c_str() << "] when another font with the same name already exists");
        fonts.emplace(std::make_pair(fontName, &font));
    }

private:
    inline static std::map<std::string, sf::Font*> fonts;
};
