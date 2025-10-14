#include "player.h"
#include "player_manager.h"

#include <sys/socket.h>

#define BUFFER_SIZE 1024

int Player::makeRequest() {
	int id = -1;
	Msg("MOVE:" + PlayerManager::BoardState + "\n");

	char buffer[BUFFER_SIZE] = {0};
	ssize_t bytesReceived = 0;

	do {
		bytesReceived = recv(this->Socket, buffer, sizeof(buffer) - 1, 0);
		buffer[bytesReceived] = '\0';

		if (bytesReceived >= 1 && isdigit(buffer[0])) {
			if (bytesReceived <= 0) {
				printf("ERROR::PLAYER: Connection error or connection was closed");
			}

			int receivedNumber = buffer[0] - '0';
			if (receivedNumber >= 0 && receivedNumber <= 9) {
				id = receivedNumber;
				break;
			}
		}

	} while (true);

	return id;
}

int Player::Prompt() {
	if (this->Name == PlayerManager::Human) {
		int id = -1;

		printf("Next move\n> ");
		scanf("%d", &id);
		return id;

	} else {
		return this->makeRequest();
	}
}

bool Player::Run() {
	if (this->Name == PlayerManager::Human)
		return false;

	if (!this->running) {
		pid_t pid = fork();
		if (pid == 0) {
			setsid();
			std::string final_cmd = this->Cmd + "; exec bash";
			execlp("gnome-terminal", "gnome-terminal", "--", "bash", "-c", final_cmd.c_str(),
				   nullptr);
			exit(1);
		} else if (pid > 0) {
			this->terminalPid = pid;
			this->running = true;
			return true;
		}
	}

	return false;
}

void Player::Msg(std::string message) {
	if (this->Name != PlayerManager::Human) {
		send(this->Socket, message.c_str(), message.length(), 0);
	}
}

void Player::CleanUp() {
	if (running) {
		Msg("EXIT\n");

		close(Socket);

		if (this->terminalPid > 0)
			kill(this->terminalPid, SIGKILL);
	}
}
