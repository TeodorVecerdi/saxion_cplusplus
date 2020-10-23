#include "Fonts.hpp"

#include "Utils/Debug.hpp"

sf::Font& Fonts::GetFont(const std::string& fontName) {
    ASSERT(fonts.contains(fontName), "Font [" << fontName.c_str() << "] does not exist");
    return *fonts[fontName];
}

void Fonts::RegisterFont(const std::string& fontName, const std::string& fontPath) {
    ASSERT(!fonts.contains(fontName), "Cannot register font with name [" << fontName.c_str() << "] when another font with the same name already exists");
    sf::Font temp;
    temp.loadFromFile(fontPath);
    fonts.emplace(std::make_pair<const std::string&, sf::Font*>(fontName, new sf::Font(temp)));
}

void Fonts::RegisterDefaultFonts() {
    RegisterFont("Arial", "assets/fonts/arial.ttf");
    RegisterFont("Calibri", "assets/fonts/calibri.ttf");
    RegisterFont("Lato", "assets/fonts/Lato-Regular.ttf");
}

Fonts::~Fonts() {
    for (auto [_, font] : fonts) {
        delete font;
    }
}

Fonts::Fonts() {
    RegisterDefaultFonts();
}