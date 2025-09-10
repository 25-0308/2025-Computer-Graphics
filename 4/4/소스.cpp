#include <iostream>
#include <windows.h>
#include <random>

std::mt19937 mtRand(static_cast<unsigned>(time(nullptr)));
std::uniform_int_distribution<int> number(1, 24);

class card {
public:
	char shape;
	int num;
	char color;
	card() : shape(' '), num(0) {}
	card(char s, int n) : shape(s), num(n) {}
};

int main() {
	char board[6][6] = { };
	board[0][0] = ' '; 
	for (int i = 1; i < 6; i++) {
		board[0][i] = '0' + i;
		board[i][0] = 96 + i;
	for (int i = 1; i < 6; i++)
		for (int j = 1; j < 6; j++)
			board[i][j] = '*';
	}
	bool run = true;
	bool check = false;
	while (run) {
		{
			for (int i = 0; i < 6; i++) {
				for (int j = 0; j < 6; j++) {
					std::cout << board[i][j] << ' ';
				}
				std::cout << std::endl;
			}
		}
		char command;
		std::cin >> command;
		switch (command) {
		case 'a':
		{
			int num;
			std::cin >> num;
			if (num >= 1 && num <= 5) {
				 
			}
		}
			break;
		case 'b':
			break;
		case 'c':
			break;
		case 'd':
			break;
		case 'e':
			break;
		case 'r':
			
			break;
		case 'h':
		{
			Sleep(3000);
			system("cls");
			break;
		}
		case 'q':
		{
			run = false;
			break;
		}
		}
	}
}