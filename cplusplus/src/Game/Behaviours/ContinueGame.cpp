#include "ContinueGame.hpp"

#include "GameController.hpp"
#include "Core/Behaviours/Button.hpp"

ContinueGame::ContinueGame(Button& continueButton, Button& eraseDataButton) : ContinueGame(default_identifier(this), continueButton, eraseDataButton) {
}

ContinueGame::ContinueGame(const std::string& identifier, Button& continueButton, Button& eraseDataButton): ScriptableBehaviour(identifier), continueButton(continueButton), eraseDataButton(eraseDataButton), gameController(nullptr) {
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
	updateButtons();
}

void ContinueGame::updateButtons() {
	if (!std::filesystem::exists("assets/data/saved-game.txt")) {
		continueButton.setEnabled(false);
		eraseDataButton.setEnabled(false);
	} else {
		continueButton.setEnabled(true);
		eraseDataButton.setEnabled(true);
	}
}

void ContinueGame::onStart() {
	updateButtons();
}
