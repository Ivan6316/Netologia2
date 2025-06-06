#include <iostream>
#include <algorithm>
#include "counter.h"

int main()
{
	//����������� �������� �����
	setlocale(LC_ALL, "rus");

	myMath::Counter data;
	setlocale(LC_ALL, "rus");
	std::string answer{};
	int value{};
	char team{};

	std::cout << "�� ������ ������� ��������� �������� ��������? ������� Yes ��� No: ";
	std::cin >> answer;

	//�������� � ������� ��������
	std::transform(answer.begin(), answer.end(), answer.begin(), [](unsigned char c) { return std::tolower(c); });

	//�������� �� ���������� �����
	while (true)
	{
		if (answer == "yes")
		{
			std::cout << "������� ��������� �������� ��������: ";
			//�������� �� ���������� ����
			while (!(std::cin >> value))
			{
				//���������� ������������ �������� cin
				std::cin.clear();
				std::cin.ignore();

				std::cout << "��� �������� �� ����������. ������� �����: ";
			}
			data = myMath::Counter(value);
			break;
		}

		else if (answer == "no") { std::cout << "����������� �������� �� ��������� (1)" << std::endl; break; }

		else
		{
			//���������� ������������ �������� cin
			std::cin.clear();
			std::cin.ignore();
			std::cout << "�� ����� �� ���������� �����. ���������� ��� ���: ";
			std::cin >> answer;
		}
	}

	while (true)
	{
		std::cout << "������� ������� ('+', '-', '=' ��� 'x'): ";
		std::cin >> team;

		if (team == '+') data.add();
		else if (team == '-') data.deduct();
		else if (team == '=') data.result();
		else if (team == 'x') {
			std::cout << "�� ��������!\n";
			break;
		}
		else {
			std::cout << "������������ �������. ��������� ����.\n";
		}
	}

	return EXIT_SUCCESS;
}