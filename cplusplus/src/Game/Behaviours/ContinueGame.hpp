#pragma once
#include "Core/Scene/ScriptableBehaviour.hpp"

class Ranking;
class Scoreboard;
class GameController;
class Button;

class ContinueGame final : public ScriptableBehaviour {
public:
	ContinueGame(Button& continueButton, Button& eraseDataButton, Ranking& ranking, Scoreboard& scoreboard);
	ContinueGame(const std::string& identifier, Button& continueButton, Button& eraseDataButton, Ranking& ranking, Scoreboard& scoreboard);
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
	Ranking& ranking;
	Scoreboard& scoreboard;
};
