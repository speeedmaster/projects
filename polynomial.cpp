#include <iostream>
#include <map>
#include <vector>

template <typename T>
class Polynomial {
private:
    std::map<size_t, T> content;

public:
    Polynomial(const std::vector<T>& origin) {
        for (size_t i = 0; i < origin.size(); ++i) {
            if (origin[i] != T(0))
                content.insert({i, origin[i]});
        }
    }
    Polynomial(const T& value = T()) {
        content.insert({0, value});
    }

    template<typename It>
    Polynomial(const It first, const It last) {
        size_t i = 0;
        It f = first;
        for (; f!= last; ++f) {
            if (*f != T(0))
                content.insert({i, *f});
            ++i;
        }
    }
    friend bool operator==(const Polynomial<T>& a, const Polynomial<T>& b) {
        return a.content == b.content;
    }
    friend bool operator!=(const Polynomial<T>& a, const Polynomial<T>& b) {
        return a.content != b.content;
    }
    Polynomial& operator+=(const Polynomial& other) {
        for (auto it = other.content.begin(); it != other.content.end(); ++it) {
            content[it->first] += it->second;
            if (content[it->first] == 0)
                content.erase(content.find(it->first));
        }
        if (content.empty())
            content[0] = T(0);
        return *this;
    }
    friend Polynomial<T> operator+(const Polynomial<T>& a, const Polynomial<T>& b) {
        Polynomial ans(a);
        ans += b;
        return ans;
    }

    Polynomial& operator-=(const Polynomial& other) {
        for (auto it = other.content.begin(); it != other.content.end(); ++it) {
            content[it->first] -= it->second;
            if (content[it->first] == 0)
                content.erase(content.find(it->first));
        }
        if (content.empty())
            content[0] = T(0);
        return *this;
    }
    friend Polynomial<T> operator-(const Polynomial<T>& a, const Polynomial<T>& b) {
        Polynomial ans(a);
        ans -= b;
        return ans;
    }

    Polynomial& operator*=(const Polynomial& other) {
        std::map<size_t, T> ans;
        for (auto i = other.content.begin(); i != other.content.end(); ++i) {
            for (auto j = content.begin(); j != content.end(); ++j) {
                ans[i->first + j->first] += (i->second) * (j->second);
                if (ans[i->first + j->first] == 0)
                    ans.erase(ans.find(i->first + j->first));
            }
        }
        content = ans;
        if (content.empty())
            content[0] = T(0);
        return *this;
    }
    friend Polynomial<T> operator*(const Polynomial<T>& a, const Polynomial<T>& b) {
        Polynomial ans(a);
        ans *= b;
        return ans;
    }
    friend Polynomial<T> operator/(const Polynomial<T>& a, const Polynomial<T>& b) {
        Polynomial temp(a);
        Polynomial ans(T(0));
        while (temp.Degree() >= b.Degree()) {
            std::vector<T> d(temp.Degree() - b.Degree() + 1, T(0));
            T quo = temp.content.rbegin()->second / b.content.rbegin()->second;
            d.back() = quo;
            ans += Polynomial(d);
            temp -= b * Polynomial(d);
        }
        return ans;
    }
    friend Polynomial<T> operator%(const Polynomial<T>& a, const Polynomial<T>& b) {
        return a - (a / b) * b;
    }
    friend Polynomial<T> operator,(const Polynomial<T>& a, const Polynomial<T>& b) {
        Polynomial a1(a);
        Polynomial b1(b);
        while (b1 != T(0)) {
            a1 = a1 % b1;
            Polynomial k(a1);
            a1 = b1;
            b1 = k;
        }
        if (a1 != T(0)) {
            a1 = a1 / a1.content.rbegin()->second;
        }
        return a1;
    }

    const T operator[] (size_t i) const {
        if (content.find(i) == content.end())
            return T();
        return content.at(i);
    }

    int Degree() const {
        if (content.size() == 1 && content.find(0) != content.end())
            if (content.at(0) == T(0))
                return -1;
        return content.rbegin()->first;
    }
    T operator()(const T& x) const {
        T ans = T();
        ans = content.begin()->second;
        T pow = x;
        auto i = content.begin();
        ++i;
        for (; i != content.end(); ++i) {
            ans += i->second * pow;
            pow *= x;
        }
        return ans;
    }

    typename std::map<size_t, T>::const_iterator begin() const {return content.cbegin();}
    typename std::map<size_t, T>::const_iterator end() const {return content.cend();}

    typename std::map<size_t, T>::iterator begin() {return content.begin();}
    typename std::map<size_t, T>::iterator end() {return content.end();}

    Polynomial operator&(const Polynomial& g) const {
        Polynomial ans(T(0));
        Polynomial pow(g);
        for (size_t i = 0; i < static_cast<size_t>(this->Degree()); ++i) {
            if (content.find(i) != content.end())
                ans += content.at(i) * pow;
            pow *= g;
        }
        return ans;
     }
};


template <typename T>
std::ostream& operator << (std::ostream& out, const Polynomial<T>& p) {
    if (p == T())
        out << T();
    for (int i = p.Degree(); i >= 0; --i) {
        if (p.Degree() == 0) {
            out << p[0];
        } else {
            if (i == p.Degree()) {
                if (p[i] == T(1)) {
                    out << 'x';
                } else if (p[i] == T(-1)) {
                    out << "-x";
                } else {
                    out << p[i];
                    out << "*x";
                }
                if (i > 1)
                    out << '^' << i;
            } else {
                if (i > 0) {
                    if (p[i] != T()) {
                        if (p[i] > T())
                            out << '+';
                        if (p[i] == T(1)) {
                            out << 'x';
                        } else if (p[i] == T(-1)) {
                            out << "-x";
                        } else {
                            out << p[i];
                            out << "*x";
                        }
                        if (i > 1)
                            out << '^' << i;
                    }
                } else {
                    if (p[i] != T()) {
                        if (p[i] > T())
                            out << '+';
                        out << p[i];
                    }
                }
            }
        }
    }
    return out;
}
