#pragma once
#include <memory>
#include <SFML/Graphics/Color.hpp>

class Theme {
public:
	sf::Color primaryText;
	sf::Color secondaryText;
	sf::Color background;
	sf::Color panelDark;
	sf::Color panelLight;
	sf::Color themeColor1;
	sf::Color themeColor2;

	sf::Color buttonNormal;
	sf::Color buttonActive;
	sf::Color buttonHover;
	sf::Color buttonDisabled;

	
	static void SetActiveTheme(std::unique_ptr<Theme>& activeTheme) {
		Theme::activeTheme = activeTheme.get();
	}
	inline static Theme* activeTheme;
private:
};
