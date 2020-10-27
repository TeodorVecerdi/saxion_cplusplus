#pragma once
#include "Core/Scene/ScriptableBehaviour.hpp"

class TextRenderer;
class SpriteRenderer;

class InfoArea final : public ScriptableBehaviour {
public:
	InfoArea(uint32_t textSize, int maxMessages, float padding, float positionX, float positionY, SpriteRenderer& infoAreaContainer);
	InfoArea(const std::string& identifier, uint32_t textSize, int maxMessages, float padding, float positionX, float positionY, SpriteRenderer& infoAreaContainer);
	void AddInfoMessage(std::string message);
protected:
	void onStart() override;
private:
	uint32_t textSize;
	float padding;
	float positionX;
	float positionY;
	int maxMessages;
	bool messagesDirty;
	SpriteRenderer& infoAreaContainer;
	std::vector<std::string> messages;
	std::vector<TextRenderer*> messageRenderers;
};
