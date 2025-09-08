#include <iostream>
#include <windows.h>

int main() {
	char board[6][6] = { };
	board[0] = { ' ', '1', '2', '3', '4', '5' };
	bool run = true;
	while (run) {
		char command;
		std::cin >> command;
		switch (command) {
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