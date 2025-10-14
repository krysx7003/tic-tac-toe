#pragma once

#include <signal.h>
#include <string>
#include <unistd.h>
#include <vector>

class Player {

	bool running = false;
	int terminalPid = 0;

	int makeRequest();

  public:
	static const char O = 'O';
	static const char X = 'X';

	std::string Name;
	std::string Cmd; //"nc localhost 8080"
	int Socket;
	char Type;

	Player(std::string name, char type, std::string cmd) : Name(name), Type(type), Cmd(cmd) {};
	Player() {};
	~Player() {

	};

	void CleanUp();
	bool Run();
	int Prompt();
	void Msg(std::string);
};
