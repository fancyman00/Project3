#include <iostream>;
#include <Windows.h>;
#include <fstream>
import Binary;

void printMenu() {
    std::cout << "\n1. Ввести двоичное число с клавиатуры\n"
        << "2. Ввести десятичное число с клавиатуры\n"
        << "3. Ввести двоичное число из файла\n"
        << "4. Вывести двоичное число на экран\n"
        << "5. Вывести десятичное число на экран\n"
        << "6. Сохранить двоичное число в файл\n"
        << "7. Выполнить операцию +\n"
        << "8. Выполнить операцию -\n"
        << "9. Выполнить операцию *\n"
        << "10. Выполнить операцию /\n"
        << "11. Выполнить операцию ==\n"
        << "12. Выполнить операцию <=>\n"
        << "13. Выполнить операцию !=\n"
        << "14. Выполнить операцию <\n"
        << "15. Выполнить операцию >\n"
        << "16. Выполнить операцию <=\n"
        << "17. Выполнить операцию >=\n"
        << "18. Выход\n"
        << "Выберите действие: ";
}
Binary input_binary() {
    int size;  
    std::string binary_x;
    std::cout << "Введите размер -> ";
    std::cin >> size;
    std::cout << "Введите двоичное число -> ";
    std::cin >> binary_x;
    Binary binary(size);
    binary.fromBinaryString(binary_x);
    return binary;
}
Binary input_decimal() {
    int size;
    int x;
    std::cout << "Введите размер -> ";
    std::cin >> size;
    std::cout << "Введите десятичное число -> ";
    std::cin >> x;
    Binary binary(size);
    binary.fromDecimal(x);
    return binary;
}
int main() {
    setlocale(LC_ALL, "Russian");
    Binary binary;
    int decimal_x, size;
    std::string binary_x, filename;
    bool res, exit = false;
    int choice;
    std::ifstream in_file;
    std::ofstream out_file;
    while (!exit) {
        try {
            printMenu();
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
                    binary.fromBinaryString(binary_x);
                }
                else 
                {
                    std::cout << "Не удалось открыть файл." << std::endl;
                }
                break;
            case 4:
                std::cout << "Текущее число (Двоичный вид): " << binary.toBinaryString() << std::endl;
                break;
            case 5:
                std::cout << "Текущее число (Десятичный вид): " << binary.toDecimal() << std::endl;
                break;
            case 6:
                std::cout << "Введите название файла -> ";
                std::cin >> filename;
                out_file.open(filename);
                if (out_file.is_open()) 
                {
                    out_file << binary.toBinaryString() << std::endl;
                    out_file.close();
                }
                else 
                {
                    std::cout << "Не удалось открыть файл." << std::endl;
                }
                break;
            case 7:
                if (binary.isEmpty())
                    std::cout << "Введите начальное число!" << std::endl;
                else
                    binary = binary + input_binary();
                break;
            case 8:
                if (binary.isEmpty())
                    std::cout << "Введите начальное число!" << std::endl;
                else
                    binary = binary - input_binary();
                break;
            case 9:
                if (binary.isEmpty())
                    std::cout << "Введите начальное число!" << std::endl;
                else
                    binary = binary * input_binary();
                break;
            case 10:
                if (binary.isEmpty())
                    std::cout << "Введите начальное число!" << std::endl;
                else
                    binary = binary / input_binary();
                break;
            case 11:
                if (binary.isEmpty())
                    std::cout << "Введите начальное число!" << std::endl;
                else
                    std::cout << (binary == input_binary() ? "True" : "False") << std::endl;
                break;
            case 12:
                if (binary.isEmpty())
                    std::cout << "Введите начальное число!" << std::endl;
                else 
                {
                    std::cout << std::endl;
                    std::strong_ordering bin = binary <=> input_binary();
                    if (bin == std::strong_ordering::equal)
                        std::cout << "=" << std::endl;
                    else if (bin == std::strong_ordering::greater)
                        std::cout << ">" << std::endl;
                    else if (bin == std::strong_ordering::less)
                        std::cout << "<" << std::endl;
                }
                break;
            case 13:
                if (binary.isEmpty())
                    std::cout << "Введите начальное число!" << std::endl;
                else
                    std::cout << (binary != input_binary() ? "True" : "False") << std::endl;
                break;
            case 14:
                if (binary.isEmpty())
                    std::cout << "Введите начальное число!" << std::endl;
                else
                    std::cout << (binary < input_binary() ? "True" : "False") << std::endl;
                break;
            case 15:
                if (binary.isEmpty())
                    std::cout << "Введите начальное число!" << std::endl;
                else 
                    std::cout << (binary > input_binary() ? "True" : "False") << std::endl;
                break;
            case 16:
                if (binary.isEmpty())
                    std::cout << "Введите начальное число!" << std::endl;
                else
                    std::cout << (binary <= input_binary() ? "True" : "False") << std::endl;
                break;
            case 17:
                if (binary.isEmpty())
                    std::cout << "Введите начальное число!" << std::endl;
                else
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
