#pragma once
#include "Core.hpp"
#include "Core/GameObject.hpp"

class Label final: public GameObject {
public:
    Label(const std::string& text, const std::string& fontName);
    Label(const std::string& identifier, const std::string& text, const std::string& fontName);
    void setText(const std::string& text);
    void setFontSize(uint32_t fontSize);
    void setFont(const std::string& fontName);
    std::string getText() const;
    uint32_t getFontSize() const;
    std::string getFont() const;
protected:
    void onRender(sf::RenderWindow& window) const override;
    void onStart() override {}
    void onUpdate(sf::Time ts) override;

private:
    sf::Text text;
    std::string font;
};
