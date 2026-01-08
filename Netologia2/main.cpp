#include <fstream>
#include <memory>
#include <string>

// Разделение интерфейса на несколько специализированных классов
class HTMLPrintable
{
public:
    virtual ~HTMLPrintable() = default;
    virtual std::string printAsHTML() const = 0;
};

class TextPrintable
{
public:
    virtual ~TextPrintable() = default;
    virtual std::string printAsText() const = 0;
};

class JSONPrintable
{
public:
    virtual ~JSONPrintable() = default;
    virtual std::string printAsJSON() const = 0;
};

// Отвязка Data от конкретного типа данных
class Data
{
private:
    std::string data_;

public:
    explicit Data(std::string data) : data_(std::move(data)) {}
    virtual ~Data() = default;

    const std::string& getData() const { return data_; }
};

// Специализированные классы для каждого формата
class TextData : public TextPrintable, public Data
{
public:
    explicit TextData(const std::string& data) : Data(data) {}

    std::string printAsText() const override
    {
        return getData(); // Возвращаем значение
    }
};

class HTMLData : public HTMLPrintable, public Data
{
public:
    explicit HTMLData(const std::string& data) : Data(data) {}

    std::string printAsHTML() const override
    {
        return "<html>" + getData() + "</html>";
    }
};

class JSONData : public JSONPrintable, public Data
{
public:
    explicit JSONData(const std::string& data) : Data(data) {}

    std::string printAsJSON() const override
    {
        return "{ \"data\": \"" + getData() + "\" }";
    }
};

// Фабрика для создания данных в нужном формате
class DataFactory {
public:
    static std::unique_ptr<Data> createFromString(const std::string& data,
        const std::string& format) {
        if (format == "text") {
            return std::unique_ptr<Data>(new TextData(data));
        }
        else if (format == "html") {
            return std::unique_ptr<Data>(new HTMLData(data));
        }
        else if (format == "json") {
            return std::unique_ptr<Data>(new JSONData(data));
        }
        return nullptr;
    }
};

// Функции сохранения, которые работают с конкретными интерфейсами
void saveToAsHTML(std::ofstream& file, const HTMLPrintable& printable)
{
    file << printable.printAsHTML();
}

void saveToAsJSON(std::ofstream& file, const JSONPrintable& printable)
{
    file << printable.printAsJSON();
}

void saveToAsText(std::ofstream& file, const TextPrintable& printable)
{
    file << printable.printAsText();
}

// Универсальная функция сохранения через шаблоны
template<typename PrintableType>
void saveTo(std::ofstream& file, const PrintableType& printable)
{
    if constexpr (std::is_base_of_v<HTMLPrintable, PrintableType>)
    {
        saveToAsHTML(file, printable);
    }
    else if constexpr (std::is_base_of_v<TextPrintable, PrintableType>)
    {
        saveToAsText(file, printable);
    }
    else if constexpr (std::is_base_of_v<JSONPrintable, PrintableType>)
    {
        saveToAsJSON(file, printable);
    }
}

// Функция с динамическим кастом для использования через Data*
void saveToDynamic(std::ofstream& file, const Data* data, const std::string& format)
{
    if (!data) return;

    if (format == "html") {
        if (auto* htmlData = dynamic_cast<const HTMLPrintable*>(data)) {
            saveToAsHTML(file, *htmlData);
        }
    }
    else if (format == "text") {
        if (auto* textData = dynamic_cast<const TextPrintable*>(data)) {
            saveToAsText(file, *textData);
        }
    }
    else if (format == "json") {
        if (auto* jsonData = dynamic_cast<const JSONPrintable*>(data)) {
            saveToAsJSON(file, *jsonData);
        }
    }
}

int main()
{
    return EXIT_SUCCESS;
}