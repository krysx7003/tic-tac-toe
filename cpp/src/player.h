#pragma once

#include <string>
#include <unistd.h>
#include <vector>

class Player {
	std::string cmd;
	bool running = false;

	int makeRequest();

  public:
	static const char O = 'O';
	static const char X = 'X';

	std::string Name;
	int Socket;
	char Type;

	Player(std::string name, char type) : Name(name), Type(type) {};
	Player() {};
	~Player() {
		if (running) {
			// Kill proc
			close(Socket);
		}
	};

	bool Run();
	int Prompt();
	void Msg(std::string);
};
