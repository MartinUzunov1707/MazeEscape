// MazeEscape.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>

const short ALL_STAGES = 6;
const short MAX_HEIGHT = 17;
const short MAX_WIDTH = 25;
std::fstream currentPlayerProfile;
unsigned short health;
unsigned int coins;
char stagesBeat[ALL_STAGES]{'0'};
char currentLevelMatrix[MAX_HEIGHT][MAX_WIDTH]{};
std::string currentLevelName = "maps\\";


void usernameValidation(std::string &username) {
	std::cout << "Enter username:";
	std::cin >> username;
	while (username.length() >= 50) {
		std::cout << "Username must not exceed 50 characters!" << std::endl;
		std::cout << "Enter username:";
		std::cin >> username;
	}
	username.append(".txt");
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
				loginPlayer(username);
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
			
			loginPlayer(username);
			break;
		}
		else if (input == '2') {
			std::string username;
			usernameValidation(username);

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
void countBeatenStages(int &number) {
	for (int i = 0; i < ALL_STAGES;i++) {
		if (stagesBeat[i] != '\0') {
			number++;
		}
	}
}
void loadMatrixFromFile(std::string fileName) {
	std::fstream matrix;
	matrix.open(fileName,std::ios::in);
	std::cout << matrix.is_open();
}
void loadSaveData() {
	if (currentPlayerProfile.peek() == std::fstream::traits_type::eof()) {
		health = 3;
		coins = 0;
	}
	else {
		health = currentPlayerProfile.get() - '0';
		currentPlayerProfile.seekg(2, std::fstream::cur);
		std::string temp;
		getline(currentPlayerProfile, temp);
		coins = std::stoi(temp);
		currentPlayerProfile.getline(stagesBeat, 6);
		if (currentPlayerProfile.peek() == std::fstream::traits_type::eof()) {
			int numberOfBeatenStages = 0;
			countBeatenStages(numberOfBeatenStages);
			
			char level = 'a' + numberOfBeatenStages + (1 + (rand() % 2));
			currentLevelName.insert(currentLevelName.size(),1, level);
			currentLevelName.append( + ".txt");
			loadMatrixFromFile(currentLevelName);
		}
	}
}
int main()
{
    identify();
	loadSaveData();
}


