#include "Ranking.hpp"

#include "Core/Scene/GameObjectTemplates.hpp"
#include "Game/State/Difficulty.hpp"
#include "Game/State/Scoreboard.hpp"

Ranking::Ranking(uint32_t textSize, float padding, float positionX, float positionY, SpriteRenderer& parentContainer, Scoreboard& scoreboard):
Ranking(default_identifier(this), textSize, padding, positionX, positionY, parentContainer, scoreboard){}

Ranking::Ranking(const std::string& identifier, uint32_t textSize, float padding, float positionX, float positionY, SpriteRenderer& parentContainer, Scoreboard& scoreboard):
ScriptableBehaviour(identifier), textSize(textSize), padding(padding), positionX(positionX), positionY(positionY), parentContainer(parentContainer), scoreboard(scoreboard), initializedRankings(false) {}

void Ranking::onStart() {
	const auto maxScores = scoreboard.getMaxScores();
	if (!initializedRankings) {
		initializedRankings = true;
		const auto spacing = (parentContainer.getSize().y - 2 * padding - maxScores * textSize) / (maxScores - 1);
		for (int i = 0; i < maxScores; i++) {
			auto [score, scoreLabel] = DeconstructLabel(
                MakeLabel("Fira Code", "", textSize, TextAlignment::LEFT, Theme::activeTheme->primaryText));
			score.setPosition(glm::vec2(positionX, positionY + padding + (textSize + spacing) * i + textSize / 2.0f));
			scoreRenderers.push_back(&scoreLabel);
			owner->addChild(score);
		}
		scores.clear();
	}
	reset();
}

void Ranking::clear() {
	for (auto scoreRenderer : scoreRenderers)
		scoreRenderer->setText("");
	scores.clear();
}

void Ranking::reset() {
	clear();
	const auto scoreCount = scoreboard.getScoreCount();
	for(int i = 0; i < scoreCount; i++) {
		const auto& currentScore = scoreboard.getScore(i);
		const auto difficulty = uppercase(Difficulty::GetDifficultyName(currentScore.difficulty)); 
		scores.push_back(string_format("%i. [%s]%s %s%s- %i", i+1, difficulty.c_str(), get_spacing_tabs("[normal]", 2).c_str(), currentScore.name.c_str(), get_spacing_tabs(currentScore.name, 3).c_str(), currentScore.score));
		scoreRenderers[i]->setText(scores[i]);
	}
}
