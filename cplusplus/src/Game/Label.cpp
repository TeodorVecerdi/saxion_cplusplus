#include "Label.hpp"

#include "Core/FontCache.hpp"

Label::Label(const std::string& identifier, const std::string& text, const std::string& fontName) : GameObject(identifier) {
    setText(text);
    setFont(fontName);
}

Label::Label(const std::string& text, const std::string& fontName) : Label(string_format("Label_%x", reinterpret_cast<void*>(this)), text, fontName) {}


void Label::setText(const std::string& text) {
    this->text.setString(text);
}

void Label::setFontSize(uint32_t fontSize) {
    text.setCharacterSize(fontSize);
}

void Label::setFont(const std::string& fontName) {
    font = fontName;
    text.setFont(FontCache::GetFont(fontName));
}

std::string Label::getText() const {
    return text.getString();
}

uint32_t Label::getFontSize() const {
    return text.getCharacterSize();
}

std::string Label::getFont() const {
    return font;
}

void Label::onRender(sf::RenderWindow& window) const {
    window.draw(text);
}

void Label::onUpdate(sf::Time ts) {
    const auto renderTransform = this->getRenderTransform();
    text.setPosition(renderTransform.position.x, renderTransform.position.y);
    text.setScale(renderTransform.scale.x, renderTransform.scale.y);
    text.setRotation(renderTransform.rotation.z);

    const auto origin = renderTransform.origin;
    const auto textRect = text.getGlobalBounds();
    text.setOrigin(origin.x * textRect.width, origin.y * textRect.height);
}
