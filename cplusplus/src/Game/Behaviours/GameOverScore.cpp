#include "GameOverScore.hpp"

#include "Game/State/Scoreboard.hpp"
#include "Core/Behaviours/TextRenderer.hpp"

GameOverScore::GameOverScore(TextRenderer& scoreText, Scoreboard& scoreboard) : GameOverScore(default_identifier(this), scoreText, scoreboard) {
}

GameOverScore::GameOverScore(const std::string& identifier, TextRenderer& scoreText, Scoreboard& scoreboard) : ScriptableBehaviour(identifier), scoreText(scoreText), scoreboard(scoreboard){}

void GameOverScore::onStart() {
	const auto score = scoreboard.getLastScore();
	scoreText.setText(string_format("SCORE: %i", score.score));
}
