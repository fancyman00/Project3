export module Binary;

#include <string>
#include <stdexcept>
#include <algorithm>

export class Binary {
public:
    // Конструктор по умолчанию (создаёт пустой объект)
    Binary();

    // Конструктор с заданным размером (количество бит)
    explicit Binary(unsigned int size);

    // Конструктор из десятичного числа
    Binary(unsigned long decimal, unsigned int size);

    // Конструктор копирования
    Binary(const Binary& other) = default;

    // Оператор присваивания
    Binary& operator=(const Binary& other) = default;

    // Арифметические операции
    Binary operator+(const Binary& other) const;
    Binary operator-(const Binary& other) const;
    Binary operator*(const Binary& other) const;
    Binary operator/(const Binary& other) const;    // Перегрузка оператора деления

    // Операторы присваивания с арифметическими операциями
    Binary& operator+=(const Binary& other);
    Binary& operator-=(const Binary& other);
    Binary& operator*=(const Binary& other);
    Binary& operator/=(const Binary& other);         // Перегрузка оператора деления с присваиванием

    // Операторы сравнения
    bool operator==(const Binary& other) const;
    bool operator!=(const Binary& other) const;
    bool operator<(const Binary& other) const;
    bool operator<=(const Binary& other) const;
    bool operator>(const Binary& other) const;
    bool operator>=(const Binary& other) const;
    std::strong_ordering operator<=>(const Binary& other) const;

    // Преобразование в десятичную систему
    unsigned long toDecimal() const;

    // Преобразование из строки двоичного числа
    void fromBinaryString(const std::string& binaryStr);

    // Преобразование из десятичного числа
    void fromDecimal(unsigned long decimal);

    // Получение строки представления двоичного числа
    std::string toBinaryString() const;

    // Получение размера (количество бит)
    unsigned int size() const;

    // Проверка на пустоту
    bool isEmpty() const;

private:
    unsigned char bits[100]; // Хранение битов (0 или 1), младший бит index 0
    unsigned int bitSize;     // Реальный размер (количество используемых бит)

    // Вспомогательные методы
    void ensureSameSize(const Binary& other) const;
};

// Реализация методов класса Binary

// Конструктор по умолчанию (создаёт пустой объект)
Binary::Binary() : bitSize(0) {
    // Инициализируем все биты нулями, хотя bitSize = 0 говорит о пустоте
    for (unsigned int i = 0; i < 100; ++i) {
        bits[i] = 0;
    }
}

// Конструктор с заданным размером (количество бит)
Binary::Binary(unsigned int size) : bitSize(size) {
    if (size == 0 || size > 100) {
        throw std::invalid_argument("Размер должен быть от 1 до 100 бит.");
    }
    for (unsigned int i = 0; i < bitSize; ++i) {
        bits[i] = 0; // Инициализируем все биты нулями
    }
}

// Конструктор из десятичного числа
Binary::Binary(unsigned long decimal, unsigned int size) : bitSize(size) {
    if (size == 0 || size > 100) {
        throw std::invalid_argument("Размер должен быть от 1 до 100 бит.");
    }
    // Проверка переполнения: убедимся, что decimal помещается в bitSize бит
    if ((size < sizeof(unsigned long) * 8) && (decimal >= (1UL << size))) {
        throw std::overflow_error("Число превышает допустимый размер.");
    }
    for (unsigned int i = 0; i < bitSize; ++i) {
        bits[i] = decimal % 2;
        decimal /= 2;
    }
}

// Операторы сравнения

bool Binary::operator==(const Binary& other) const {
    if (bitSize != other.bitSize) return false;
    for (unsigned int i = 0; i < bitSize; ++i) {
        if (bits[i] != other.bits[i]) return false;
    }
    return true;
}

bool Binary::operator!=(const Binary& other) const {
    return !(*this == other);
}

bool Binary::operator<(const Binary& other) const {
    ensureSameSize(other);
    for (int i = bitSize - 1; i >= 0; --i) {
        if (bits[i] < other.bits[i]) return true;
        if (bits[i] > other.bits[i]) return false;
    }
    return false;
}

bool Binary::operator<=(const Binary& other) const {
    return (*this < other) || (*this == other);
}

bool Binary::operator>(const Binary& other) const {
    return !(*this <= other);
}

bool Binary::operator>=(const Binary& other) const {
    return !(*this < other);
}
std::strong_ordering Binary::operator<=>(const Binary& other) const {
    ensureSameSize(other);
    for (int i = static_cast<int>(bitSize) - 1; i >= 0; --i) {
        if (bits[i] < other.bits[i]) {
            return std::strong_ordering::less;
        }
        if (bits[i] > other.bits[i]) {
            return std::strong_ordering::greater;
        }
    }
    return std::strong_ordering::equal;
}
// Арифметические операции

Binary Binary::operator+(const Binary& other) const {
    ensureSameSize(other);
    Binary result(bitSize);
    unsigned char carry = 0;
    for (unsigned int i = 0; i < bitSize; ++i) {
        unsigned char sum = bits[i] + other.bits[i] + carry;
        result.bits[i] = sum % 2;
        carry = sum / 2;
    }
    if (carry) {
        throw std::overflow_error("Переполнение при сложении.");
    }
    return result;
}

Binary Binary::operator-(const Binary& other) const {
    ensureSameSize(other);
    Binary result(bitSize);
    signed char borrow = 0;
    for (unsigned int i = 0; i < bitSize; ++i) {
        signed char diff = bits[i] - other.bits[i] - borrow;
        if (diff < 0) {
            result.bits[i] = diff + 2;
            borrow = 1;
        } else {
            result.bits[i] = diff;
            borrow = 0;
        }
    }
    if (borrow) {
        throw std::underflow_error("Переполнение при вычитании.");
    }
    return result;
}

Binary Binary::operator*(const Binary& other) const {
    ensureSameSize(other);
    Binary result(bitSize);
    for (unsigned int i = 0; i < bitSize; ++i) {
        if (other.bits[i]) {
            unsigned char carry = 0;
            for (unsigned int j = 0; j < bitSize - i; ++j) {
                unsigned int index = i + j;
                if (index >= bitSize) break; // Игнорировать переполнение
                unsigned char temp = result.bits[index] + bits[j] + carry;
                result.bits[index] = temp % 2;
                carry = temp / 2;
                if (j == bitSize - i - 1 && carry) {
                    throw std::overflow_error("Переполнение при умножении.");
                }
            }
        }
    }
    return result;
}

Binary Binary::operator/(const Binary& other) const {
    ensureSameSize(other);
    // Проверка деления на ноль
    bool isZero = true;
    for (unsigned int i = 0; i < other.bitSize; ++i) {
        if (other.bits[i] != 0) {
            isZero = false;
            break;
        }
    }
    if (isZero) {
        throw std::invalid_argument("Деление на ноль.");
    }

    // Инициализация делимого и делителя
    Binary dividend(*this); // Копия делимого
    Binary divisor(other);   // Делитель

    // Инициализация результата (частное)
    Binary quotient(bitSize);

    // Рабочий объект для остатка
    Binary remainder(bitSize);

    // Деление бит за битом от старшего к младшему
    for (int i = bitSize - 1; i >= 0; --i) {
        // Сдвигаем остаток влево на 1 бит и добавляем текущий бит делимого
        // Для сдвига умножаем на 2
        for (unsigned int j = bitSize - 1; j > 0; --j) {
            remainder.bits[j] = remainder.bits[j - 1];
        }
        remainder.bits[0] = dividend.bits[i];

        // Проверяем, можно ли вычесть делитель из остатка
        if (!(remainder < divisor)) {
            remainder = remainder - divisor;
            quotient.bits[i] = 1;
        }
    }
    return quotient;
}

// Операторы присваивания с арифметическими операциями

Binary& Binary::operator+=(const Binary& other) {
    *this = *this + other;
    return *this;
}

Binary& Binary::operator-=(const Binary& other) {
    *this = *this - other;
    return *this;
}

Binary& Binary::operator*=(const Binary& other) {
    *this = *this * other;
    return *this;
}

Binary& Binary::operator/=(const Binary& other) {
    *this = *this / other;
    return *this;
}

// Преобразование в десятичную систему
unsigned long Binary::toDecimal() const {
    unsigned long decimal = 0;
    unsigned long base = 1;
    for (unsigned int i = 0; i < bitSize; ++i) {
        decimal += bits[i] * base;
        base *= 2;
    }
    return decimal;
}

// Преобразование из строки двоичного числа
void Binary::fromBinaryString(const std::string& binaryStr) {
    if (binaryStr.length() != bitSize) {
        throw std::invalid_argument("Длина строки не соответствует размеру Binary.");
    }
    for (unsigned int i = 0; i < bitSize; ++i) {
        char c = binaryStr[bitSize - 1 - i];
        if (c == '0') {
            bits[i] = 0;
        } else if (c == '1') {
            bits[i] = 1;
        } else {
            throw std::invalid_argument("Строка должна содержать только '0' или '1'.");
        }
    }
}

// Преобразование из десятичного числа
void Binary::fromDecimal(unsigned long decimal) {
    if ((bitSize < sizeof(unsigned long) * 8) && (decimal >= (1UL << bitSize))) {
        throw std::overflow_error("Число превышает допустимый размер.");
    }
    for (unsigned int i = 0; i < bitSize; ++i) {
        bits[i] = decimal % 2;
        decimal /= 2;
    }
}

// Получение строки представления двоичного числа
std::string Binary::toBinaryString() const {
    std::string str;
    str.reserve(bitSize);
    for (int i = bitSize - 1; i >= 0; --i) {
        str += (bits[i] ? '1' : '0');
    }
    return str;
}

// Получение размера (количество бит)
unsigned int Binary::size() const {
    return bitSize;
}

// Проверка на пустоту
bool Binary::isEmpty() const {
    return bitSize == 0;
}

// Вспомогательные методы

// Проверка, что размеры двух чисел совпадают
void Binary::ensureSameSize(const Binary& other) const {
    if (bitSize != other.bitSize) {
        throw std::invalid_argument("Размеры чисел должны совпадать.");
    }
}