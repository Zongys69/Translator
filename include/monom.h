#ifndef __MONOM_H__
#define __MONOM_H__

#include <string>
#include <sstream>

class Monom {
private:
    double coefficient;
    int x_degree;
    int y_degree;
    int z_degree;

public:
    Monom(double coeff, int x, int y, int z)
        : coefficient(coeff), x_degree(x), y_degree(y), z_degree(z) {}

    std::string toString() const {
        std::ostringstream oss;
        oss << coefficient << "*(x^" << x_degree << ")*1*(y^" << y_degree << ")*1*(z^" << z_degree << ")";
        return oss.str();
    }

    double getCoefficient() const { return coefficient; }
    int getX() const { return x_degree; }
    int getY() const { return y_degree; }
    int getZ() const { return z_degree; }

    Monom operator+(const Monom& m) const {
        if (x_degree != m.x_degree || y_degree != m.y_degree || z_degree != m.z_degree)
            throw std::invalid_argument("Degrees do not match for addition");
        return Monom(coefficient + m.coefficient, x_degree, y_degree, z_degree);
    }

    Monom operator-(const Monom& m) const {
        if (x_degree != m.x_degree || y_degree != m.y_degree || z_degree != m.z_degree)
            throw std::invalid_argument("Degrees do not match for subtraction");
        return Monom(coefficient - m.coefficient, x_degree, y_degree, z_degree);
    }

    Monom operator*(const Monom& m) const {
        return Monom(coefficient * m.coefficient,
            x_degree + m.x_degree,
            y_degree + m.y_degree,
            z_degree + m.z_degree);
    }

    Monom operator/(const Monom& m) const {
        if (m.coefficient == 0)
            throw std::invalid_argument("Division by zero monom");
        if (x_degree < m.x_degree || y_degree < m.y_degree || z_degree < m.z_degree)
            throw std::invalid_argument("Degree in divisor is higher than in dividend");
        return Monom(coefficient / m.coefficient,
            x_degree - m.x_degree,
            y_degree - m.y_degree,
            z_degree - m.z_degree);
    }

    Monom& operator+=(const Monom& m) {
        if (x_degree != m.x_degree || y_degree != m.y_degree || z_degree != m.z_degree)
            throw std::invalid_argument("Degrees do not match for addition");
        coefficient += m.coefficient;
        return *this;
    }

    Monom& operator-=(const Monom& m) {
        if (x_degree != m.x_degree || y_degree != m.y_degree || z_degree != m.z_degree)
            throw std::invalid_argument("Degrees do not match for subtraction");
        coefficient -= m.coefficient;
        return *this;
    }

    Monom& operator*=(const Monom& m) {
        coefficient *= m.coefficient;
        x_degree += m.x_degree;
        y_degree += m.y_degree;
        z_degree += m.z_degree;
        return *this;
    }

    Monom& operator/=(const Monom& m) {
        if (m.coefficient == 0)
            throw std::invalid_argument("Division by zero monom");
        if (x_degree < m.x_degree || y_degree < m.y_degree || z_degree < m.z_degree)
            throw std::invalid_argument("Degree in divisor is higher than in dividend");
        coefficient /= m.coefficient;
        x_degree -= m.x_degree;
        y_degree -= m.y_degree;
        z_degree -= m.z_degree;
        return *this;
    }

    Monom operator*(double c) const {
        return Monom(coefficient * c, x_degree, y_degree, z_degree);
    }

    Monom operator/(double c) const {
        if (c == 0)
            throw std::invalid_argument("Division by zero");
        return Monom(coefficient / c, x_degree, y_degree, z_degree);
    }

    Monom& operator*=(double c) {
        coefficient *= c;
        return *this;
    }

    Monom& operator/=(double c) {
        if (c == 0)
            throw std::invalid_argument("Division by zero");
        coefficient /= c;
        return *this;
    }
};

#endif // __MONOM_H__
