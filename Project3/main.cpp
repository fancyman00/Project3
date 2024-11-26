#include <iostream>;
#include <Windows.h>;
#include <fstream>
import Binary;

void printMenu() {
    std::cout << "\n1. ������ �������� ����� � ����������\n"
        << "2. ������ ���������� ����� � ����������\n"
        << "3. ������ �������� ����� �� �����\n"
        << "4. ������� �������� ����� �� �����\n"
        << "5. ������� ���������� ����� �� �����\n"
        << "6. ��������� �������� ����� � ����\n"
        << "7. ��������� �������� +\n"
        << "8. ��������� �������� -\n"
        << "9. ��������� �������� *\n"
        << "10. ��������� �������� /\n"
        << "11. ��������� �������� ==\n"
        << "12. ��������� �������� <=>\n"
        << "13. ��������� �������� !=\n"
        << "14. ��������� �������� <\n"
        << "15. ��������� �������� >\n"
        << "16. ��������� �������� <=\n"
        << "17. ��������� �������� >=\n"
        << "18. �����\n"
        << "�������� ��������: ";
}
Binary input_binary() {
    int size;  
    std::string binary_x;
    std::cout << "������� ������ -> ";
    std::cin >> size;
    std::cout << "������� �������� ����� -> ";
    std::cin >> binary_x;
    Binary binary(size);
    binary.fromBinaryString(binary_x);
    return binary;
}
Binary input_decimal() {
    int size;
    int x;
    std::cout << "������� ������ -> ";
    std::cin >> size;
    std::cout << "������� ���������� ����� -> ";
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
    bool res;
    int choice;
    std::ifstream in_file;
    std::ofstream out_file;
    while (true) {
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
                std::cout << "������� �������� ����� -> ";
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
                    std::cout << "�� ������� ������� ����." << std::endl;
                }
                break;
            case 4:
                std::cout << "������� ����� (�������� ���): " << binary.toBinaryString() << std::endl;
                break;
            case 5:
                std::cout << "������� ����� (���������� ���): " << binary.toDecimal() << std::endl;
                break;
            case 6:
                std::cout << "������� �������� ����� -> ";
                std::cin >> filename;
                out_file.open(filename);
                if (out_file.is_open()) 
                {
                    out_file << binary_x << std::endl;
                    out_file.close();
                }
                else 
                {
                    std::cout << "�� ������� ������� ����." << std::endl;
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
                std::cout << (binary == input_binary() ? "True" : "False") << std::endl;
                break;
            case 13:
                std::cout << (binary != input_binary() ? "True" : "False") << std::endl;
                break;
            case 14:
                std::cout << (binary < input_binary() ? "True" : "False") << std::endl;
                break;
            case 15:
                break;
            case 16:
                std::cout << (binary > input_binary() ? "True" : "False") << std::endl;
                break;
            case 17:
                std::cout << (binary <= input_binary() ? "True" : "False") << std::endl;
                break;
            case 18:
                std::cout << (binary >= input_binary() ? "True" : "False") << std::endl;
                break;
            default:
                std::cout << "������������ �����, ���������� �����.\n";
            }
        }
        catch (std::exception e) {
            std::cout << "������: " << e.what() << std::endl;
        }
    }

    return 0;
}