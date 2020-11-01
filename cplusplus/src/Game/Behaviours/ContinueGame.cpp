#include "ContinueGame.hpp"

#include "GameController.hpp"
#include "Ranking.hpp"
#include "Core/IO.hpp"
#include "Core/Behaviours/Button.hpp"
#include "Game/State/Scoreboard.hpp"

ContinueGame::ContinueGame(Button& continueButton, Button& eraseDataButton, Ranking& ranking, Scoreboard& scoreboard) : ContinueGame(default_identifier(this), continueButton, eraseDataButton, ranking, scoreboard) {
}

ContinueGame::ContinueGame(const std::string& identifier, Button& continueButton, Button& eraseDataButton, Ranking& ranking, Scoreboard& scoreboard): ScriptableBehaviour(identifier), continueButton(continueButton), eraseDataButton(eraseDataButton), gameController(nullptr), ranking(ranking), scoreboard(scoreboard) {
}

void ContinueGame::setGameController(GameController* gameController) {
	this->gameController = gameController;
}


void ContinueGame::continueGame() {
	gameController->continueGame();
}

void ContinueGame::eraseData() {
	if(std::filesystem::exists("assets/data/save_game/state.txt")) {
		std::filesystem::remove("assets/data/save_game/state.txt");
		std::filesystem::remove("assets/data/save_game/player.txt");
		std::filesystem::remove("assets/data/save_game/enemy.txt");
	}
	if(std::filesystem::exists("assets/data/highscore.txt")) {
		std::filesystem::remove("assets/data/highscore.txt");
	}
	scoreboard.clear();
	ranking.reset();
	updateButtons();
}

void ContinueGame::updateButtons() {
	if(std::filesystem::exists("assets/data/save_game/state.txt") || (std::filesystem::exists("assets/data/highscore.txt") || scoreboard.getScoreCount() != 0)) {
		eraseDataButton.setEnabled(true);
	} else {
		eraseDataButton.setEnabled(false);
	}
	if (std::filesystem::exists("assets/data/save_game/state.txt")) {
		continueButton.setEnabled(true);
	} else {
		continueButton.setEnabled(false);
	}
}

void ContinueGame::onStart() {
	updateButtons();
}
