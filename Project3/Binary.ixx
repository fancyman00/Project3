export module Binary;

#include <string>
#include <stdexcept>
#include <algorithm>

export class Binary {
public:
    // ����������� �� ��������� (������ ������ ������)
    Binary();

    // ����������� � �������� �������� (���������� ���)
    explicit Binary(unsigned int size);

    // ����������� �� ����������� �����
    Binary(unsigned long decimal, unsigned int size);

    // ����������� �����������
    Binary(const Binary& other) = default;

    // �������� ������������
    Binary& operator=(const Binary& other) = default;

    // �������������� ��������
    Binary operator+(const Binary& other) const;
    Binary operator-(const Binary& other) const;
    Binary operator*(const Binary& other) const;
    Binary operator/(const Binary& other) const;    // ���������� ��������� �������

    // ��������� ������������ � ��������������� ����������
    Binary& operator+=(const Binary& other);
    Binary& operator-=(const Binary& other);
    Binary& operator*=(const Binary& other);
    Binary& operator/=(const Binary& other);         // ���������� ��������� ������� � �������������

    // ��������� ���������
    bool operator==(const Binary& other) const;
    bool operator!=(const Binary& other) const;
    bool operator<(const Binary& other) const;
    bool operator<=(const Binary& other) const;
    bool operator>(const Binary& other) const;
    bool operator>=(const Binary& other) const;
    std::strong_ordering operator<=>(const Binary& other) const;

    // �������������� � ���������� �������
    unsigned long toDecimal() const;

    // �������������� �� ������ ��������� �����
    void fromBinaryString(const std::string& binaryStr);

    // �������������� �� ����������� �����
    void fromDecimal(unsigned long decimal);

    // ��������� ������ ������������� ��������� �����
    std::string toBinaryString() const;

    // ��������� ������� (���������� ���)
    unsigned int size() const;

    // �������� �� �������
    bool isEmpty() const;

private:
    unsigned char bits[100]; // �������� ����� (0 ��� 1), ������� ��� index 0
    unsigned int bitSize;     // �������� ������ (���������� ������������ ���)

    // ��������������� ������
    void ensureSameSize(const Binary& other) const;
};

// ���������� ������� ������ Binary

// ����������� �� ��������� (������ ������ ������)
Binary::Binary() : bitSize(0) {
    // �������������� ��� ���� ������, ���� bitSize = 0 ������� � �������
    for (unsigned int i = 0; i < 100; ++i) {
        bits[i] = 0;
    }
}

// ����������� � �������� �������� (���������� ���)
Binary::Binary(unsigned int size) : bitSize(size) {
    if (size == 0 || size > 100) {
        throw std::invalid_argument("������ ������ ���� �� 1 �� 100 ���.");
    }
    for (unsigned int i = 0; i < bitSize; ++i) {
        bits[i] = 0; // �������������� ��� ���� ������
    }
}

// ����������� �� ����������� �����
Binary::Binary(unsigned long decimal, unsigned int size) : bitSize(size) {
    if (size == 0 || size > 100) {
        throw std::invalid_argument("������ ������ ���� �� 1 �� 100 ���.");
    }
    // �������� ������������: ��������, ��� decimal ���������� � bitSize ���
    if ((size < sizeof(unsigned long) * 8) && (decimal >= (1UL << size))) {
        throw std::overflow_error("����� ��������� ���������� ������.");
    }
    for (unsigned int i = 0; i < bitSize; ++i) {
        bits[i] = decimal % 2;
        decimal /= 2;
    }
}

// ��������� ���������

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
// �������������� ��������

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
        throw std::overflow_error("������������ ��� ��������.");
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
        throw std::underflow_error("������������ ��� ���������.");
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
                if (index >= bitSize) break; // ������������ ������������
                unsigned char temp = result.bits[index] + bits[j] + carry;
                result.bits[index] = temp % 2;
                carry = temp / 2;
                if (j == bitSize - i - 1 && carry) {
                    throw std::overflow_error("������������ ��� ���������.");
                }
            }
        }
    }
    return result;
}

Binary Binary::operator/(const Binary& other) const {
    ensureSameSize(other);
    // �������� ������� �� ����
    bool isZero = true;
    for (unsigned int i = 0; i < other.bitSize; ++i) {
        if (other.bits[i] != 0) {
            isZero = false;
            break;
        }
    }
    if (isZero) {
        throw std::invalid_argument("������� �� ����.");
    }

    // ������������� �������� � ��������
    Binary dividend(*this); // ����� ��������
    Binary divisor(other);   // ��������

    // ������������� ���������� (�������)
    Binary quotient(bitSize);

    // ������� ������ ��� �������
    Binary remainder(bitSize);

    // ������� ��� �� ����� �� �������� � ��������
    for (int i = bitSize - 1; i >= 0; --i) {
        // �������� ������� ����� �� 1 ��� � ��������� ������� ��� ��������
        // ��� ������ �������� �� 2
        for (unsigned int j = bitSize - 1; j > 0; --j) {
            remainder.bits[j] = remainder.bits[j - 1];
        }
        remainder.bits[0] = dividend.bits[i];

        // ���������, ����� �� ������� �������� �� �������
        if (!(remainder < divisor)) {
            remainder = remainder - divisor;
            quotient.bits[i] = 1;
        }
    }
    return quotient;
}

// ��������� ������������ � ��������������� ����������

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

// �������������� � ���������� �������
unsigned long Binary::toDecimal() const {
    unsigned long decimal = 0;
    unsigned long base = 1;
    for (unsigned int i = 0; i < bitSize; ++i) {
        decimal += bits[i] * base;
        base *= 2;
    }
    return decimal;
}

// �������������� �� ������ ��������� �����
void Binary::fromBinaryString(const std::string& binaryStr) {
    if (binaryStr.length() != bitSize) {
        throw std::invalid_argument("����� ������ �� ������������� ������� Binary.");
    }
    for (unsigned int i = 0; i < bitSize; ++i) {
        char c = binaryStr[bitSize - 1 - i];
        if (c == '0') {
            bits[i] = 0;
        } else if (c == '1') {
            bits[i] = 1;
        } else {
            throw std::invalid_argument("������ ������ ��������� ������ '0' ��� '1'.");
        }
    }
}

// �������������� �� ����������� �����
void Binary::fromDecimal(unsigned long decimal) {
    if ((bitSize < sizeof(unsigned long) * 8) && (decimal >= (1UL << bitSize))) {
        throw std::overflow_error("����� ��������� ���������� ������.");
    }
    for (unsigned int i = 0; i < bitSize; ++i) {
        bits[i] = decimal % 2;
        decimal /= 2;
    }
}

// ��������� ������ ������������� ��������� �����
std::string Binary::toBinaryString() const {
    std::string str;
    str.reserve(bitSize);
    for (int i = bitSize - 1; i >= 0; --i) {
        str += (bits[i] ? '1' : '0');
    }
    return str;
}

// ��������� ������� (���������� ���)
unsigned int Binary::size() const {
    return bitSize;
}

// �������� �� �������
bool Binary::isEmpty() const {
    return bitSize == 0;
}

// ��������������� ������

// ��������, ��� ������� ���� ����� ���������
void Binary::ensureSameSize(const Binary& other) const {
    if (bitSize != other.bitSize) {
        throw std::invalid_argument("������� ����� ������ ���������.");
    }
}