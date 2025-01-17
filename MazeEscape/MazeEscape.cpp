// MazeEscape.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>

const short ALL_STAGES = 6;
const short MAX_HEIGHT = 18;
const short MAX_WIDTH = 26;
const short MAX_PORTAL_COORDINATES = 6;

std::fstream currentPlayerProfile;
unsigned short health;
unsigned int coins;
char stagesBeat[ALL_STAGES]{'\0'};
char currentLevelMatrix[MAX_HEIGHT][MAX_WIDTH]{};
std::string currentLevelName = "maps\\.txt";
short portalCoordinates[6];
int playerRow, playerCol;
std::string username;


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

void clearBeatenStages() {
	stagesBeat[0] = 'n';
	for (int i = 1; i < ALL_STAGES;i++) {
		stagesBeat[i] = '\0';
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
			usernameValidation(username);
			
			loginPlayer(username);
			break;
		}
		else if (input == '2') {
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
	//removes last 4 characters from username - ".txt"
	username = username.substr(0, username.length() - 4);
}

int countBeatenStages() {
	if (stagesBeat[0] == 'n') {
		return -1;
	}
	int number = 0;
	for (int i = 0; i < ALL_STAGES;i++) {
		if (stagesBeat[i] != '\0') {
			number++;
		}
	}
	return number;
}

void flipPortalCoordinates() {
	for (int i = 0; i < (MAX_PORTAL_COORDINATES / 2);i++) {
		short temp = portalCoordinates[i];
		portalCoordinates[i] = portalCoordinates[MAX_PORTAL_COORDINATES -1 - i];
		portalCoordinates[MAX_PORTAL_COORDINATES -1 -i] = temp;
	}
}

void loadMatrixFromFile(std::string fileName) {
	std::fstream matrix;
	matrix.open(fileName,std::ios::in);
	int portalIndex = 0;
	for (int i = 0; i < MAX_HEIGHT; i++){
		for (int a = 0; a < MAX_WIDTH; a++) {
			if (matrix.peek() != '\n') {
				if (matrix.peek() == '%') {
					portalCoordinates[portalIndex] = a;
					portalCoordinates[portalIndex + 1] = i;
					portalIndex += 2;
				}
				else if (matrix.peek() == '@') {
					playerRow = i;
					playerCol = a;
				}
				currentLevelMatrix[i][a] = matrix.get();
			}
			else {
				matrix.get();
				break;
			}
		}
	}
	flipPortalCoordinates();
}

void loadMatrixFromPlayerFile() {

	int portalIndex = 0;
	char r = currentPlayerProfile.get();
	for (int i = 0; i < MAX_HEIGHT; i++) {
		for (int a = 0; a < MAX_WIDTH; a++) {
			if (currentPlayerProfile.peek() != '\n') {
				if (currentPlayerProfile.peek() == '%') {
					portalCoordinates[portalIndex] = a;
					portalCoordinates[portalIndex + 1] = i;
					portalIndex += 2;
				}
				else if (currentPlayerProfile.peek() == '@') {
					playerRow = i;
					playerCol = a;
				}
				currentLevelMatrix[i][a] = currentPlayerProfile.get();
			}
			else {
				currentPlayerProfile.get();
				break;
			}
		}
	}
	flipPortalCoordinates();
}

void goToNextLevel() {
	int numberOfBeatenStages = countBeatenStages();
	char level = 'a' + numberOfBeatenStages + (1 + (rand() % 2));
	if (currentLevelName.length() == 10) {
		currentLevelName[5] = level;
	}
	else {
		currentLevelName.insert(5, 1, level);
	}
	loadMatrixFromFile(currentLevelName);
}

void saveAndQuit(bool onLevelComplete) {
	currentPlayerProfile.close();
	username.append(".txt");
	currentPlayerProfile.open(username, std::ios::out);
	currentPlayerProfile << health << std::endl;
	currentPlayerProfile << coins << std::endl;
	currentPlayerProfile << stagesBeat << std::endl;
	if (!onLevelComplete) {
		currentPlayerProfile << currentLevelName[5] << std::endl;
		for (int i = 0; i < MAX_HEIGHT;i++) {
			for (int a = 0; a < MAX_WIDTH; a++)
			{
				if (currentLevelMatrix[i][a] == '\0' || currentLevelMatrix[i][a] == -1) {
					break;
				}
				currentPlayerProfile << currentLevelMatrix[i][a];
			}
			currentPlayerProfile << std::endl;
		}
	}
	currentPlayerProfile.close();
	exit(EXIT_SUCCESS);
}

std::string mapToLevel(char input) {
	switch (input) {
	case 'a':
	case 'b':
		return "Easy";
	case 'c':
	case 'd':
		return "Medium";
	case 'e':
	case 'f':
		return "Hard";
	}
}

void playCompletedLevel() {
	std::cout << "Choose level to play again:" << std::endl;
	int numberOfcompletedStages = countBeatenStages();
	for (int i = 1; i <= numberOfcompletedStages; i++) {
		std::cout << i << "." << mapToLevel(stagesBeat[i - 1]) << std::endl;
	}
	char res;
	std::cout << "Input:";
	std::cin >> res;
	while (true) {
		if (res <= '3' && res >= '1') {
			if (currentLevelName.length() == 10) {
				currentLevelName[5] = stagesBeat[(res - '0') - 1];
			}
			else {
				currentLevelName.insert(5, 1, stagesBeat[(res - '0')-1]);
			}
			loadMatrixFromFile(currentLevelName);
			break;
		}
		std::cout << "Invalid command!" << std::endl;
		std::cout << "Input:";
		std::cin >> res;
	}
}

void loadSaveData() {
	std::cout << "Welcome, " << username << '!' << std::endl;
	if (currentPlayerProfile.peek() == std::fstream::traits_type::eof()) {
		health = 3;
		coins = 0;
		stagesBeat[0] = 'n';

		std::cout << "1.Start new game." << std::endl;
		std::cout << "2.Exit." << std::endl;
		std::cout << "Input:";
		char res;
		std::cin >> res;
		while (true) {
			if (res == '1') {
				goToNextLevel();
				break;
			}
			else if (res == '2') {
				exit(EXIT_SUCCESS);
			}
			std::cout << "Invalid command!" << std::endl;
			std::cout << "Input:";
			std::cin >> res;
		}
		
	}
	else {
		health = currentPlayerProfile.get() - '0';
		currentPlayerProfile.seekg(2, std::fstream::cur);
		std::string temp;
		getline(currentPlayerProfile, temp);
		coins = std::stoi(temp);
		currentPlayerProfile.getline(stagesBeat, 6);
		
		std::cout << "1.Continue game." << std::endl;
		std::cout << "2.Replay level(current progress will be lost)." << std::endl;
		std::cout << "Input:";
		char res;
		std::cin >> res;

		while (true) {
			if (res == '1') {
				if (currentPlayerProfile.peek() == std::fstream::traits_type::eof()) {
					goToNextLevel();
				}
				else {
					currentLevelName.insert(5, 1, currentPlayerProfile.get());
					loadMatrixFromPlayerFile();
				}
				break;
			}
			else if (res == '2') {
				playCompletedLevel();
				break;
			}
			std::cout << "Invalid command!" << std::endl;
			std::cout << "Input:";
			std::cin >> res;
		}
	}
}

void winGame() {
	system("cls");
	std::cout << "Congratulations! You have beaten the game!" << std::endl;
	std::cout << "Your score is " << health + coins << "!" << std::endl;
	std::cout << "1.Lose all progress and play again." << std::endl;
	std::cout << "2.Save and quit." << std::endl;
	std::cout << "3.Replay completed level." << std::endl;
	std::cout << "Input:";
	char res;
	std::cin >> res;
	while (true) {
		if (res == '1') {
			clearBeatenStages();
			goToNextLevel();
			break;
		}
		else if (res == '2') {
			saveAndQuit(true);
			break;
		}
		else if (res == '3') {
			playCompletedLevel();
			break;
		}
		std::cout << "Invalid command!" << std::endl;
		std::cout << "Input:";
		std::cin >> res;
	}
}

void winLevel() {
	char wonLevel = currentLevelName[5];
	for (int i = 0; i < ALL_STAGES; i++)
	{
		if (stagesBeat[i] == 'n' || stagesBeat[i] == '\0') {
			stagesBeat[i] = wonLevel;
			break;
		}
	}
	if (countBeatenStages() >= 3) {
		winGame();
	}
	else {
		system("cls");
		std::cout << "Congratulations! You have beaten the level!" << std::endl;
		std::cout << "1.Go to next level." << std::endl;
		std::cout << "2.Save and quit." << std::endl;
		std::cout << "Input:";
		char res;
		std::cin >> res;
		while (true) {
			if (res == '1') {
				goToNextLevel();
				break;
			}
			else if(res == '2') {
				saveAndQuit(true);
			}
			std::cout << "Invalid command!" << std::endl;
			std::cout << "Input:";
			std::cin >> res;
		}
	}

}

void displayHealth(int health) {
	std::cout << "Health:";
	for (int i = 0; i < health; i++)
	{
		std::cout << "<3";
	}
	std::cout << std::endl;
}

void renderMatrix(bool hasKey) {
	system("cls");
	std::cout << "Level:" << mapToLevel(currentLevelName[5]) << std::endl;
	displayHealth(health);
	std::cout << "Coins:" << coins << std::endl;
	std::cout << "Key: " << (hasKey ? "Found" : "Not found") << std::endl;
	for (int i = 0; i < MAX_HEIGHT; i++)
	{
		bool finished = false;
		for (int a = 0; a < MAX_WIDTH; a++)
		{
			std::cout << currentLevelMatrix[i][a];
		}
		std::cout << std::endl;
	}
}

void goToNextPortal(int& row, int& col) {
	int nextPortalRow = 0, nextPortalCol = 0;
	for (int i = 0; i < MAX_PORTAL_COORDINATES; i+=2)
	{
		if (portalCoordinates[i] == row) {
			if (i + 2 >= MAX_PORTAL_COORDINATES) {
				nextPortalRow = portalCoordinates[0];
				nextPortalCol = portalCoordinates[1];
			}
			else {
				nextPortalRow = portalCoordinates[i + 2];
				nextPortalCol = portalCoordinates[i + 3];
			}
		}
	}
	if (currentLevelMatrix[nextPortalRow][nextPortalCol-1] == ' ') {
		row = nextPortalRow;
		col = nextPortalCol - 1;
	}
	else if (currentLevelMatrix[nextPortalRow-1][nextPortalCol ] == ' ') {
		row = nextPortalRow - 1;
		col = nextPortalCol;
	}
	else if(currentLevelMatrix[nextPortalRow][nextPortalCol + 1] == ' '){
		row = nextPortalRow;
		col = nextPortalCol + 1;
	}
	else {
		row = nextPortalRow + 1;
		col = nextPortalCol;
	}
	currentLevelMatrix[row][col] = '@';
}

void validatePlayerMoves(int &row, int &col, bool& hasKey, char move) {
	char toGo;
	switch (move) {
	case 'W':
	case 'w':
			toGo = currentLevelMatrix[row - 1][col];
			switch (toGo) {
			case '#':
				health--;
				break;
			case '%':
				currentLevelMatrix[row][col] = ' ';
				goToNextPortal(--row, col);
				break;
			case '&':
				hasKey = true;
				coins--;
			case 'C':
				coins++;
			case ' ':
				currentLevelMatrix[row][col] = ' ';
				currentLevelMatrix[--row][col] = '@';
				break;
			case 'X':
				if (hasKey) {
					hasKey = false;
					winLevel();
				}
				else {
					std::cout << "You need a key to unlock the chest!" << std::endl;
				}
				break;
			}
		break;
	case 'D':
	case 'd':
		toGo = currentLevelMatrix[row][col + 1];
		switch (toGo) {
		case '#':
			health--;
			break;
		case '%':
			currentLevelMatrix[row][col] = ' ';
			goToNextPortal(row, ++col);
			break;
		case '&':
			hasKey = true;
			coins--;
		case 'C':
			coins++;
		case ' ':
			currentLevelMatrix[row][col] = ' ';
			currentLevelMatrix[row][++col] = '@';
			break;
		case 'X':
			if (hasKey) {
				hasKey = false;
				winLevel();
			}
			else {
				std::cout << "You need a key to unlock the chest!" << std::endl;
			}
			break;
		}
		break;
	case 'A':
	case 'a':
		toGo = currentLevelMatrix[row][col-1];
		switch (toGo) {
		case '#':
			health--;
			break;
		case '%':
			currentLevelMatrix[row][col] = ' ';
			goToNextPortal(row, --col);
			break;
		case '&':
			hasKey = true;
			coins--;
		case 'C':
			coins++;
		case ' ':
			currentLevelMatrix[row][col] = ' ';
			currentLevelMatrix[row][--col] = '@';
			break;
		case 'X':
				if (hasKey) {
					hasKey = false;
					winLevel();
				}
				else {
					std::cout << "You need a key to unlock the chest!" << std::endl;
				}
			break;
		}
		break;
	case 'S':
	case 's':
		toGo = currentLevelMatrix[row + 1][col];
		switch (toGo) {
		case '#':
			health--;
			break;
		case '%':
			currentLevelMatrix[row][col] = ' ';
			goToNextPortal(++row,col);
			break;
		case '&':
			hasKey = true;
			coins--;
		case 'C':
			coins++;
		case ' ':
			currentLevelMatrix[row][col] = ' ';
			currentLevelMatrix[++row][col] = '@';
			break;
		case 'X':
			if (hasKey) {
				hasKey = false;
				winLevel();
			}
			else {
				std::cout << "You need a key to unlock the chest!" << std::endl;
			}
			break;
		}
		break;
	case 'E':
	case 'e':
		saveAndQuit(false);
		break;
	}
}

void startGame() {
	bool hasKey = false;
	while (true) {
		renderMatrix(hasKey);
		std::cout << "W/w(up) D/d(right) A/a(left) S/s(down) E/e(exit)" << std::endl;
		std::cout << "Input:";
		char input;
		std::cin >> input;
		validatePlayerMoves(playerRow, playerCol, hasKey, input);
	}
}

int main()
{
    identify();
	loadSaveData();
	startGame();
}


