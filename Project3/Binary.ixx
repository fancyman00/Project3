export module Binary;

#include <string>
#include <stdexcept>
#include <algorithm>
#include <compare>

export class Binary {
public:
    Binary();
    Binary(unsigned long long decimal);
    Binary(const std::string binary_str);
    Binary(const Binary& other) = default;
    Binary& operator=(const Binary& other) = default;

    Binary operator+(const Binary& other) const;
    Binary operator-(const Binary& other) const;
    Binary operator*(const Binary& other) const;
    Binary operator^(unsigned int exponent) const;
    Binary operator/(const Binary& other) const;

    Binary& operator+=(const Binary& other);
    Binary& operator-=(const Binary& other);
    Binary& operator*=(const Binary& other);
    Binary& operator/=(const Binary& other);

    bool operator==(const Binary& other) const;
    bool operator!=(const Binary& other) const;
    std::strong_ordering operator<=>(const Binary& other) const;

    unsigned long long to_decimal() const;
    std::string to_binary_string() const;
    unsigned int size() const;
    bool is_empty() const;
    void empty_error() const;

private:
    unsigned char bits[100] = { 0 };
    unsigned int bit_size = 0;

    static unsigned int significant_bits(unsigned long long value);
    void update_size();
    void shift_left();
};

Binary::Binary() {}

Binary::Binary(unsigned long long decimal) {
    bit_size = significant_bits(decimal);
    for (unsigned int i = 0; i < bit_size; ++i) {
        bits[i] = decimal % 2;
        decimal /= 2;
    }
}

Binary::Binary(const std::string binary_str) {
    bit_size = binary_str.length();
    if (binary_str.empty() || bit_size > 100) {
        throw std::invalid_argument("Размер должен быть от 1 до 100 бит.");
    }
    for (unsigned int i = 0; i < bit_size; ++i) {
        char c = binary_str[bit_size - 1 - i];
        if (c == '0') {
            bits[i] = 0;
        }
        else if (c == '1') {
            bits[i] = 1;
        }
        else {
            throw std::invalid_argument("Строка должна содержать только '0' или '1'.");
        }
    }
}

bool Binary::operator==(const Binary& other) const {
    if (bit_size != other.bit_size) return false;
    for (unsigned int i = 0; i < bit_size; ++i) {
        if (bits[i] != other.bits[i]) return false;
    }
    return true;
}

bool Binary::operator!=(const Binary& other) const {
    return !(*this == other);
}

std::strong_ordering Binary::operator<=>(const Binary& other) const {
    if (bit_size != other.bit_size) {
        return bit_size <=> other.bit_size;
    }
    for (int i = static_cast<int>(bit_size) - 1; i >= 0; --i) {
        if (bits[i] != other.bits[i]) {
            return bits[i] <=> other.bits[i];
        }
    }
    return std::strong_ordering::equal;
}

Binary Binary::operator+(const Binary& other) const {
    unsigned int max_size = std::max(bit_size, other.bit_size);
    Binary result;
    result.bit_size = max_size;

    unsigned char carry = 0;
    for (unsigned int i = 0; i < max_size; ++i) {
        unsigned char bit1 = (i < bit_size) ? bits[i] : 0;
        unsigned char bit2 = (i < other.bit_size) ? other.bits[i] : 0;

        unsigned char sum = bit1 + bit2 + carry;
        result.bits[i] = sum % 2;
        carry = sum / 2;
    }
    if (carry) {
        if (max_size >= 100) {
            throw std::overflow_error("Результат сложения превышает 100 бит.");
        }
        result.bits[max_size] = carry;
        result.bit_size++;
    }
    return result;
}

Binary Binary::operator-(const Binary& other) const {
    if (*this < other) {
        throw std::underflow_error("Результат вычитания отрицательный.");
    }
    Binary result;
    signed char borrow = 0;
    for (unsigned int i = 0; i < bit_size; ++i) {
        signed char bit1 = bits[i];
        signed char bit2 = (i < other.bit_size) ? other.bits[i] : 0;

        signed char diff = bit1 - bit2 - borrow;
        if (diff < 0) {
            result.bits[i] = diff + 2;
            borrow = 1;
        }
        else {
            result.bits[i] = diff;
            borrow = 0;
        }
    }
    result.update_size();
    return result;
}

Binary Binary::operator*(const Binary& other) const {
    if (bit_size == 0 || other.bit_size == 0) {
        return Binary(0);
    }
    Binary result;
    for (unsigned int i = 0; i < other.bit_size; ++i) {
        if (other.bits[i] == 1) {
            Binary temp;
            unsigned char carry = 0;
            for (unsigned int j = 0; j < bit_size; ++j) {
                unsigned int index = i + j;
                if (index >= 100) {
                    throw std::overflow_error("Результат умножения превышает 100 бит.");
                }
                unsigned char sum = result.bits[index] + bits[j] + carry;
                result.bits[index] = sum % 2;
                carry = sum / 2;
            }
            if (carry) {
                if (i + bit_size >= 100) {
                    throw std::overflow_error("Результат умножения превышает 100 бит.");
                }
                result.bits[i + bit_size] = carry;
            }
        }
    }
    result.update_size();
    return result;
}

Binary Binary::operator^(unsigned int exponent) const {
    if (exponent == 0) {
        return Binary(1);
    }
    Binary result(1);
    Binary base = *this;
    for (unsigned int i = 0; i < exponent; ++i) {
        result = result * base;
    }
    return result;
}

Binary Binary::operator/(const Binary& other) const {
    if (other.is_empty()) {
        throw std::invalid_argument("Деление на ноль.");
    }
    Binary quotient;
    Binary remainder;

    // Итерация от старшего бита к младшему
    for (int i = bit_size - 1; i >= 0; --i) {
        remainder.shift_left();                // Сдвигаем остаток влево
        remainder.bits[0] = bits[i];          // Добавляем текущий бит
        remainder.update_size();

        if (remainder >= other) {
            remainder = remainder - other;    // Вычитаем делитель из остатка
            quotient.bits[i] = 1;             // Устанавливаем бит в частном
        }
    }

    quotient.update_size();                   // Обновляем размер частного
    return quotient;
}




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

unsigned long long Binary::to_decimal() const {
    unsigned long long result = 0;
    for (unsigned int i = 0; i < bit_size; ++i) {
        result += bits[i] * (1ULL << i);
    }
    return result;
}

std::string Binary::to_binary_string() const {
    std::string str;
    str.reserve(bit_size);
    for (int i = bit_size - 1; i >= 0; --i) {
        str += (bits[i] ? '1' : '0');
    }
    return str;
}

unsigned int Binary::size() const {
    return bit_size;
}

bool Binary::is_empty() const {
    return bit_size == 0;
}

void Binary::empty_error() const {
    if (is_empty()) {
        throw std::invalid_argument("Отсутствует значение Binary!");
    }
}

unsigned int Binary::significant_bits(unsigned long long value) {
    unsigned int count = 0;
    while (value > 0) {
        count++;
        value >>= 1;
    }
    return count;
}

void Binary::update_size() {
    for (int i = 99; i >= 0; --i) {
        if (bits[i] != 0) {
            bit_size = i + 1;
            return;
        }
    }
    bit_size = 0;
}

void Binary::shift_left() {
    for (unsigned int i = 99; i > 0; --i) {
        bits[i] = bits[i - 1];
    }
    bits[0] = 0;
}
