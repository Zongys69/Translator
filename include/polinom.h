#ifndef __POLINOM_H__
#define __POLINOM_H__

#include <vector>
#include <string>
#include <sstream>
#include "monom.h"

class Polinom {
private:
    std::vector<Monom> monoms;

public:
    void addMonom(const Monom& monom) {
        if (monom.getCoefficient() == 0.0) {
            return;
        }
        bool found = false;
        for (auto it = monoms.begin(); it != monoms.end();) {
            if (it->getX() == monom.getX() &&
                it->getY() == monom.getY() &&
                it->getZ() == monom.getZ()) {
                double newCoeff = it->getCoefficient() + monom.getCoefficient();
                if (newCoeff == 0.0) {
                    it = monoms.erase(it);
                }
                else {
                    *it = Monom(newCoeff, it->getX(), it->getY(), it->getZ());
                    ++it;
                }
                found = true;
                break;
            }
            else {
                ++it;
            }
        }
        if (!found) {
            monoms.push_back(monom);
        }
    }

    std::string toString() const {
        if (monoms.empty()) {
            return "";
        }
        std::string result;
        for (size_t i = 0; i < monoms.size(); ++i) {
            if (i != 0) {
                result += " + ";
            }
            result += monoms[i].toString();
        }
        return result;
    }
    Polinom operator+(const Polinom& p) const {
        Polinom result = *this;
        for (const auto& m : p.monoms) {
            result.addMonom(m);
        }
        return result;
    }

    Polinom operator-(const Polinom& p) const {
        Polinom result = *this;
        for (const auto& m : p.monoms) {
            result.addMonom(Monom(-m.getCoefficient(), m.getX(), m.getY(), m.getZ()));
        }
        return result;
    }

    Polinom operator*(const Polinom& p) const {
        Polinom result;
        for (const auto& m1 : monoms) {
            for (const auto& m2 : p.monoms) {
                result.addMonom(m1 * m2);
            }
        }
        return result;
    }

    Polinom operator/(const Polinom& p) const {
        if (p.monoms.size() != 1) {
            throw std::invalid_argument("Can only divide by a single-monom polinom");
        }
        const Monom& divisor = p.monoms[0];
        Polinom result;
        for (const auto& m : monoms) {
            result.addMonom(m / divisor);
        }
        return result;
    }

    Polinom& operator+=(const Polinom& p) {
        for (const auto& m : p.monoms) {
            addMonom(m);
        }
        return *this;
    }

    Polinom& operator-=(const Polinom& p) {
        for (const auto& m : p.monoms) {
            addMonom(Monom(-m.getCoefficient(), m.getX(), m.getY(), m.getZ()));
        }
        return *this;
    }

    Polinom& operator*=(const Polinom& p) {
        Polinom temp;
        for (const auto& m1 : monoms) {
            for (const auto& m2 : p.monoms) {
                temp.addMonom(m1 * m2);
            }
        }
        monoms = temp.monoms;
        return *this;
    }

    Polinom& operator/=(const Polinom& p) {
        if (p.monoms.size() != 1) {
            throw std::invalid_argument("Can only divide by a single-monom polinom");
        }
        const Monom& divisor = p.monoms[0];
        Polinom temp;
        for (const auto& m : monoms) {
            temp.addMonom(m / divisor);
        }
        monoms = temp.monoms;
        return *this;
    }

    Polinom operator*(double c) const {
        Polinom result;
        for (const auto& m : monoms) {
            result.addMonom(m * c);
        }
        return result;
    }

    Polinom operator/(double c) const {
        if (c == 0) {
            throw std::invalid_argument("Division by zero");
        }
        Polinom result;
        for (const auto& m : monoms) {
            result.addMonom(m / c);
        }
        return result;
    }

    Polinom& operator*=(double c) {
        for (auto& m : monoms) {
            m = m * c;
        }
        
        auto it = monoms.begin();
        while (it != monoms.end()) {
            if (it->getCoefficient() == 0) {
                it = monoms.erase(it);
            }
            else {
                ++it;
            }
        }
        return *this;
    }

    Polinom& operator/=(double c) {
        if (c == 0) {
            throw std::invalid_argument("Division by zero");
        }
        for (auto& m : monoms) {
            m = m / c;
        }
        return *this;
    }
    
    friend Polinom operator*(double c, const Polinom& p) {
        return p * c;
    }
};

#endif // __POLINOM_H__
