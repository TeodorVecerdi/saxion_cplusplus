#pragma once
#include <SFML/Graphics/Glsl.hpp>

#include "Core/GameObject.hpp"

class Sprite final : public GameObject {
public:
	Sprite(const std::string& identifier, const std::string& texturePath);
protected:
	void onStart() override;
	void onUpdate(sf::Time ts) override;
	void onRender(sf::RenderWindow& renderWindow) const override;
private:
	sf::Sprite sprite;
	sf::Texture texture;
};
