#include "biginteger.hpp"
#include <cctype>

void BigInteger::deleteMemory() {
    digits.clear();
    minus = false;
}


void BigInteger::makeLenthAs(size_t to) {
    size_t difference = to - digits.size();
    for (size_t i = 0; i < difference; ++i) {
        digits.push_back(0);
    }
}

 BigInteger::BigInteger(int n) : BigInteger(static_cast<int64_t>(n)) {}


 BigInteger::BigInteger(uint64_t number) {
     if (number == 0) {
         minus = false;
         digits.push_back(0);
     } else {
         while (number > 0) {
             digits.push_back(number % 10);
             number /= 10;
         }
         minus = false;
     }
}


 BigInteger::BigInteger(int64_t number) {
     if (number == 0) {
         minus = false;
         digits.push_back(0);
     } else {
         uint64_t number2;
         if (number < 0) {
             minus = true;
             number2 = static_cast<uint64_t>(-(number + 1)) + 1;
         } else {
             minus = false;
             number2 = static_cast<uint64_t>(number);
         }
         while (number2 > 0) {
             digits.push_back(number2 % 10);
             number2 /= 10;
         }
     }
}

BigInteger::BigInteger(const BigInteger& other) : digits({}), minus(other.minus) {
    for (size_t i = 0; i < other.digits.size(); ++i) {
        digits.push_back(other.digits[i]);
    }
}

BigInteger::BigInteger(BigInteger&& other) noexcept : digits(std::move(other.digits)), minus(other.minus) {
    other.minus = false;
}


BigInteger::BigInteger(const std::string& other): digits(std::vector<int>{}), minus(false) {
    size_t i = 0;
    if (other[i] == '-') {
        minus = true;
        ++i;
    }
    while (other[i] == '0' && i < other.size()) {++i;}
    for (size_t j = other.size();i < j; --j) {
        digits.push_back(other[j - 1] - '0');
    }
}


BigInteger& BigInteger::operator=(const BigInteger& other) {
    if (this == &other) {return *this;}
    digits = other.digits;
    minus = other.minus;
    return *this;
}

BigInteger& BigInteger::operator=(BigInteger&& other) noexcept {
    if (this == &other) {return *this;}
    digits = std::move(other.digits);
    minus = other.minus;
    other.minus = false;
    return *this;
}

std::strong_ordering BigInteger::operator<=>(const BigInteger& other) const{
    if (minus == true && other.minus == false) {return std::strong_ordering::less;}
    if (minus == false && other.minus == true) {return std::strong_ordering::greater;}

    if (minus == false && other.minus == false) {

        if (digits.size() < other.digits.size()) {return std::strong_ordering::less;}
        if (digits.size() > other.digits.size()) {return std::strong_ordering::greater;}
        for (size_t i = digits.size(); i > 0; --i) {
            size_t ind = i - 1;
            if (digits[ind] < other.digits[ind]) {
                return std::strong_ordering::less;
            } else if (digits[ind] > other.digits[ind]) {
                return std::strong_ordering::greater;
            }
        }
    } else {
        if (digits.size() < other.digits.size()) {return std::strong_ordering::greater;}
        if (digits.size() > other.digits.size()) {return std::strong_ordering::less;}
        for (size_t i = digits.size(); i > 0; --i) {
            size_t ind = i - 1;
            if (digits[ind] < other.digits[ind]) {
                return std::strong_ordering::greater;
            } else if (digits[ind] > other.digits[ind]) {
                return std::strong_ordering::less;
            }
        }
    }
    return std::strong_ordering::equal;
}

BigInteger& BigInteger::operator+=(const BigInteger& other2) {
    BigInteger other = other2; //можно и лучше но так думать не надо)
    if (minus == other.minus) {
        int to_next = 0;
        int to_next2 = 0;
        if (digits.size() < other.digits.size()) {
            this->makeLenthAs(other.digits.size() + 1);
            other.makeLenthAs(other.digits.size() + 1);
        } else if (digits.size() > other.digits.size()) {
            other.makeLenthAs(digits.size() + 1);
            this->makeLenthAs(digits.size() + 1);
        } else {
            this->makeLenthAs(digits.size() + 1);
            other.makeLenthAs(digits.size() + 1);
        }
        for (size_t i = 0; i < digits.size(); ++i) {
            to_next2 = (digits[i] + other.digits[i] + to_next) / 10;
            digits[i] = (digits[i] + other.digits[i] + to_next) % 10;
            to_next = to_next2;
        }
        return *this;
    } else if (minus == false && other.minus == true) {
        return *this -= other;
    } else {
        // кринж надо с модулями работать
        return *this;//понятно что не так надо
    }
}

// -= также почти)

BigInteger& BigInteger::operator*=(const BigInteger& other) {
    if (minus != other.minus) {
        minus = true;
    } else {
        minus = false;
    }
    std::vector<int> res(digits.size() + other.digits.size(), 0);

    for (size_t i = 0; i < digits.size(); ++i) {
        for (size_t j = 0; j < other.digits.size(); ++j) {
            res[i + j] += digits[i] * other.digits[j];
        }
    }

    for (size_t i = 0; i + 1 < res.size(); ++i) {
        res[i + 1] += res[i] / 10;
        res[i] %= 10;
    }

    while (res.size() > 1 && res.back() == 0) {
        res.pop_back();
    }

    if (res.size() == 1 && res[0] == 0) {
        minus = false;
    }

    digits = std::move(res);
    return *this;
}

std::string BigInteger::ToString() const {
    std::string res;
    if (minus == true) {
        res += '-';
    }
    for (size_t i = digits.size(); i > 0; --i) {
        res += static_cast<char>(digits[i - 1] + '0');
    }

    return res;
}

BigInteger::operator bool() const {
    return digits.size() != 1 || digits[0] != 0;
}

std::istream& operator>>(std::istream& in, BigInteger& bigint) {
    bigint.deleteMemory();
    std::string str;
    in >> str;
    bigint = BigInteger(str);
    return in;
}

std::ostream& operator<<(std::ostream& out, const BigInteger& bigint) {
    out << bigint.ToString();
    return out;
}

BigInteger operator""_bi(const char* str) {
    return BigInteger(std::string(str));
}

BigInteger operator""_bi(const char* str, size_t len) {
    return BigInteger(std::string(str, len));
}


BigInteger BigInteger::operator+() const {
    return *this;
}

BigInteger BigInteger::operator-() const {
    BigInteger result(*this);

    if (result) {
        result.minus = !result.minus;
    }

    return result;
}
