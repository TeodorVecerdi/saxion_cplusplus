#include "InfoArea.hpp"

#include "Core/Behaviours/TextRenderer.hpp"
#include "Core/Scene/GameObjectTemplates.hpp"

InfoArea::InfoArea(const uint32_t textSize, const int maxMessages, const float padding, const float positionX,
                   const float positionY,
                   SpriteRenderer& infoAreaContainer) : InfoArea(
	default_identifier(this), textSize, maxMessages, padding, positionX, positionY, infoAreaContainer) {
}

InfoArea::InfoArea(const std::string& identifier, const uint32_t textSize, const int maxMessages, const float padding,
                   const float positionX, const float positionY, SpriteRenderer& infoAreaContainer):
	ScriptableBehaviour(identifier), textSize(textSize), padding(padding), positionX(positionX), positionY(positionY),
	maxMessages(maxMessages),
	messagesDirty(false),
	infoAreaContainer(infoAreaContainer) {
}

void InfoArea::onStart() {
	const float spacing = (infoAreaContainer.getSize().y - 2 * padding - maxMessages * textSize) / (maxMessages - 1);
	for (int i = 0; i < maxMessages; i++) {
		auto [message, messageLabel] = DeconstructLabel(
			MakeLabel("Lato", "", textSize, TextAlignment::LEFT, Theme::activeTheme->primaryText));
		message.setPosition(glm::vec2(positionX, positionY + padding + (textSize + spacing) * i + textSize / 2.0f));
		messageRenderers.push_back(&messageLabel);
		owner->addChild(message);
	}
}

void InfoArea::AddInfoMessage(const std::string message) {
	messages.push_back(message);
	if (messages.size() > maxMessages)
		messages.erase(messages.begin());

	for (auto& messageRenderer :  messageRenderers)
		messageRenderer->setText("");

	for (int i = 0; i < messages.size(); i++)
		messageRenderers[i]->setText(messages[i]);
}
