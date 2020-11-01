#pragma once
#include "Core/Scene/ScriptableBehaviour.hpp"

class TextRenderer;
class Scoreboard;

class GameOverScore final : public ScriptableBehaviour {
public:
	GameOverScore(TextRenderer& scoreText, Scoreboard& scoreboard);
	GameOverScore(const std::string& identifier, TextRenderer& scoreText, Scoreboard& scoreboard);
protected:
	void onStart() override;
private:
	TextRenderer& scoreText;
	Scoreboard& scoreboard;
};
