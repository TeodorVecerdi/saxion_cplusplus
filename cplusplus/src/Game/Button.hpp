#pragma once
#include "Core.hpp"
#include "Core/GameObject.hpp"

class Button final : public GameObject {
public:
	Button(const std::string& text, const std::string& fontName, const std::string& buttonTexturePath, void (*onClick)());
	Button(const std::string& identifier, const std::string& text, const std::string& fontName, const std::string& buttonTexturePath, void (*onClick)());
    void setSize(glm::vec2 size);
    glm::vec2 getSize() const;

    sf::Color normalColor = sf::Color(255, 255, 255);
    sf::Color hoverColor = sf::Color(227, 227, 227);
    sf::Color activeColor = sf::Color(200, 200, 200);
protected:
    void onStart() override {}
    void onRender(sf::RenderWindow& window) const override;
    void onUpdate(sf::Time ts) override;
private:
    sf::Text label;
    sf::Sprite sprite;
    sf::Texture texture;
    glm::vec2 size;
    
    void(*onClick)();
};
