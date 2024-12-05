//Домбровский А, 9 группа, 2 курс
// 2 Лаба, 20 номер
//Создать класс Binary для работы с беззнаковыми целыми двоичными числами,
//используя для представления числа массив из 100 элементов типа unsigned char,
//каждый элемент которого является двоичной цифрой.Младшая цифра имеет
//младший индекс(единицы – в нулевом элементе массива).Реальный размер
//задается как аргумент конструктора инициализации.Реализовать арифметические
//операции, аналогичные встроенным для целых в C++, и операции сравнения,
//приведение к десятичной системе счисления и из нее.
#include <iostream>;
#include <Windows.h>;
#include <fstream>
#include <limits>

import Binary;
void print_menu() {
    std::cout << "\n1. Ввести двоичное число A с клавиатуры\n"
        << "2. Ввести десятичное число A с клавиатуры\n"
        << "3. Ввести двоичное число A из файла\n"
        << "4. Вывести двоичное число A на экран\n"
        << "5. Вывести десятичное число A на экран\n"
        << "6. Сохранить двоичное число A в файл\n"
        << "7. Выполнить операцию  A = A + B\n"
        << "8. Выполнить операцию A = A - B\n"
        << "9. Выполнить операцию A = A * B\n"
        << "10. Выполнить операцию A = A / B\n"
        << "11. Выполнить операцию A == B\n"
        << "12. Выполнить операцию A <=> B\n"
        << "13. Выполнить операцию A != B\n"
        << "14. Выполнить операцию A < B\n"
        << "15. Выполнить операцию A > B\n"
        << "16. Выполнить операцию A <= B\n"
        << "17. Выполнить операцию A >= B\n"
        << "18. Выход\n"
        << "Выберите действие: ";
}
unsigned int input_size() {
    unsigned int size;
    while (true) {
        std::cout << "Введите размер -> ";
        if (!(std::cin >> size)) {
            std::cout << "Ошибка: введите целое число." << std::endl;
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n');
            continue;
        }
        break;
    }
    return size;
}
Binary input_binary() {
    unsigned int size = input_size();
    std::string binary_x;

    std::cout << "Введите двоичное число -> ";
    std::cin >> binary_x;
    
    Binary binary(size);
    binary.from_binary_string(binary_x);
    return binary;
}
Binary input_decimal() {
    unsigned int size = input_size();
    int x;

    while (true) {
        std::cout << "Введите десятичное число -> ";
        if (!(std::cin >> x)) {
            std::cout << "Ошибка: введите целое число." << std::endl;
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n');
            continue;
        }
        break;
    }

    Binary binary(size);
    binary.from_decimal(x);
    return binary;
}
int main() {
    setlocale(LC_ALL, "Russian");
    Binary binary;
    int decimal_x, size;
    std::string binary_x, filename;
    bool exit = false;
    int choice;
    std::ifstream in_file;
    std::ofstream out_file;
    while (!exit) {
        try {
            print_menu();
            std::cin >> choice;

            switch (choice) {
            case 1:
                binary = input_binary();
                break;
            case 2:
                binary = input_decimal();
                break;
            case 3:
                std::cout << "Введите название файлы -> ";
                std::cin >> filename;
                in_file.open(filename);
                if (in_file.is_open())
                {
                    in_file >> binary_x;
                    in_file.close();
                    binary = Binary(binary_x.length());
                    binary.from_binary_string(binary_x);
                }
                else 
                {
                    std::cout << "Не удалось открыть файл." << std::endl;
                }
                break;
            case 4:
                std::cout << "Текущее число (Двоичный вид): " << binary.to_binary_string() << std::endl;
                break;
            case 5:
                std::cout << "Текущее число (Десятичный вид): " << binary.to_decimal() << std::endl;
                break;
            case 6:
                std::cout << "Введите название файла -> ";
                std::cin >> filename;
                out_file.open(filename);
                if (out_file.is_open()) 
                {
                    out_file << binary.to_binary_string() << std::endl;
                    out_file.close();
                }
                else 
                {
                    std::cout << "Не удалось открыть файл." << std::endl;
                }
                break;
            case 7:
                binary = binary + input_binary();
                break;
            case 8:
                binary = binary - input_binary();
                break;
            case 9:
                binary = binary * input_binary();
                break;
            case 10:
                binary = binary / input_binary();
                break;
            case 11:
                std::cout << (binary == input_binary() ? "True" : "False") << std::endl;
                break;
            case 12:
                std::cout << std::endl;
                std::strong_ordering bin = binary <=> input_binary();
                if (bin == std::strong_ordering::equal)
                    std::cout << "==" << std::endl;
                else if (bin == std::strong_ordering::greater)
                    std::cout << ">" << std::endl;
                else if (bin == std::strong_ordering::less)
                    std::cout << "<" << std::endl;
                break;
            case 13:
                std::cout << (binary != input_binary() ? "True" : "False") << std::endl;
                break;
            case 14:
                std::cout << (binary < input_binary() ? "True" : "False") << std::endl;
                break;
            case 15:
                std::cout << (binary > input_binary() ? "True" : "False") << std::endl;
                break;
            case 16:
                std::cout << (binary <= input_binary() ? "True" : "False") << std::endl;
                break;
            case 17:
                std::cout << (binary >= input_binary() ? "True" : "False") << std::endl;
                break;
            case 18:
                exit = true;
                break;
            default:
                std::cout << "Некорректный выбор, попробуйте снова.\n";
            }
        }
        catch (std::exception e) {
            std::cout << "Ошибка: " << e.what() << std::endl;
        }
    }

    return 0;
}
