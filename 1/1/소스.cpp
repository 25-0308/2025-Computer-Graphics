#include <iostream>
#include <random>
#include <algorithm>

std::mt19937 mtRand(static_cast<unsigned>(time(nullptr)));
std::uniform_int_distribution<int> number(1, 9);

int det3x3(int m[3][3]) {
	return m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) -
		m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
		m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
}
int det(int (*arr)[4]) {
	int sum = 0;
	int minor_Matrix[4][3][3];

	for (int k = 0; k < 4; k++) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 4; j++) {
				if (j < k)
					minor_Matrix[k][i][j] = arr[i + 1][j];
				else if (j > k)
					minor_Matrix[k][i][j - 1] = arr[i + 1][j];
			}
		}
	}
	
	for (int k = 0; k < 4; k++) {
		sum += (k % 2 == 0 ? 1 : -1) * arr[0][k] * det3x3(minor_Matrix[k]);
	}

	return sum;
}

int main() {
	bool c_e = false;
	bool c_f = false;
	int array[4][4] = {};
	int array2[4][4] = {};
	int command_e[4][4] = {};
	int command_e2[4][4] = {};
	int command_f[4][4] = {};
	int command_f2[4][4] = {};
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
			if (c_e) {
				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 4; j++) {
						std::cout << command_e[i][j] << " ";
					}
					std::cout << std::endl;
				}
				std::cout << std::endl;
				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 4; j++) {
						std::cout << command_e2[i][j] << " ";
					}
					std::cout << std::endl;
				}
				std::cout << std::endl;
				std::cin >> com;
			}
			else if (c_f) {
				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 4; j++) {
						std::cout << command_f[i][j] << " ";
					}
					std::cout << std::endl;
				}
				std::cout << std::endl;
				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 4; j++) {
						std::cout << command_f2[i][j] << " ";
					}
					std::cout << std::endl;
				}
				std::cout << std::endl;
				std::cin >> com;
			}
			else {
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
			r1 = det(array);
			r2 = det(array2);
			std::cout << r1 << " " << r2 << std::endl;
			break;
		}
		case 't':
		{
			int mat[16] = {};
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					mat[j + i * 4] = array[i][j];
				}
			}
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					array[j][i] = mat[i*4 + j];

			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					mat[j + i * 4] = array2[i][j];
				}
			}
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					array2[j][i] = mat[i * 4 + j];
			break;
		}
		case 'e':
		{
			if (!c_f) {
				int min_1 = 10, min_2 = 10;
				int mat[16] = {};
				int mat2[16] = {};
				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 4; j++) {
						mat[j + i * 4] = array[i][j];
					}
				}
				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 4; j++) {
						mat2[j + i * 4] = array2[i][j];
					}
				}
				for (int i = 0; i < 16; i++) {
					if (mat[i] < min_1) {
						min_1 = mat[i];
					}
					if (mat2[i] < min_2) {
						min_2 = mat2[i];
					}
				}
				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 4; j++) {
						command_e[i][j] = array[i][j] - min_1;
						command_e2[i][j] = array2[i][j] - min_2;
					}
				}
				c_e = !c_e;
			}
			break;
		}
		case 'f':
		{
			if (!c_e) {
				int max_1 = 0, max_2 = 0;
				int mat[16] = {};
				int mat2[16] = {};
				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 4; j++) {
						mat[j + i * 4] = array[i][j];
					}
				}
				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 4; j++) {
						mat2[j + i * 4] = array2[i][j];
					}
				}
				for (int i = 0; i < 16; i++) {
					if (mat[i] > max_1) {
						max_1 = mat[i];
					}
					if (mat2[i] > max_2) {
						max_2 = mat2[i];
					}
				}
				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 4; j++) {
						command_f[i][j] = array[i][j] + max_1;
						command_f2[i][j] = array2[i][j] + max_2;
					}
				}
				c_f = !c_f;
				break;
			}
		}
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
		{
			wh = !wh;
			break;
		}
		default:
			break;
		}
	}
}