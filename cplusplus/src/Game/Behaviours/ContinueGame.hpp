#pragma once
#include "Core/Scene/ScriptableBehaviour.hpp"

class GameController;
class Button;

class ContinueGame final : public ScriptableBehaviour {
public:
	ContinueGame(Button& continueButton, Button& eraseDataButton);
	ContinueGame(const std::string& identifier, Button& continueButton, Button& eraseDataButton);
	void setGameController(GameController* gameController);
	void continueGame();
	void eraseData();
protected:
	void updateButtons();
	void onStart() override;
private:
	Button& continueButton;
	Button& eraseDataButton;
	GameController* gameController;
};
