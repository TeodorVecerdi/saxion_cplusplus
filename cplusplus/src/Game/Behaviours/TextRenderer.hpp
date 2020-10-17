#pragma once
#include "Core/ScriptableBehaviour.hpp"

enum class TextAlignment {
    LOWER_LEFT,
    LOWER_CENTER,
    LOWER_RIGHT,
    MIDDLE_LEFT,
    MIDDLE_CENTER,
    MIDDLE_RIGHT,
    UPPER_LEFT,
    UPPER_CENTER,
    UPPER_RIGHT
};

class TextRenderer final : public ScriptableBehaviour {
public:
    TextRenderer(const std::string& fontName);
    TextRenderer(const std::string& identifier, const std::string& fontName);

    void setText(const std::string& textString);
    void setFillColor(sf::Color color);
    void setOutlineColor(sf::Color color);
    void setAlignment(TextAlignment alignment);
    void setFontSize(uint32_t fontSize);
    void setFont(const std::string& fontName);
    std::string getText() const;
    sf::Color getFillColor() const;
    sf::Color getOutlineColor() const;
    TextAlignment getAlignment() const;
    uint32_t getFontSize() const;
protected:
    void onRender(sf::RenderWindow& window) const override;
    void onUpdate(sf::Time ts) override;
private:
    sf::Text text;
    TextAlignment alignment;

    static glm::vec2 AlignmentToOrigin(const TextAlignment alignment) {
        switch (alignment) {
        case TextAlignment::UPPER_LEFT: return glm::vec2(0, 0);
        case TextAlignment::UPPER_CENTER: return glm::vec2(.5, 0);
        case TextAlignment::UPPER_RIGHT: return glm::vec2(1, 0);
        case TextAlignment::MIDDLE_LEFT: return glm::vec2(0, .5);
        case TextAlignment::MIDDLE_CENTER: return glm::vec2(.5, .5);
        case TextAlignment::MIDDLE_RIGHT: return glm::vec2(1, .5);
        case TextAlignment::LOWER_LEFT: return glm::vec2(0, 1);
        case TextAlignment::LOWER_CENTER: return glm::vec2(.5, 1);
        case TextAlignment::LOWER_RIGHT: return glm::vec2(1, 1);
        default: return glm::vec2(0, 0);
        }
    }
};
