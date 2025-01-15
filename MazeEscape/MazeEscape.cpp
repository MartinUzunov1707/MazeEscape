// MazeEscape.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
std::fstream currentPlayerProfile;

void usernameValidation(std::string &username) {
	std::cout << "Enter username:";
	std::cin >> username;
	while (username.length() >= 50) {
		std::cout << "Username must not exceed 50 characters!" << std::endl;
		std::cout << "Enter username:";
		std::cin >> username;
	}
}
void registerPlayer(std::string username) {
	currentPlayerProfile.open(username);
	if (currentPlayerProfile) {
		std::cout << "Username already exist. Logged in succesfully!" << std::endl;
	}
	else {
		currentPlayerProfile.open(username, std::ios::out);
		std::cout << "Registered successfully!" << std::endl;
	}
}
void loginPlayer(std::string username) {

	currentPlayerProfile.open(username);
	while (!currentPlayerProfile) {

		std::cout << "Username does not exist. Do you wish to register?" << std::endl;
		std::cout << "1.Yes" << std::endl;
		std::cout << "2.No, enter new username" << std::endl;
		std::cout << "3.Exit" << std::endl;
		std::cout << "Input:";
		char input;
		std::cin >> input;
		switch (input) {
			case '1':
				registerPlayer(username);
			return;
			case '2':
				usernameValidation(username);
				break;
			case '3':
				exit(EXIT_SUCCESS);
				break;
			default:
				std::cout << "Invalid command!" << std::endl;
		}

	}
	
}


void identify() {
	std::cout << "Welcome to Dungeon Escape!" << std::endl;
	std::cout << "1.Login" << std::endl;
	std::cout << "2.Register" << std::endl;
	std::cout << "3.Exit" << std::endl;
	std::cout << "Input:";
	char input = 0;
	std::cin >> input;
	while (true) {
		if (input == '1') {
			std::string username;
			usernameValidation(username);
			username.append(".txt");

			loginPlayer(username);
			break;
		}
		else if (input == '2') {
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


