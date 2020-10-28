#include "GameController.hpp"

#include "CharacterVisualizer.hpp"
#include "InfoArea.hpp"
#include "PlayerController.hpp"
#include "Core/IO.hpp"
#include "Core/Behaviours/TextRenderer.hpp"
#include "Game/Character.hpp"
#include "Game/State/Difficulty.hpp"

GameController::GameController(InfoArea& infoArea, PlayerController& playerController,
                               CharacterVisualizer& playerVisualizer,
                               CharacterVisualizer& enemyVisualizer, TextRenderer& playerStatusText,
                               TextRenderer& enemyStatusText) : GameController(
	default_identifier(this), infoArea, playerController, playerVisualizer, enemyVisualizer, playerStatusText,
	enemyStatusText) {
}

GameController::GameController(const std::string& identifier, InfoArea& infoArea, PlayerController& playerController,
                               CharacterVisualizer& playerVisualizer, CharacterVisualizer& enemyVisualizer,
                               TextRenderer& playerStatusText, TextRenderer& enemyStatusText) :
	ScriptableBehaviour(identifier), infoArea(infoArea), playerController(playerController),
	playerVisualizer(playerVisualizer),
	enemyVisualizer(enemyVisualizer), playerStatusText(playerStatusText), enemyStatusText(enemyStatusText) {

	currentEnemy = new Character();
	loadedGame = false;
	player = this->playerController.getPlayer();
}

void GameController::onStart() {
	if (!loadedGame) {
		nextEnemy();
		enemiesKilled = 0;
		enemyVisualizer.setCharacter(currentEnemy);
	}

	playerVisualizer.setCharacter(player);
}

void GameController::onUpdate(const sf::Time ts) {
	bool playerAttacks = attackPriority ^ currentOrder;
	if (playerAttacks && !waitingForPlayer) {
		waitingForPlayer = true;
		if (playerStatusTextTimer <= 0) playerStatusText.setText("Thinking...");
		enablePlayer();
	} else if (!playerAttacks && !waitingForEnemy) {
		waitingForEnemy = true;
		disablePlayer();
		if (enemyStatusTextTimer <= 0) enemyStatusText.setText("...Thinking");
		// wait between 1 and 4 seconds for enemy action
		waitForEnemyTimer = static_cast<float>(rand() % 30 + 11) / 10.0f;
	}
	if (waitingForEnemy && playerStatusTextTimer <= 0)
		playerStatusText.setText("");
	else if (waitingForPlayer && enemyStatusTextTimer <= 0)
		enemyStatusText.setText("");

	if (playerStatusTextTimer > 0) {
		playerStatusTextTimer -= ts.asSeconds();
		if (playerStatusTextTimer <= 0) playerStatusText.setText("Thinking...");
	}

	if (enemyStatusTextTimer > 0) {
		enemyStatusTextTimer -= ts.asSeconds();
		if (enemyStatusTextTimer <= 0) enemyStatusText.setText("...Thinking");
	}

	if (waitingForEnemy) {
		waitForEnemyTimer -= ts.asSeconds();
		if (waitForEnemyTimer <= 0.0f) {
			waitingForEnemy = false;
			enemyAct();
			currentOrder = !currentOrder;
		}
	}

	if (currentEnemy->getHealth() <= 0) {
		reset();
		enemiesKilled++;
		currentOrder = false;
		attackPriority = rand() % 2 == 0;
		nextEnemy();
		return;
	}
	if (player->getHealth() <= 0) {
		// TODO: Implement player death.
	}
}

void GameController::reset() {
	enemyStatusTextTimer = 0.0f;
	enemyStatusText.setText("...Thinking");
	waitingForEnemy = false;
	waitForEnemyTimer = 0.0f;
	enemyVisualizer.setState(IDLE);
	playerStatusTextTimer = 0.0f;
	playerStatusText.setText("Thinking...");
	waitingForPlayer = false;
	playerVisualizer.setState(IDLE);
}

void GameController::enablePlayer() {
	playerController.enableControls();
}

void GameController::disablePlayer() {
	playerController.disableControls();
}

void GameController::playerAttack() {
	playerVisualizer.setState(ATTACK);
	enemyVisualizer.setState(HURT);
	const int attackPower = player->getAttackPower();
	const bool evaded = currentEnemy->takeDamage(attackPower);
	if (evaded) {
		infoArea.AddInfoMessage(string_format("%s (you) tried to attack %s, but missed.", player->getName().c_str(),
		                                      currentEnemy->getName().c_str()));
	} else {
		infoArea.AddInfoMessage(string_format("%s (you) successfully attacked %s taking away %i health points.",
		                                      player->getName().c_str(), currentEnemy->getName().c_str(), attackPower));
	}
	currentOrder = !currentOrder;
	waitingForPlayer = false;
	playerStatusText.setText("Attacking!");
	playerStatusTextTimer = statusTextTime;
	disablePlayer();
}

void GameController::playerHeal() {
	int healPower = player->getHealPower();
	player->heal(healPower);
	infoArea.AddInfoMessage(string_format("%s (you) recovered %i health points.", player->getName().c_str(),
	                                      healPower));
	currentOrder = !currentOrder;
	waitingForPlayer = false;
	playerStatusText.setText("Healing!");
	playerStatusTextTimer = statusTextTime;
	disablePlayer();
}

void GameController::playerFocus() {
	const int successChance = rand() % 100;
	if (successChance < Difficulty::GetDifficultySettings().PlayerFocusChance) {
		infoArea.AddInfoMessage(string_format(
			"%s (you) successfully focused. Their next attack is less likely to miss and more likely to cause more damage.",
			player->getName().c_str()));
		playerStatusText.setText("Focusing!");
		playerStatusTextTimer = statusTextTime;
	} else {
		infoArea.AddInfoMessage(string_format("%s (you) tried to focus, but failed.", player->getName().c_str()));
	}

	currentOrder = !currentOrder;
	waitingForPlayer = false;
	disablePlayer();
}

void GameController::continueGame() {
	currentEnemy->loadFromFile("assets/data/save_game/enemy.txt");
	player->loadFromFile("assets/data/save_game/player.txt");
	enemyVisualizer.setCharacter(currentEnemy);
	playerVisualizer.setCharacter(player);

	if (IO::BeginRead("assets/data/save_game/state.txt")) {
		IO::ReadInt(enemiesKilled);
		IO::ReadBool(currentOrder);
		IO::ReadBool(attackPriority);
		int difficulty;
		IO::ReadInt(difficulty);
		Difficulty::SetActiveDifficulty(difficulty);
		IO::EndRead();
	}
	reset();
	loadedGame = true;
}

void GameController::saveGame() const {
	if (IO::BeginWrite("assets/data/save_game/state.txt")) {
		IO::WriteTitle("###### GAME STATE");
		IO::Write(enemiesKilled, "Enemies killed");
		IO::Write(currentOrder, "Current order");
		IO::Write(attackPriority, "Attack priority");
		IO::Write(Difficulty::GetDifficulty(), "Difficulty");
		IO::EndWrite();
	}
	player->exportCharacter("assets/data/save_game/player.txt", std::ios::out);
	currentEnemy->exportCharacter("assets/data/save_game/enemy.txt", std::ios::out);
}

void GameController::enemyAct() {
	const int enemyChance = rand() % 100;
	if (enemyChance < Difficulty::GetDifficultySettings().EnemyAttackChance) {
		enemyVisualizer.setState(ATTACK);
		playerVisualizer.setState(HURT);
		const int attackPower = currentEnemy->getAttackPower();
		const bool evaded = player->takeDamage(attackPower);
		if (evaded) {
			infoArea.AddInfoMessage(string_format("%s tried to attack %s (you), but missed.",
			                                      currentEnemy->getName().c_str(), player->getName().c_str()));
		} else {
			infoArea.AddInfoMessage(string_format("%s successfully attacked %s (you) taking away %i health points.",
			                                      currentEnemy->getName().c_str(), player->getName().c_str(),
			                                      attackPower));
		}
		enemyStatusText.setText("Attacking!");
		enemyStatusTextTimer = statusTextTime;
	} else if (enemyChance < Difficulty::GetDifficultySettings().EnemyHealChance) {
		int healPower = currentEnemy->getHealPower();
		currentEnemy->heal(healPower);
		enemyStatusText.setText("Healing!");
		infoArea.AddInfoMessage(string_format("%s recovered %i health points.", currentEnemy->getName().c_str(),
		                                      healPower));
		enemyStatusTextTimer = statusTextTime;
	} else if (enemyChance < Difficulty::GetDifficultySettings().EnemyDoNothingChance) {
		// Do nothing
		enemyStatusText.setText("Trembling in fear!");
		infoArea.AddInfoMessage(string_format("%s trembles in fear.", currentEnemy->getName().c_str()));
		enemyStatusTextTimer = statusTextTime;
	}
}

void GameController::nextEnemy() {
	currentEnemy->randomize(Difficulty::GetDifficultySettings().EnemyBaseAttrPoints);
	const auto name = capitalize(currentEnemy->getCharacterType());
	currentEnemy->setName(name);
	enemyVisualizer.setCharacter(currentEnemy); // update sprite
}
