#include <iostream>
#include <string>
#include <windows.h>

#include "CMyList.h"

namespace
{
    template <typename T>
    void PrintForward(const CMyList<T>& list, const std::string& label)
    {
        std::cout << label << " (size=" << list.GetSize() << "): [ ";
        for (const auto& item : list)
        {
            std::cout << item << " ";
        }
        std::cout << "]\n";
    }

    template <typename T>
    void PrintReverse(const CMyList<T>& list, const std::string& label)
    {
        std::cout << label << " (reverse): [ ";
        for (auto it = list.rbegin(); it != list.rend(); ++it)
        {
            std::cout << *it << " ";
        }
        std::cout << "]\n";
    }

    void DemonstrateStringList()
    {
        std::cout << "\nСписок строк:\n";

        CMyList<std::string> words;
        words.PushBack("мир");
        words.PushFront("Привет,");
        words.PushBack("!");
        PrintForward(words, "После PushBack/PushFront");

        auto it = words.begin();
        ++it;
        words.Insert(it, "прекрасный");
        PrintForward(words, "После Insert перед \"мир\"");

        PrintReverse(words, "Тот же список");

        std::cout << "\nКопирование:\n";
        CMyList<std::string> copy = words;
        copy.PushBack("(копия)");
        PrintForward(words, "Оригинал");
        PrintForward(copy, "Копия");

        std::cout << "\nПеремещение:\n";
        CMyList<std::string> moved = std::move(copy);
        PrintForward(moved, "После перемещения");
        std::cout << "Источник после перемещения (size=" << copy.GetSize() << ")\n";

        std::cout << "\nУдаление первого и последнего:\n";
        moved.Erase(moved.begin());
        auto last = moved.end();
        --last;
        moved.Erase(last);
        PrintForward(moved, "После удаления крайних");
    }

    void DemonstrateIntList()
    {
        std::cout << "\n\nСписок целых чисел:\n\n";

        CMyList<int> numbers;
        for (int i = 1; i <= 6; ++i)
        {
            numbers.PushBack(i);
        }
        PrintForward(numbers, "Начальный");

        auto it = numbers.begin();
        std::advance(it, 2);
        numbers.Erase(it);
        PrintForward(numbers, "Удалён элемент \"3\"");

        it = numbers.begin();
        std::advance(it, 2);
        numbers.Insert(it, 99);
        PrintForward(numbers, "Вставлен 99 перед \"4\"");

        std::cout << "\nОператор присваивания:\n";
        CMyList<int> other;
        other.PushBack(10);
        other.PushBack(20);
        PrintForward(other, "other до присваивания");
        other = numbers;
        PrintForward(other, "other после \"other = numbers\"");

        std::cout << "\nПеремещающее присваивание:\n";
        CMyList<int> target = std::move(other);
        PrintForward(target, "target после перемещения");
        std::cout << "other после перемещения (size=" << other.GetSize() << ")\n";
    }
}

int main()
{
    SetConsoleOutputCP(65001);
    DemonstrateStringList();
    DemonstrateIntList();
    return 0;
}
