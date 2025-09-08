#include <iostream>

class list {
public:
	int x, y, z;
	int getDistance() {
		return (x * x) + (y * y) + (z * z);
	}
private:
};

int main() {
	bool run = true;
	bool command_f = false;
	list list[10];
	class list list_f[10];
	int top = 0;
	int bottom = 0;
	while (run) {
		{
			if (command_f) {
				for (int i = 0; i < top - bottom; i++) {
					if (list_f[i].x != 0 || list_f[i].y != 0 || list_f[i].z != 0) {
						std::cout << i << " : " << list_f[i].x << " " << list_f[i].y << " " << list_f[i].z << std::endl;
					}
				}
			}
			else {
				for (int i = 0; i < top; i++) {
					if (list[i].x != 0 || list[i].y != 0 || list[i].z != 0) {
						std::cout << i << " : " << list[i].x << " " << list[i].y << " " << list[i].z << std::endl;
					}
				}
			}
		}
		char command;
		std::cin >> command;
		switch (command)
		{
		case'+':
		{
			int x, y, z;
			std::cout << "input x y z : ";
			std::cin >> x >> y >> z;
			list[top].x = x;
			list[top].y = y;
			list[top].z = z;
			top++;
			break;
		}
		case'-':
		{
			if (top == bottom)
			{
				std::cout << "삭제할 점이 없습니다." << std::endl;
				break;
			}
			else {
				list[top - 1].x = 0;
				list[top - 1].y = 0;
				list[top - 1].z = 0;
				top--;
				break;
			}
		}
		case'e':
		{
			if (bottom != 0) {
				bottom--;
				int x, y, z;
				std::cout << "input x y z : ";
				std::cin >> x >> y >> z;
				list[bottom].x = x;
				list[bottom].y = y;
				list[bottom].z = z;
			}
			else {
				std::cout << "아래에 공간이 없습니다." << std::endl;
			}
			break;
		}
		case'd':
		{
			if (top == bottom)
			{
				std::cout << "삭제할 점이 없습니다." << std::endl;
				break;
			}
			else {
				list[bottom].x = 0;
				list[bottom].y = 0;
				list[bottom].z = 0;
				bottom++;
				break;
			}
		}
		case'a':
		{
			std::cout << "등록된 점의 갯수 : " << top - bottom << "개" << std::endl;
			break;
		}
		case'b':
		{
			for (int i = bottom; i < top; i++)
			{
				list[i].x -= 1;
				list[i].y -= 1;
				list[i].z -= 1;
				if (list[i].x < 0) list[i].x = 9;
				if (list[i].y < 0) list[i].y = 9;
				if (list[i].z < 0) list[i].z = 9;
			}
			break;
		}
		case'c':
		{
			top = 0;
			bottom = 0;
			for (int i = 0; i < 10; i++)
			{
				list[i].x = 0;
				list[i].y = 0;
				list[i].z = 0;
			}
			break;
		}
		case'f':
		{
			if (!command_f) {
				for (int i = 0; i < top; i++) {
					list_f[i] = list[i];
				}
				for (int i = 0; i < top; i++) {
					for (int j = i; j < top; j++) {
						if (list_f[i].getDistance() < list_f[j].getDistance()) {
							class list temp = list_f[i];
							list_f[i] = list_f[j];
							list_f[j] = temp;
						}
					}
				}
			}
			command_f = !command_f;
			break;
		}
		case'q':
		{
			run = !run;
			break;
		}
		default:
			break;
		}
	}
}