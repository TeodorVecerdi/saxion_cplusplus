#pragma once
#include <SFML/Graphics/Glsl.hpp>

#include "Core/GameObject.hpp"

class Sprite final : public GameObject {
public:
	Sprite(const std::string& identifier, std::string texturePath);
	void setPosition(const sf::Vector2f& newPosition);
	void movePosition(const sf::Vector2f& deltaPosition);
	sf::Vector2f getPosition() const;
protected:
	void onStart() override;
	void onUpdate(sf::Time ts) override;
	void onRender(sf::RenderWindow& renderWindow) const override;
private:
	std::string texturePath;
	sf::Sprite sprite;
	sf::Texture texture;
	sf::Vector2f position;
};
