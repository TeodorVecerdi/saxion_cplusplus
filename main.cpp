#include <SFML/Graphics.hpp>

#include "Sprite.hpp"

int main() {
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Hello SFML");
	window.setView(sf::View(sf::FloatRect(0.0f, 0.0f, 1280, 720)));

	sf::Texture texture;
	if (!texture.loadFromFile("res/testTexture.png")) {
		return -1;
	}

	Sprite sprites[2]{
		Sprite(texture, sf::Vector2f(0, 0), 10),
		Sprite(texture, sf::Vector2f(0, 400), 5)
	};

	while (window.isOpen()) {
		sf::Event event{};
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Escape)) {
				window.close();
			}

			for (Sprite& sprite : sprites) {
				sprite.update(event);
			}
		}
		window.clear();
		for (const Sprite& sprite : sprites) {
			sprite.render(window);
		}
		window.display();
	}
}
