#include <iostream>
#include <algorithm>
#include "Math.h"

int main()
{
	//подключение Русского языка
	setlocale(LC_ALL, "rus");

	myMath::Counter data;
	setlocale(LC_ALL, "rus");
	std::string answer{};
	int value{};
	char team{};

	std::cout << "Вы хотите указать начальное значение счётчика? Введите Yes или No: ";
	std::cin >> answer;

	//приводим к нижнему регистру
	std::transform(answer.begin(), answer.end(), answer.begin(), [](unsigned char c) { return std::tolower(c); });

	//проверка на корректный ответ
	while (true)
	{
		if (answer == "yes")
		{
			std::cout << "Введите начальное значение счётчика: ";
			//проверка на корректный ввод
			while (!(std::cin >> value))
			{
				//сбрасываем неправильное значение cin
				std::cin.clear();
				std::cin.ignore();

				std::cout << "Это значение не корректное. Введите новое: ";
			}
			data = myMath::Counter(value);
			break;
		}

		else if (answer == "no") { std::cout << "Установлено значение по умолчанию (1)" << std::endl; break; }

		else
		{
			//сбрасываем неправильное значение cin
			std::cin.clear();
			std::cin.ignore();
			std::cout << "Вы ввели не корректный ответ. Попробуйте ещё раз: ";
			std::cin >> answer;
		}
	}

	while (true)
	{
		std::cout << "Введите команду ('+', '-', '=' или 'x'): ";
		std::cin >> team;

		if (team == '+') data.add();
		else if (team == '-') data.deduct();
		else if (team == '=') data.result();
		else if (team == 'x') {
			std::cout << "До свидания!\n";
			break;
		}
		else {
			std::cout << "Некорректная команда. Повторите ввод.\n";
		}
	}

	return EXIT_SUCCESS;
}