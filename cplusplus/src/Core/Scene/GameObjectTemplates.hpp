#pragma once
#include "Core/Scene/GameObject.hpp"
#include "Core/Behaviours/Button.hpp"
#include "Core/Behaviours/SpriteRenderer.hpp"
#include "Core/Behaviours/TextRenderer.hpp"

inline GameObject& MakeEmpty() {
    auto* object = new GameObject();
    return *object;
}

inline GameObject& MakeButton(const std::string& spritePath, const glm::vec2 size, const std::string& fontName, const std::string& labelText, uint32_t fontSize, TextAlignment textAlignment, sf::Color fillColor) {
    auto* object = new GameObject();
    auto& sprite = object->addScriptableBehaviour<SpriteRenderer>(spritePath);
    sprite.setSize(size);
    object->addScriptableBehaviour<TextRenderer>(fontName, labelText, fontSize, textAlignment, fillColor);
    object->addScriptableBehaviour<Button>(sprite);
    return *object;
}

inline std::tuple<GameObject&, SpriteRenderer&, TextRenderer&, Button&> DeconstructButton(GameObject& object) {
    auto* sprite = object.getScriptableBehaviour<SpriteRenderer>();
    auto* label = object.getScriptableBehaviour<TextRenderer>();
    auto* button = object.getScriptableBehaviour<Button>();
    return std::forward_as_tuple(object, *sprite, *label, *button);
}

inline GameObject& MakeSprite(const std::string& spritePath, const glm::vec2 size) {
    auto* object = new GameObject();
    auto& sprite = object->addScriptableBehaviour<SpriteRenderer>(spritePath);
    sprite.setSize(size);
    return *object;
}

inline std::tuple<GameObject&, SpriteRenderer&> DeconstructSprite(GameObject& object) {
    auto* sprite = object.getScriptableBehaviour<SpriteRenderer>();
    return std::forward_as_tuple(object, *sprite);
}

inline GameObject& MakeLabel(const std::string& fontName, const std::string& labelText, uint32_t fontSize, TextAlignment textAlignment, sf::Color fillColor) {
    auto* object = new GameObject();
    object->addScriptableBehaviour<TextRenderer>(fontName, labelText, fontSize, textAlignment, fillColor);
    return *object;
}

inline std::tuple<GameObject&, TextRenderer&> DeconstructLabel(GameObject& object) {
    auto* label = object.getScriptableBehaviour<TextRenderer>();
    return std::forward_as_tuple(object, *label);
}
