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
	maxMessages(maxMessages), infoAreaContainer(infoAreaContainer) {
}

void InfoArea::AddInfoMessage(const std::string message) {
	// Make sure messages were initialized before adding a message.
	// Could have put this inside onStart, but if other behaviours
	// add messages in their onStart, then I can't guarantee this
	// onStart was called before the other onStart.
	if (!initializedMessages) {
		initializedMessages = true;
		const auto spacing = (infoAreaContainer.getSize().y - 2 * padding - maxMessages * textSize) / (maxMessages - 1);
		for (int i = 0; i < maxMessages; i++) {
			auto [message, messageLabel] = DeconstructLabel(
				MakeLabel("Lato", "", textSize, TextAlignment::LEFT, Theme::activeTheme->primaryText));
			message.setPosition(glm::vec2(positionX, positionY + padding + (textSize + spacing) * i + textSize / 2.0f));
			messageRenderers.push_back(&messageLabel);
			owner->addChild(message);
		}
		messages.clear();
	}

	messages.push_back(message);
	if (messages.size() > maxMessages)
		messages.erase(messages.begin());

	for (auto messageRenderer : messageRenderers)
		messageRenderer->setText("");

	for (int i = 0; i < messages.size(); i++) {
		messageRenderers[i]->setText(messages[i]);
		const auto messageBounds = messageRenderers[i]->getGlobalBounds();
		const auto areaContainerSize = infoAreaContainer.getSize();

		// split message if longer than info area
		if (messageBounds.width > areaContainerSize.x - 2.0f * padding) {
			auto splitMessage = messages[i];
			auto splitStart = static_cast<int>(floor(
				static_cast<float>(splitMessage.size()) * ((areaContainerSize.x - 2.0f * padding) / messageBounds.width
				)));
			for (; splitStart >= 0; splitStart--) {
				if (splitMessage[splitStart] == ' ') break;
			}
			if (splitStart != 0) {
				splitMessage[splitStart] = '\n';
				messageRenderers[i]->setText(splitMessage);
			}
		}
	}
}

void InfoArea::Clear() {
	for (auto messageRenderer : messageRenderers)
		messageRenderer->setText("");
	messages.clear();
}
