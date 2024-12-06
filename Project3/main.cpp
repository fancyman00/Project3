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

static void print_menu() {
    std::cout << "\nМеню\n\n"
        << "1. Ввести двоичное число с клавиатуры\n"
        << "2. Ввести десятичное число с клавиатуры\n\n"
        << "3. Ввести двоичное число из файла\n"
        << "4. Сохранить двоичное число в файл\n\n"
        << "5. Вывести значение переменной на экран\n\n"
        << "6. Выполнить операцию  A + B\n"
        << "7. Выполнить операцию A - B\n"
        << "8. Выполнить операцию A * B\n"
        << "9. Выполнить операцию A / B\n\n"
        << "10. Выполнить операцию A == B\n"
        << "11. Выполнить операцию A <=> B\n"
        << "12. Выполнить операцию A != B\n"
        << "13. Выполнить операцию A < B\n"
        << "14. Выполнить операцию A > B\n"
        << "15. Выполнить операцию A <= B\n"
        << "16. Выполнить операцию A >= B\n\n"
        << "17. Выход\n\n";
}
static void print_sub_menu() {
    std::cout << "\nВыберите переменную\n"
        << "1. A\n"
        << "2. B\n";
}
static unsigned int input_uint(const char* message) {
    unsigned int size;
    while (true) {
        std::cout << message;
        if (!(std::cin >> size)) {
            std::cout << "Ошибка: введите беззнаковое целое число." << std::endl;
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n');
            continue;
        }
        break;
    }
    return size;
}

static unsigned int input_variable() {
    unsigned short choice = 0;
    print_sub_menu();
    while (choice != 1 && choice != 2) {
        choice = input_uint("Выберите переменную -> ");
    }
    return choice;
}

static Binary input_binary() {
    unsigned int size = input_uint("Введите размер -> ");
    std::string binary_x;

    std::cout << "Введите двоичное число -> ";
    std::cin >> binary_x;
    
    Binary binary(size);
    binary.from_binary_string(binary_x);
    return binary;
}

static Binary input_decimal() {
    unsigned int size = input_uint("Введите размер -> ");
    int x = input_uint("Введите беззнаковое десятичное число -> ");

    Binary binary(size);
    binary.from_decimal(x);
    return binary;
}

int main() {
    setlocale(LC_ALL, "Russian");
    Binary binary_a;
    Binary binary_b;
    Binary res;
    std::string binary_x, filename;
    bool exit = false;
    int choice;
    std::ifstream in_file;
    std::ofstream out_file;
    while (!exit) {
        try {
            print_menu();
            choice = input_uint("Выберите действие -> ");

            switch (choice) {
            case 1:
                switch (input_variable()) {
                    case 1:
                        binary_a = input_binary();
                        break;
                    case 2:
                        binary_b = input_binary();
                        break;
                }
                break;
            case 2:
                switch (input_variable()) {
                case 1:
                    binary_a = input_decimal();
                    break;
                case 2:
                    binary_b = input_decimal();
                    break;
                }
                break;
            case 3:
                std::cout << "Введите название файла -> ";
                std::cin >> filename;
                in_file.open(filename);
                if (in_file.is_open())
                {
                    in_file >> binary_x;
                    in_file.close();
                    switch (input_variable()) {
                    case 1:
                        binary_a = Binary(binary_x.length());
                        binary_a.from_binary_string(binary_x);
                        break;
                    case 2:
                        binary_b = Binary(binary_x.length());
                        binary_b.from_binary_string(binary_x);
                        break;
                    }
                }
                else
                    throw std::runtime_error("Не удалось открыть файл.");
                break;
            case 4:
                std::cout << "Введите название файла -> ";
                std::cin >> filename;
                out_file.open(filename);
                if (out_file.is_open())
                {
                    switch (input_variable()) {
                    case 1:
                        binary_a.empty_error();
                        out_file << binary_a.to_binary_string() << std::endl;
                        break;
                    case 2:
                        binary_b.empty_error();
                        out_file << binary_b.to_binary_string() << std::endl;
                        break;
                    }
                    out_file.close();
                }
                else
                    throw std::runtime_error("Не удалось открыть файл.");
                break;
            case 5:
                switch (input_variable()) {
                case 1:
                    binary_a.empty_error();
                    std::cout << "Переменная A - Двоичный вид: " << binary_a.to_binary_string() << " Десятичный вид: " << binary_a.to_decimal() << std::endl;
                    break;
                case 2:
                    binary_b.empty_error();
                    std::cout << "Переменная B - Двоичный вид: " << binary_b.to_binary_string() << " Десятичный вид: " << binary_b.to_decimal() << std::endl;
                    break;
                }
                break;     
            case 6:
                res = binary_a + binary_b;
                std::cout << "Двоичный вид: " << res.to_binary_string() << " Десятичный вид: " << res.to_decimal() << std::endl;
                break;
            case 7:
                res = binary_a - binary_b;
                std::cout << "Двоичный вид: " << res.to_binary_string() << " Десятичный вид: " << res.to_decimal() << std::endl;      
                break;
            case 8:
                res = binary_a * binary_b;
                std::cout << "Двоичный вид: " << res.to_binary_string() << " Десятичный вид: " << res.to_decimal() << std::endl;
                break;
            case 9:
                res = binary_a / binary_b;
                std::cout << "Двоичный вид: " << res.to_binary_string() << " Десятичный вид: " << res.to_decimal() << std::endl;
                break;
            case 10:
                std::cout << (binary_a == binary_b ? "True" : "False") << std::endl;
                break;
            case 11:
                std::cout << std::endl;
                std::strong_ordering bin = binary_a <=> binary_b;
                std::cout << "Результат операции: ";
                if (bin == std::strong_ordering::equal)
                    std::cout << "A == B" << std::endl;
                else if (bin == std::strong_ordering::greater)
                    std::cout << "A > B" << std::endl;
                else if (bin == std::strong_ordering::less)
                    std::cout << "A < B" << std::endl;
                break;
            case 12:
                std::cout << (binary_a != binary_b ? "True" : "False") << std::endl;
                break;
            case 13:
                std::cout << (binary_a < binary_b ? "True" : "False") << std::endl;
                break;
            case 14:
                std::cout << (binary_a > binary_b ? "True" : "False") << std::endl;
                break;
            case 15:
                std::cout << (binary_a <= binary_b ? "True" : "False") << std::endl;
                break;
            case 16:
                std::cout << (binary_a >= binary_b ? "True" : "False") << std::endl;
                break;
            case 17:
                exit = true;
                break;
            default:
                throw std::invalid_argument("Некорректный выбор, попробуйте снова.");
            }
        }
        catch (std::exception e) {
            std::cout << "Ошибка: " << e.what() << std::endl;
        }
    }

    return 0;
}
