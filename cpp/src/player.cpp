#include "player.h"
#include "player_manager.h"

int Player::makeRequest() {
	int id = -1;

	// TODO - Implement input
	printf("Move:%s", PlayerManager::BoardState.c_str());
	scanf(" %d", &id);
	return id;
}

int Player::Prompt() {
	if (this->Name == PlayerManager::Human) {
		int id = -1;

		printf("Next move\n> ");
		scanf("%d", &id);
		return id;

	} else if (this->Name == PlayerManager::AI_1) {
		return this->makeRequest();

	} else if (this->Name == PlayerManager::AI_2) {
		return this->makeRequest();

	} else {
		printf("ERROR::PLAYER: Invalid name: %s \n", Name.c_str());
		exit(-1);
	}
}

bool Player::Run() {
	if (this->Name == PlayerManager::Human)
		return false;

	if (!this->running) {

		this->running = true;
		return true;
	}

	return false;
}

void Player::Msg(std::string message) {
	if (this->Name == PlayerManager::AI_1) {
		// TODO - Implement output
		printf("Msg:%s", message.c_str());

	} else if (this->Name == PlayerManager::AI_2) {
		// TODO - Implement output
		printf("Msg:%s", message.c_str());
	}
}
