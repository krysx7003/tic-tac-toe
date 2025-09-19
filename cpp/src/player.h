#pragma once

#include <string>
#include <vector>

class Player {
	int makeRequest();

  public:
	static const char O = 'O';
	static const char X = 'X';

	std::string option;
	std::string Cmd;
	char Type;

	Player(std::string option, char type) : option(option), Type(type) {};
	Player() {};

	int Prompt();
	void Connect();
};
