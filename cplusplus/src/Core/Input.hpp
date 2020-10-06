#pragma once
#include <SFML/Window/Event.hpp>

class Input {
public:
	inline static int mouseX;
	inline static int mouseY;

	static void setKey(int key, sf::Event::EventType mode) {
		bool press = mode == sf::Event::EventType::KeyPressed;
		if (press) {
			keydown[key] = true;
			lastKeyDown = key;
		} else {
			keyup[key] = true;
			lastKeyUp = key;
		}
		keys[key] = press;
		lastKey = key;
	}

	static void setButton(int button, sf::Event::EventType mode) {
		bool press = mode == sf::Event::EventType::MouseButtonPressed;
		if (press) mousehits[button] = true;
		else mouseup[button] = true;
		buttons[button] = press;
	}

	static bool GetKey(int key) {
		return keys[key];
	}

	static bool GetKeyDown(int key) {
		return keydown[key];
	}

	static bool GetKeyUp(int key) {
		return keyup[key];
	}

	static bool AnyKey() {
		for (int i = 0; i < MAXKEYS; i++) if (keys[i]) return true;
		return false;
	}

	static bool AnyKeyDown() {
		for (int i = 0; i < MAXKEYS; i++) if (keydown[i]) return true;
		return false;
	}

	static bool AnyKeyUp() {
		for (int i = 0; i < MAXKEYS; i++) if (keyup[i]) return true;
		return false;
	}

	static int LastKey() {
		return lastKey;
	}

	static int LastKeyDown() {
		return lastKeyDown;
	}

	static int LastKeyUp() {
		return lastKeyUp;
	}

	static bool GetMouseButton(int button) {
		return buttons[button];
	}

	static bool GetMouseButtonDown(int button) {
		return mousehits[button];
	}

	static bool GetMouseButtonUp(int button) {
		return mouseup[button];
	}

	static void resetHitCounters() {
		std::fill_n(keydown, MAXKEYS, false);
		std::fill_n(keyup, MAXKEYS, false);
		std::fill_n(mousehits, MAXBUTTONS, false);
		std::fill_n(mouseup, MAXBUTTONS, false);
	}

	static void updateMouse(int x, int y) {
		mouseX = x;
		mouseY = y;
	}

private:
	inline const static int MAXKEYS = 65535;
	inline const static int MAXBUTTONS = 255;
	inline static bool keys[MAXKEYS + 1];
	inline static bool keydown[MAXKEYS + 1];
	inline static bool keyup[MAXKEYS + 1];
	inline static bool buttons[MAXBUTTONS + 1];
	inline static bool mousehits[MAXBUTTONS + 1];
	inline static bool mouseup[MAXBUTTONS + 1];
	inline static int lastKey;
	inline static int lastKeyDown;
	inline static int lastKeyUp;
};
