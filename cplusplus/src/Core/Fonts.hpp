#pragma once
#include <string>
#include <map>
#include <SFML/Graphics/Font.hpp>

class Fonts {
public:
    sf::Font& GetFont(const std::string& fontName);
    void RegisterFont(const std::string& fontName, const std::string& fontPath);
    static Fonts* getInstance() {
        if(!instance) {
            instance = new Fonts();
        }
        return instance;
    }
private:
    ~Fonts();
    Fonts();
    void RegisterDefaultFonts();
    std::map<std::string, sf::Font*> fonts;
    inline static Fonts* instance = nullptr;
};
