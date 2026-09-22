#include <vector>
#include <iostream>
#include <cstdint>
#include <string>
class BigInteger {
private:
    std::vector<int> digits;
    bool minus = false;
public:
    BigInteger() {
        digits = {0};
        minus = false;
    }
    BigInteger(int);
    BigInteger(uint64_t);
    BigInteger(int64_t);
    BigInteger(const BigInteger& other);
    BigInteger(BigInteger&& other) noexcept;
    BigInteger(const std::string&);

    BigInteger& operator=(const BigInteger& other);
    BigInteger& operator=(BigInteger&& other) noexcept;

    std::strong_ordering operator<=>(const BigInteger&) const;

    BigInteger& operator+=(const BigInteger& other);
    BigInteger& operator-=(const BigInteger& other);
    BigInteger& operator/=(const BigInteger& other);
    BigInteger& operator%=(const BigInteger& other);
    BigInteger& operator*=(const BigInteger& other);

    BigInteger& operator++();
    BigInteger& operator--();
    BigInteger operator--(int);
    BigInteger operator++(int);

    bool operator==(const BigInteger&) const;

    void makeLenthAs(size_t);
    void deleteMemory();

    std::string ToString() const;

    explicit operator bool() const;

    BigInteger operator+() const;
    BigInteger operator-() const;

};

std::istream& operator>>(std::istream& in, BigInteger&);
std::ostream& operator<<(std::ostream& out, const BigInteger&);

BigInteger operator+(BigInteger, const BigInteger&);
BigInteger operator-(BigInteger, const BigInteger&);
BigInteger operator/(BigInteger, const BigInteger&);
BigInteger operator%(BigInteger, const BigInteger&);
BigInteger operator*(BigInteger, const BigInteger&);

BigInteger operator""_bi(const char*);
BigInteger operator""_bi(const char*, size_t); //строки неявно передают len
