#include <iostream>
#include <numeric>

class Rational {
private:
    int num, denom;

public:
    Rational(int a = 0, int b = 1) {
        if (a == 0) {
            num = 0;
            denom = 1;
        } else {
            int g = std::gcd(abs(a), abs(b));
            num = (a * b) / abs(a * b) * abs(a) / g;
            denom = abs(b) / g;
        }
    }
    int Numerator() const {
        return num;
    }
    int Denominator() const {
        return denom;
    }
    Rational operator+() const {
        return Rational(num, denom);
    }
    Rational operator-() const {
        return Rational(-num, denom);
    }
    Rational& operator+=(const Rational& x) {
        *this = *this + x;
        return *this;
    }
    Rational& operator-=(const Rational& x) {
        *this = *this - x;
        return *this;
    }
    Rational& operator*=(const Rational& x) {
        *this = *this * x;
        return *this;
    }
    Rational& operator/=(const Rational& x) {
        *this = *this / x;
        return *this;
    }
    Rational& operator++() {
        num += denom;
        return *this;
    }
    Rational& operator--() {
        num -= denom;
        return *this;
    }
    Rational operator++(int) {
        auto temp = *this;
        num += denom;
        return temp;
    }
    Rational operator--(int) {
        auto temp = *this;
        num -= denom;
        return temp;
    }
    friend Rational operator/(const Rational& a, const Rational& b);

    friend Rational operator*(const Rational& a, const Rational& b);

    friend Rational operator+(const Rational& a, const Rational& b);

    friend Rational operator-(const Rational& a, const Rational& b);
};

Rational operator+(const Rational& a, const Rational& b) {
    return {a.Numerator() * b.Denominator() + b.Numerator() * a.Denominator(),
        a.Denominator() * b.Denominator()};
}
Rational operator-(const Rational& a, const Rational& b) {
    return {a.Numerator() * b.Denominator() - b.Numerator() * a.Denominator(),
            a.Denominator() * b.Denominator()};
}
Rational operator/(const Rational& a, const Rational& b) {
    return {a.Numerator() * b.Denominator(), a.Denominator() * b.Numerator()};
}
Rational operator*(const Rational& a, const Rational& b) {
    return {a.Numerator() * b.Numerator(), a.Denominator() * b.Denominator()};
}
bool operator==(const Rational& a, const Rational& b) {
    return a.Numerator() == b.Numerator() && a.Denominator() == b.Denominator();
}
bool operator!=(const Rational& a, const Rational& b) {
    return !(a.Numerator() == b.Numerator() && a.Denominator() == b.Denominator());
}
