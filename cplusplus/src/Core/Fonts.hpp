#pragma once
#include <string>
#include <map>
#include <SFML/Graphics/Font.hpp>

class Fonts {
public:
    sf::Font& GetFont(const std::string& fontName);
    void RegisterFont(const std::string& fontName, const std::string& fontPath);
    static Fonts* getInstance() {
        static Fonts instance;
        return &instance;
    }
private:
    ~Fonts();
    Fonts();
    Fonts(const Fonts&) = delete;
    Fonts& operator=(const Fonts&) = delete;
    
    void RegisterDefaultFonts();
    std::map<std::string, sf::Font*> fonts;
    static Fonts* instance;
};
