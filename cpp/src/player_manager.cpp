#include "player_manager.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define SOCKET_ERROR (-1)
#define SERVER_SOCKET 8080
#define BUFFER_SIZE 1024

std::vector<std::string> PlayerManager::options = {Human, AI_1, AI_2};

std::vector<std::string> PlayerManager::GetOptions() { return options; }

bool PlayerManager::RequestHandled;

Player PlayerManager::player1 = Player("Human", Player::O);
Player PlayerManager::player2 = Player("Human", Player::X);
char PlayerManager::Curr_player = Player::O;
std::string PlayerManager::BoardState = "/ / / / / / / / / /";

int PlayerManager::serverSocket;

void PlayerManager::PrintOptions() {
	for (int i = 0; i < options.size(); i++) {
		printf("%d. %s\n", i, options[i].c_str());
	}
}

std::string PlayerManager::GetOption(int id) { return options[id]; }

bool PlayerManager::ValidOption(char c) {
	if (c > '0' && c < '9')
		return false;

	int id = c - '0';
	if (id >= options.size())
		return false;

	return true;
}

void PlayerManager::StartServer() {
	check((serverSocket = socket(AF_INET, SOCK_STREAM, 0)), "Socket creation failed!\n");

	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(SERVER_SOCKET);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	check(bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)),
		  "Bind failed");

	check(listen(serverSocket, 10), "Listen failed");

	if (player1.Run()) {
		check((player1.Socket = accept(serverSocket, nullptr, nullptr)), "Accept failed");
		player1.Msg("Name:O");
	} else {
		player1.Msg("Restart");
	}

	if (player2.Run()) {
		check((player2.Socket = accept(serverSocket, nullptr, nullptr)), "Accept failed");
		player2.Msg("Name:X");
	} else {
		player2.Msg("Restart");
	}
}

void PlayerManager::CloseServer() { close(serverSocket); }

void PlayerManager::check(int res, const char *message) {
	if (res == SOCKET_ERROR) {
		std::string errorMessage = std::string(message) + ". Exiting...";
		printf("%s\n", errorMessage.c_str());
		exit(-1);
	}
}

int PlayerManager::MakeMove() {
	if (Curr_player == Player::O) {
		return player1.Prompt();
	} else {
		return player2.Prompt();
	}
}

bool PlayerManager::CurrPlayerHuman() {
	if (Curr_player == Player::O) {
		return player1.Name == options[0];
	} else {
		return player2.Name == options[0];
	}
}

void PlayerManager::SetPlayers(std::string playerO, std::string playerX) {
	if (player1.Name != playerO)
		player1 = Player(playerO, Player::O);

	if (player2.Name != playerX)
		player2 = Player(playerX, Player::X);
}

void PlayerManager::MsgAll(std::string message) {
	player1.Msg(message);
	player2.Msg(message);
}
