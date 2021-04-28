#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>

template <typename T>
class Matrix {
private:
    std::vector<T> content;
    size_t rows = 0, columns = 0;

public:
    Matrix(const std::vector<std::vector<T>> & origin) : rows(origin.size()) {
        if (rows)
            columns = origin[0].size();
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < columns; ++j)
                content.push_back(origin[i][j]);
        }
    }

    std::pair<size_t, size_t> size() const {
        return {rows, columns};
    }

    Matrix<T>& operator+=(const Matrix<T> x) {
        for (size_t i = 0; i < content.size(); ++i) {
            content[i] += x.content[i];
        }
        return *this;
    }

    Matrix<T> operator+(const Matrix<T> x) const {
        return Matrix<T>(*this) += x;
    }

    template<typename M>
    Matrix<T>& operator*=(const M& x) {
        for (size_t i = 0; i < content.size(); ++i) {
            content[i] *= x;
        }
        return *this;
    }

    template<typename M>
    Matrix<T> operator*(const M& x) const {
        Matrix ans(*this);
        ans *= x;
        return ans;
    }

    Matrix<T>& operator*=(const Matrix<T>& other) {
        assert(columns == other.rows);
        std::vector<T> temp(rows * other.columns);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < other.columns; ++j) {
                T d = content[columns * i] * other.content[j];
                for (size_t k = 1; k < columns; ++k) {
                    d += content[columns * i + k] * other.content[j + k * other.columns];
                }
                temp[other.columns * i + j] = d;
            }
        }
        content = temp;
        columns = other.columns;
        return *this;
    }

    Matrix<T> operator*(const Matrix<T>& other) const {
        Matrix ans(*this);
        ans *= other;
        return ans;
    }

    Matrix<T>& transpose() {
        std::vector<T> trnspsd(rows * columns);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < columns; ++j) {
                trnspsd[j * rows + i] = content[i * columns + j];
            }
        }
        auto t = rows;
        rows = columns;
        columns = t;
        content = trnspsd;
        return *this;
    }
    Matrix<T> transposed() const {
        return Matrix(*this).transpose();
    }
    typename std::vector<T>::iterator begin() {return content.begin();}
    typename std::vector<T>::iterator end() {return content.end();}
    typename std::vector<T>::const_iterator begin() const {return content.cbegin();}
    typename std::vector<T>::const_iterator end() const {return content.cend();}

    template<typename M> friend std::ostream& operator << (std::ostream& out, const Matrix<M>& m);
};
template <typename T>
std::ostream& operator << (std::ostream& out, const Matrix<T>& m) {
    size_t d = 0;
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.columns; ++j) {
            out << m.content[d];
            if (j != m.columns - 1)
                out << '\t';
            ++d;
        }
        if (i != m.rows - 1)
            out << '\n';
    }
    return out;
}
