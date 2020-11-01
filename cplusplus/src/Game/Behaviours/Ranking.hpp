#pragma once
#include "Core/Scene/ScriptableBehaviour.hpp"

class Scoreboard;
class SpriteRenderer;
class TextRenderer;

class Ranking final : public ScriptableBehaviour {
public:
	Ranking(uint32_t textSize, float padding, float positionX, float positionY, SpriteRenderer& parentContainer, Scoreboard& scoreboard);
	Ranking(const std::string& identifier, uint32_t textSize, float padding, float positionX, float positionY, SpriteRenderer& parentContainer, Scoreboard& scoreboard);
	void clear();
	void reset();
protected:
	void onStart() override;
private:
	uint32_t textSize;
	float padding;
	float positionX;
	float positionY;
	bool initializedRankings;
	SpriteRenderer& parentContainer;
	Scoreboard& scoreboard;
	std::vector<std::string> scores;
	std::vector<TextRenderer*> scoreRenderers;
};
