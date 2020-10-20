#pragma once
#include "Core/ScriptableBehaviour.hpp"


class SpriteRenderer;


class TextRenderer final : public ScriptableBehaviour {
public:
    TextRenderer(const std::string& fontName);
    TextRenderer(const std::string& identifier, const std::string& fontName);

    void setText(const std::string& textString);
    void setFillColor(sf::Color color);
    void setOutlineColor(sf::Color color);
    void setFontSize(uint32_t fontSize);
    void setFont(const std::string& fontName);
    void setResizeReference(SpriteRenderer* resizeReference);
    std::string getText() const;
    sf::Color getFillColor() const;
    sf::Color getOutlineColor() const;
    uint32_t getFontSize() const;
protected:
    void onRender(sf::RenderWindow& window) const override;
    void onUpdate(sf::Time ts) override;
private:
    sf::Text text;
    SpriteRenderer* resizeReference;

};
