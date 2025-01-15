// MazeEscape.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
std::fstream currentPlayerProfile;

void usernameValidation(std::string &username) {
	std::cin >> username;
	while (username.length() >= 50) {
		std::cout << "Username must not exceed 50 characters!" << std::endl;
		std::cin >> username;
	}
}
void loginPlayer(std::string username) {

}
void registerPlayer(std::string username) {
	currentPlayerProfile.open(username);
	if (currentPlayerProfile) {
		std::cout << "Username already exist. Logged in succesfully!" << std::endl;
	}
}

void identify() {
	std::cout << "Welcome to Dungeon Escape!" << std::endl;
	std::cout << "1.Login" << std::endl;
	std::cout << "2.Register" << std::endl;
	std::cout << "3.Exit" << std::endl;
	std::cout << "Input: ";
	char input = 0;
	std::cin >> input;
	while (true) {
		if (input == '1') {
			std::cout << "Please enter username:";
			std::string username;
			usernameValidation(username);
			username.append(".txt");

			loginPlayer(username);
			break;
		}
		else if (input == '2') {
			std::cout << "Please enter username:";
			std::string username;
			usernameValidation(username);
			username.append(".txt");

			registerPlayer(username);
			break;
		}
		else if (input == '3') {
			exit(EXIT_SUCCESS);
			break;
		}
		std::cout << "Invalid command, please try again!" << std::endl;
		std::cout << "Input: ";
		std::cin >> input;
	}
	
}

int main()
{
    identify();
}


