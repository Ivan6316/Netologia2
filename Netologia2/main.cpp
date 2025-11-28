#include <iostream>
#include <vector>
#include <thread>
#include <future>
#include <algorithm>
#include <iterator>
#include <chrono>

template<typename Iterator, typename Func>
void parallel_for_each(Iterator first, Iterator last, Func func, size_t num_threads = std::thread::hardware_concurrency()) {
    auto length{ std::distance(first, last) };

    if (length == 0)
    {
        return;
    }

    // Если элементов мало или потоков слишком много, используем последовательную версию
    if (length < 1000 || num_threads <= 1)
    {
        std::for_each(first, last, func);
        return;
    }

    auto block_size = length / num_threads;

    std::vector<std::future<void>> futures;
    futures.reserve(num_threads - 1);

    Iterator block_start{ first };

    // Запускаем задачи для всех блоков, кроме последнего
    for (size_t i = 0; i < num_threads - 1; ++i)
    {
        Iterator block_end{ block_start };
        std::advance(block_end, block_size);

        // Запускаем асинхронную задачу для текущего блока
        futures.emplace_back(std::async(std::launch::async,
            [block_start, block_end, func]()
            {
                std::for_each(block_start, block_end, func);
            }
        ));

        block_start = block_end;
    }

    // Обрабатываем последний блок в текущем потоке
    std::for_each(block_start, last, func);

    // Ждем завершения всех асинхронных задач
    for (auto& future : futures)
    {
        future.get();
    }
}

// Рекурсивная версия (альтернативная реализация)
template<typename Iterator, typename Func>
void parallel_for_each_recursive(Iterator first, Iterator last, Func func, size_t depth = 0)
{
    auto length = std::distance(first, last);

    if (length < 1000 || depth > 3)
    {
        std::for_each(first, last, func);
        return;
    }

    // Разделяем диапазон пополам
    Iterator middle = first;
    std::advance(middle, length / 2);

    // Запускаем задачу для левой половины
    auto left_future = std::async(std::launch::async,
        [first, middle, func, depth]()
        {
            parallel_for_each_recursive(first, middle, func, depth + 1);
        }
    );

    // Обрабатываем правую половину рекурсивно
    parallel_for_each_recursive(middle, last, func, depth + 1);

    // Ждем завершения левой половины
    left_future.get();
}

int main()
{
    // Подключение Русского языка
    setlocale(LC_ALL, "rus");

    std::vector<int> data(10000);

    // Заполняем вектор
    for (size_t i = 0; i < data.size(); ++i)
    {
        data[i] = static_cast<int>(i);
    }

    // Функция для обработки элементов
    auto process_element = [](int& value)
        {
        // Удваиваем каждый элемент
        value *= 2; 
        };

    std::cout << "Используем параллельный for_each..." << std::endl;

    // Измеряем время выполнения
    auto start = std::chrono::high_resolution_clock::now();

    parallel_for_each(data.begin(), data.end(), process_element);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "Параллельная версия выполнена за " << duration.count() << " микросекунд" << std::endl;

    // Проверяем результат на первых 10 элементах
    std::cout << "Первые 10 элементов после обработки: ";
    for (size_t i = 0; i < 10 && i < data.size(); ++i)
    {
        std::cout << data[i] << " ";
    }
    std::cout << std::endl;

    // Сравниваем с последовательной версией
    std::vector<int> data2(10000);
    for (size_t i = 0; i < data2.size(); ++i)
    {
        data2[i] = static_cast<int>(i);
    }

    start = std::chrono::high_resolution_clock::now();
    std::for_each(data2.begin(), data2.end(), process_element);
    end = std::chrono::high_resolution_clock::now();
    auto duration_sequential = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "Последовательная версия выполнена за " << duration_sequential.count() << " микросекунд" << std::endl;

    
    return 0;
}