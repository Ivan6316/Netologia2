#include <iostream>
#include <string>
#include <vector>
#include <map>

class SqlSelectQueryBuilder
{
private:
    std::vector<std::string> columns;
    std::string from_table;
    std::map<std::string, std::string> where_conditions;

public:
    SqlSelectQueryBuilder() = default;

    // Существующие методы
    SqlSelectQueryBuilder& AddColumn(const std::string& column)
    {
        columns.push_back(column);
        return *this;
    }

    SqlSelectQueryBuilder& AddFrom(const std::string& table)
    {
        from_table = table;
        return *this;
    }

    SqlSelectQueryBuilder& AddWhere(const std::string& column, const std::string& value) 
    {
        where_conditions[column] = value;
        return *this;
    }

    // Новые методы с noexcept
    SqlSelectQueryBuilder& AddWhere(const std::map<std::string, std::string>& kv) noexcept
    {
        // Объединяем с существующими условиями
        where_conditions.insert(kv.begin(), kv.end());
        return *this;
    }

    SqlSelectQueryBuilder& AddColumns(const std::vector<std::string>& cols) noexcept
    {
        // Добавляем новые колонки к существующим
        columns.insert(columns.end(), cols.begin(), cols.end());
        return *this;
    }

    std::string BuildQuery() const
    {
        std::string query = "SELECT ";

        // Обрабатываем колонки
        if (columns.empty())
        {
            query += "*";
        }
        else
        {
            for (size_t i = 0; i < columns.size(); ++i)
            {
                query += columns[i];
                if (i != columns.size() - 1)
                {
                    query += ", ";
                }
            }
        }

        // Проверяем, что таблица задана
        if (from_table.empty())
        {
            return "";
        }

        // Добавляем FROM
        query += " FROM " + from_table;

        // Добавляем WHERE если есть условия
        if (!where_conditions.empty())
        {
            query += " WHERE ";
            bool first = true;
            for (const auto& condition : where_conditions)
            {
                if (!first)
                {
                    query += " AND ";
                }
                query += condition.first + "=" + condition.second;
                first = false;
            }
        }

        query += ";";
        return query;
    }
};

int main()
{
    // Подключение Русского языка
    setlocale(LC_ALL, "rus");

    SqlSelectQueryBuilder query_builder;
    query_builder.AddColumn("name").AddColumn("phone");
    query_builder.AddFrom("students");
    query_builder.AddWhere("id", "42").AddWhere("name", "John");

    std::string result = query_builder.BuildQuery();
    std::cout << "Результат: " << result << std::endl;
    std::cout << "Ожидалось: SELECT name, phone FROM students WHERE id=42 AND name=John;" << std::endl;
    std::cout << "Совпадает: " << (result == "SELECT name, phone FROM students WHERE id=42 AND name=John;" ? "Да" : "Нет") << std::endl;

    return EXIT_SUCCESS;
}