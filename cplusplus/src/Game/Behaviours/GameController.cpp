#include "GameController.hpp"

#include "CharacterVisualizer.hpp"
#include "InfoArea.hpp"
#include "PlayerController.hpp"
#include "Core/IO.hpp"
#include "Core/Behaviours/SpriteRenderer.hpp"
#include "Core/Behaviours/TextRenderer.hpp"
#include "Game/State/Character.hpp"
#include "Game/State/Difficulty.hpp"

GameController::GameController(InfoArea& infoArea, PlayerController& playerController,
                               CharacterVisualizer& playerVisualizer,
                               CharacterVisualizer& enemyVisualizer, TextRenderer& playerStatusText,
                               TextRenderer& enemyStatusText, SpriteRenderer& focusRenderer) : GameController(
	default_identifier(this), infoArea, playerController, playerVisualizer, enemyVisualizer, playerStatusText,
	enemyStatusText, focusRenderer) {
}

GameController::GameController(const std::string& identifier, InfoArea& infoArea, PlayerController& playerController,
                               CharacterVisualizer& playerVisualizer, CharacterVisualizer& enemyVisualizer,
                               TextRenderer& playerStatusText, TextRenderer& enemyStatusText,
                               SpriteRenderer& focusRenderer) :
	ScriptableBehaviour(identifier), infoArea(infoArea), playerController(playerController),
	playerVisualizer(playerVisualizer),
	enemyVisualizer(enemyVisualizer), playerStatusText(playerStatusText), enemyStatusText(enemyStatusText),
	focusRenderer(focusRenderer) {

	currentEnemy = std::make_shared<Character>();
	loadedGame = false;
	player = this->playerController.getPlayer();
	focusRenderer.setEnabled(false);
}

void GameController::onStart() {
	infoArea.Clear();

	if (!loadedGame) {
		nextEnemy();
		enemiesKilled = 0;
		enemyVisualizer.setCharacter(currentEnemy);
	} else {
		loadedGame = false;
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
		onPlayerDeath(Difficulty::GetDifficulty(), enemiesKilled, player->getName());
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
	int attackPower = player->getAttackPower();
	if (playerFocused) attackPower += rand() % (attackPower / 2);
	bool evaded = currentEnemy->takeDamage(attackPower);
	if (playerFocused && evaded) {
		evaded = currentEnemy->takeDamage(attackPower);
	}
	if (playerFocused) {
		endFocus();
	}
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
	if (playerFocused) healPower += rand() % (healPower / 2);
	healPower = player->heal(healPower);
	if (playerFocused) {
		endFocus();
	}
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
		beginFocus();
	} else {
		infoArea.AddInfoMessage(string_format("%s (you) tried to focus, but failed.", player->getName().c_str()));
	}

	currentOrder = !currentOrder;
	waitingForPlayer = false;
	disablePlayer();
}

void GameController::beginFocus() {
	focusRenderer.setEnabled(true);
	playerFocused = true;
}

void GameController::endFocus() {
	focusRenderer.setEnabled(false);
	infoArea.AddInfoMessage(string_format("%s (you) is no longer focused.", player->getName().c_str()));
	playerFocused = false;
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
		healPower = currentEnemy->heal(healPower);
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

void GameController::setOnPlayerDeath(std::function<void(int,int,std::string)> onPlayerDeath) {
	this->onPlayerDeath = onPlayerDeath;

}

void GameController::nextEnemy() {
	currentEnemy->randomize(Difficulty::GetDifficultySettings().EnemyBaseAttrPoints);
	const auto name = string_format("Enemy %s", capitalize(currentEnemy->getCharacterType()).c_str());
	currentEnemy->setName(name);
	enemyVisualizer.setCharacter(currentEnemy); // update sprite
	infoArea.Clear();
	auto newEnemyMessage = string_format("A wild %s appears!", name.c_str());
	auto separator = std::string(static_cast<int>(newEnemyMessage.size()*1.3f), '-');
	infoArea.AddInfoMessage(string_format("%s\n%s", newEnemyMessage.c_str(), separator.c_str()));
}
