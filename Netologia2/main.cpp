#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <mutex>

// Базовый класс наблюдателя
class Observer
{
public:
    virtual void onWarning(const std::string& message) {}
    virtual void onError(const std::string& message) {}
    virtual void onFatalError(const std::string& message) {}
    virtual ~Observer() = default;
};

// Наблюдаемый класс
class Observable
{
private:
    std::vector<Observer*> observers;
    mutable std::mutex mutex;

public:
    // Добавление наблюдателя
    void addObserver(Observer* observer)
    {
        std::lock_guard<std::mutex> lock(mutex);
        if (observer &&
            std::find(observers.begin(), observers.end(), observer) == observers.end())
        {
            observers.push_back(observer);
        }
    }

    // Удаление наблюдателя
    void removeObserver(Observer* observer)
    {
        std::lock_guard<std::mutex> lock(mutex);
        auto it = std::find(observers.begin(), observers.end(), observer);
        if (it != observers.end())
        {
            observers.erase(it);
        }
    }

    // Оповещение наблюдателей о предупреждении
    void warning(const std::string& message) const
    {
        std::vector<Observer*> observers_copy;
        {
            std::lock_guard<std::mutex> lock(mutex);
            observers_copy = observers; // копируем для безопасного обхода
        }

        for (auto observer : observers_copy)
        {
            observer->onWarning(message);
        }
    }

    // Оповещение наблюдателей об ошибке
    void error(const std::string& message) const 
    {
        std::vector<Observer*> observers_copy;
        {
            std::lock_guard<std::mutex> lock(mutex);
            observers_copy = observers;
        }

        for (auto observer : observers_copy)
        {
            observer->onError(message);
        }
    }

    // Оповещение наблюдателей о фатальной ошибке
    void fatalError(const std::string& message) const
    {
        std::vector<Observer*> observers_copy;
        {
            std::lock_guard<std::mutex> lock(mutex);
            observers_copy = observers;
        }

        for (auto observer : observers_copy) 
        {
            observer->onFatalError(message);
        }
    }
};

// Класс для работы с предупреждениями
class WarningObserver : public Observer
{
public:
    void onWarning(const std::string& message) override
    {
        std::cout << "WARNING: " << message << std::endl;
    }
};

// Класс для работы с ошибками
class ErrorObserver : public Observer
{
private:
    std::string filePath;
    mutable std::mutex fileMutex;

public:
    explicit ErrorObserver(const std::string& path) : filePath(path) {}

    void onError(const std::string& message) override
    {
        std::lock_guard<std::mutex> lock(fileMutex);
        std::ofstream logFile(filePath, std::ios::app);
        if (logFile.is_open())
        {
            logFile << "ERROR: " << message << std::endl;
        }
        else
        {
            std::cerr << "Cannot write to error log file: " << filePath << std::endl;
        }
    }
};

// Класс для работы с фатальными ошибками
class FatalErrorObserver : public Observer
{
private:
    std::string filePath;
    mutable std::mutex fileMutex;

public:
    explicit FatalErrorObserver(const std::string& path) : filePath(path) {}

    void onFatalError(const std::string& message) override
    {
        // Печать в консоль
        std::cerr << "FATAL ERROR: " << message << std::endl;

        // Печать в файл
        {
            std::lock_guard<std::mutex> lock(fileMutex);
            std::ofstream logFile(filePath, std::ios::app);
            if (logFile.is_open())
            {
                logFile << "FATAL ERROR: " << message << std::endl;
            }
        }
    }
};


int main() {
    Observable observable;

    WarningObserver warningObserver;
    ErrorObserver errorObserver("input.txt");
    FatalErrorObserver fatalErrorObserver("input.txt");

    observable.addObserver(&warningObserver);
    observable.addObserver(&errorObserver);
    observable.addObserver(&fatalErrorObserver);

    observable.warning("Test warning");
    observable.error("Test error");
    observable.fatalError("Test fatal error");
}