#include <iostream>
#include <random>
#include <algorithm>

std::mt19937 mtRand(static_cast<unsigned>(time(nullptr)));
std::uniform_int_distribution<int> number(1, 9);

int main() {
	int array[4][4] = {};
	int array2[4][4] = {};
	int arrayr[4][4] = {};
	{
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				array[i][j] = number(mtRand);
			}
		}
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				array2[i][j] = number(mtRand);
			}
		}
	}
	char com = 0;
	bool wh = true;
	while (wh) {
		{
			for (int i = 0; i < 4; i++) {
				memset(arrayr[i], 0, sizeof(int) * 4);
			}
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					std::cout << array[i][j] << " ";
				}
				std::cout << std::endl;
			}
			std::cout << std::endl;
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					std::cout << array2[i][j] << " ";
				}
				std::cout << std::endl;
			}
			std::cout << std::endl;
			std::cin >> com;
		}
		switch (com)
		{
		case 'm':
		{
			for (int l = 0; l < 4; l++) {
				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 4; j++) {
						arrayr[l][i] += array[l][j] * array2[j][i];
					}
				}
			}
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					std::cout << arrayr[i][j] << " ";
				}
				std::cout << std::endl;
			}
			std::cout << std::endl;
			break;
		}
		case 'a':
		{
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					arrayr[i][j] += array[i][j] + array2[i][j];
				}
			}
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					std::cout << arrayr[i][j] << " ";
				}
				std::cout << std::endl;
			}
			std::cout << std::endl;
			break;
		}
		case 'd':
		{
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					arrayr[i][j] += array[i][j] - array2[i][j];
				}
			}
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					std::cout << arrayr[i][j] << " ";
				}
				std::cout << std::endl;
			}
			std::cout << std::endl;
			break;
		}
		case 'r':
		{
			int r1, r2;
			for (int l = 0; l < 4; l++) {
				for (int i = 0; i < 4; i++) {
					if (i == l)i++;
					for (int j = 0; j < 4; j++) {
						if (j == l)j++;
						array[i][j] =
					}
				}
			}
			std::cout << r1 << " " << r2 << std::endl;
			break;
		}
		case 't':
			break;
		case 'e':
			break;
		case 'f':
			break;
		case 's':
		{
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					array[i][j] = number(mtRand);
				}
			}
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					array2[i][j] = number(mtRand);
				}
			}
			break;
		}
		case 'q':
			wh = !wh;
			break;
		default:
			break;
		}
	}
}