#include "artans.h"
#include "sorted_table.h"
#include "string.h"
#include <gtest.h>
#include "monom.h"
#include <stdexcept>


/// Тест для сложного выражения с возведением в степень
TEST(translator, ComplexExpressionWithExponentiation) {
    ArithmeticTranslator translator;
    std::string infix = "(4 ^ 2) * (2 + (3 / (2 + 2))) + ((4 * 5) / (2 + 3))";
    double expectedResult = 48;
    EXPECT_EQ(expectedResult, translator.getAnswer(infix));
}

// Тест для сложения
TEST(translator, AddSimple) {
    ArithmeticTranslator translator;
    std::string infix = "1+2";
    double expectedResult = 3;
    EXPECT_EQ(expectedResult, translator.getAnswer(infix));
}

// Тест для вычитания
TEST(translator, SubSimple) {
    ArithmeticTranslator translator;
    std::string infix = "2-1";
    double expectedResult = 1;
    EXPECT_EQ(expectedResult, translator.getAnswer(infix));
}

// Тест для умножения
TEST(translator, MulSimple) {
    ArithmeticTranslator translator;
    std::string infix = "3*5";
    double expectedResult = 15;
    EXPECT_EQ(expectedResult, translator.getAnswer(infix));
}

// Тест для деления
TEST(translator, DivSimple) {
    ArithmeticTranslator translator;
    std::string infix = "15/3";
    double expectedResult = 5;
    EXPECT_EQ(expectedResult, translator.getAnswer(infix));
}

// Тест на деление на 0
TEST(translator, DivisionByZeroThrowsError) {
    ArithmeticTranslator translator;
    std::string infix = "5/0";
    ASSERT_ANY_THROW(translator.getAnswer(infix));
}

// Тест на приоритет скобок
TEST(translator, BracketPriorityTest) {
    ArithmeticTranslator translator;
    std::string infix = "2*(3+4)";
    double expectedResult = 14;
    EXPECT_EQ(expectedResult, translator.getAnswer(infix));
}

// Тест с плавающими числами с нулем
TEST(translator, FloatingPointWithZero) {
    ArithmeticTranslator translator;
    std::string infix = "0.2+0.3";
    double expectedResult = 0.5;
    EXPECT_EQ(expectedResult, translator.getAnswer(infix));
}

// Тест с числами с плавающей точкой, где числа заканчиваются на точку
TEST(translator, FloatingPointWithTrailingDot) {
    ArithmeticTranslator translator;
    std::string infix = "2.+3.";
    double expectedResult = 5;
    EXPECT_EQ(expectedResult, translator.getAnswer(infix));
}

// Тест с обычными плавающими числами
TEST(translator, FloatingPointAddition) {
    ArithmeticTranslator translator;
    std::string infix = "2.2+0.3";
    double expectedResult = 2.5;
    EXPECT_EQ(expectedResult, translator.getAnswer(infix));
}

// Тест на неправильный порядок скобок (закрывающая скобка первой)
TEST(translator, InvalidBracketOrderClosingFirst) {
    ArithmeticTranslator translator;
    std::string infix = ")5+5(";
    ASSERT_ANY_THROW(translator.getAnswer(infix));
}

// Тест на разное количество открывающих и закрывающих скобок
TEST(translator, MismatchedBracketsCount) {
    ArithmeticTranslator translator;
    std::string infix = "((5+5)";
    ASSERT_ANY_THROW(translator.getAnswer(infix));
}
TEST(translator, SixConsecutiveOperations) {
    ArithmeticTranslator translator;

    
    double result1 = translator.getAnswer("5 + 3 - 2 * 4 - 67 + 102 * 100");     
    double result2 = translator.getAnswer("8 * 2 / 2 - 6 + 92 - 0.111");     
    double result3 = translator.getAnswer("6 ^ 2 - 13 + 4^2");    
    double result4 = translator.getAnswer("36 - 10^2 - 16.67 + 3/2");    
    double result5 = translator.getAnswer("7 ^ 2 - 156 -12 + 4^3");     
    double result6 = translator.getAnswer("12 - 23 / 2 + 9.7");  

    EXPECT_EQ(result1, 10133);     
    EXPECT_EQ(result2, 93.889);    
    EXPECT_EQ(result3, 39);
    EXPECT_EQ(result4, -79.17);
    EXPECT_EQ(result5, -55);
    EXPECT_EQ(result6, 10.2);
}
//тесты на унарный минус
TEST(translator, UnaryMinusBeforeNumber) {
    ArithmeticTranslator translator;
    std::string infix = "-5";
    double expectedResult = -5;
    EXPECT_EQ(expectedResult, translator.getAnswer(infix));
}


TEST(translator, UnaryMinusBeforeExpressionInBrackets) {
    ArithmeticTranslator translator;
    std::string infix = "-(3 + 4)";
    double expectedResult = -7;
    EXPECT_EQ(expectedResult, translator.getAnswer(infix));
}

TEST(translator, UnaryMinusWithMultiplication) {
    ArithmeticTranslator translator;
    std::string infix = "-2 * 3";
    double expectedResult = -6;
    EXPECT_EQ(expectedResult, translator.getAnswer(infix));
}
TEST(translator, UnaryMinusWithOperatorPriority) {
    ArithmeticTranslator translator;
    std::string infix = "-3 + 4 * 2";
    double expectedResult = 5; 
    EXPECT_EQ(expectedResult, translator.getAnswer(infix));
}
TEST(translator, UnaryMinusWithComplexFloatingPointExpression) {
    ArithmeticTranslator translator;
    std::string infix = "-(2.5 * (3.2 - 1.1) + 4.5)";
    double expectedResult = -(2.5 * 2.1 + 4.5);
    EXPECT_EQ(expectedResult, translator.getAnswer(infix));
}

TEST(POLINOM, SIMPLEMONOM) {
    ArithmeticTranslator translator;
    int key1 = translator.addPolinom("xyz^2");
    EXPECT_EQ(translator.getPolinom(key1), "1*(x^1)*1*(y^1)*1*(z^2)");
}

TEST(POLINOM, SIMPLEPOLINOM) {
    ArithmeticTranslator translator;
    int key1 = translator.addPolinom("xyz^2 + 2xy");
    EXPECT_EQ(translator.getPolinom(key1), "1*(x^1)*1*(y^1)*1*(z^2) + 2*(x^1)*1*(y^1)*1*(z^0)");
}

TEST(POLINOM, POLINOMWITHFLOATKOEF) {
    ArithmeticTranslator translator;
    int key1 = translator.addPolinom("xyz^2 + 2.1xy");
    EXPECT_EQ(translator.getPolinom(key1), "1*(x^1)*1*(y^1)*1*(z^2) + 2.1*(x^1)*1*(y^1)*1*(z^0)");
}


TEST(POLINOM, LONGPOLINOM) {
    ArithmeticTranslator translator;
    int key1 = translator.addPolinom("xyz^2 + 2.1xy +x^6y^9 - 84xy^2 - 1");
    EXPECT_EQ(translator.getPolinom(key1), "1*(x^1)*1*(y^1)*1*(z^2) + 2.1*(x^1)*1*(y^1)*1*(z^0) + 1*(x^6)*1*(y^9)*1*(z^0) + -84*(x^1)*1*(y^2)*1*(z^0) + -1*(x^0)*1*(y^0)*1*(z^0)");
}
TEST(POLINOM, UNARYMINUSMONOM) {
    ArithmeticTranslator translator;
    int key1 = translator.addPolinom("-2xyz^2");
    EXPECT_EQ(translator.getPolinom(key1), "-2*(x^1)*1*(y^1)*1*(z^2)");
}
TEST(POLINOM, CANMAKEAKEY) {
    ArithmeticTranslator translator;
    int key1 = translator.addPolinom("-2xyz^2");
    int key2 = translator.addPolinom("-2xyz");
    int n = 2;
    EXPECT_EQ(key2, n);
    EXPECT_EQ(key1, n-1);
}
TEST(POLINOM, CANTTHROWANEXPECTION) {
    
    ArithmeticTranslator translator;
    int key1 = translator.addPolinom("-2xyz^2");
    ASSERT_NO_THROW();
}




TEST(SortedTable, CanCreateEmptyTable) {
    SortedTable<int, std::string> table;
    ASSERT_EQ(table.begin(), table.end());
}

TEST(SortedTable, InsertIncreasesSize) {
    SortedTable<int, int> table;
    table.insert(1, 10);
    ASSERT_NE(table.begin(), table.end());
}

TEST(SortedTable, FindExistingElement) {
    SortedTable<int, std::string> table;
    table.insert(1, "one");
    auto it = table.find(1);
    ASSERT_NE(it, table.end());
    ASSERT_EQ(it->second, "one");
}

TEST(SortedTable, InsertThrowsWhenDuplicate) {
    SortedTable<std::string, int> table;
    table.insert("key", 10);
    ASSERT_THROW(table.insert("key", 20), const char*);
}

TEST(SortedTable, EraseRemovesElement) {
    SortedTable<int, double> table;
    table.insert(1, 3.14);
    table.erase(1);
    ASSERT_EQ(table.find(1), table.end());
}

TEST(SortedTable, EraseNonExistingThrows) {
    SortedTable<int, int> table;
    ASSERT_THROW(table.erase(2), const char*);
}

TEST(SortedTable, ElementsAreSortedAfterInsert) {
    SortedTable<int, char> table;
    table.insert(3, 'C');
    table.insert(1, 'A');
    table.insert(2, 'B');

    auto it = table.begin();
    ASSERT_EQ(it->first, 1);
    ASSERT_EQ(it->second, 'A');

    ++it;
    ASSERT_EQ(it->first, 2);
    ASSERT_EQ(it->second, 'B');

    ++it;
    ASSERT_EQ(it->first, 3);
    ASSERT_EQ(it->second, 'C');
}

TEST(SortedTable, IteratorIncrement) {
    SortedTable<int, int> table;
    table.insert(1, 10);
    table.insert(2, 20);

    auto it = table.begin();
    ASSERT_EQ(it->second, 10);

    auto copy = it++;
    ASSERT_EQ(copy->second, 10);
    ASSERT_EQ(it->second, 20);

    auto& ref = ++it;
    ASSERT_EQ(ref, table.end());
}

TEST(SortedTable, MaintainsSortOrderAfterComplexInserts) {
    SortedTable<int, int> table;
    table.insert(5, 50);
    table.insert(3, 30);
    table.insert(7, 70);
    table.insert(1, 10);
    table.insert(9, 90);

    auto it = table.begin();
    ASSERT_EQ(it++->first, 1);
    ASSERT_EQ(it++->first, 3);
    ASSERT_EQ(it++->first, 5);
    ASSERT_EQ(it++->first, 7);
    ASSERT_EQ(it++->first, 9);
    ASSERT_EQ(it, table.end());
}

TEST(SortedTable, CanUseRangeBasedForLoop) {
    SortedTable<int, int> table;
    table.insert(2, 20);
    table.insert(1, 10);
    table.insert(3, 30);

    int expected_key = 1;
    for (auto& pair : table) {
        ASSERT_EQ(pair.first, expected_key++);
    }
}

TEST(SortedTable, InsertReturnsCorrectIteratorPosition) {
    SortedTable<int, std::string> table;
    auto it1 = table.insert(3, "three");
    ASSERT_EQ(it1->first, 3);

    auto it2 = table.insert(1, "one");
    ASSERT_EQ(it2->first, 1);

    auto it3 = table.insert(5, "five");
    ASSERT_EQ(it3->first, 5);
}



TEST(Monom, ConstructorAndGetters) {
    Monom m(2.5, 3, 2, 1);
    EXPECT_DOUBLE_EQ(m.getCoefficient(), 2.5);
    EXPECT_EQ(m.getX(), 3);
    EXPECT_EQ(m.getY(), 2);
    EXPECT_EQ(m.getZ(), 1);
}

TEST(Monom, ToString) {
    Monom m(-1.5, 0, 4, 2);
    EXPECT_EQ(m.toString(), "-1.5*(x^0)*1*(y^4)*1*(z^2)");
}

TEST(Monom, ValidAddition) {
    Monom m1(2.0, 1, 2, 3);
    Monom m2(3.0, 1, 2, 3);
    Monom result = m1 + m2;
    EXPECT_DOUBLE_EQ(result.getCoefficient(), 5.0);
}

TEST(Monom, InvalidAddition) {
    Monom m1(1.0, 1, 1, 1);
    Monom m2(2.0, 2, 2, 2);
    EXPECT_THROW(m1 + m2, std::invalid_argument);
}

TEST(Monom, ValidSubtraction) {
    Monom m1(5.0, 2, 3, 4);
    Monom m2(2.0, 2, 3, 4);
    Monom result = m1 - m2;
    EXPECT_DOUBLE_EQ(result.getCoefficient(), 3.0);
}

TEST(Monom, Multiplication) {
    Monom m1(2.0, 1, 2, 3);
    Monom m2(3.0, 4, 5, 6);
    Monom result = m1 * m2;
    EXPECT_DOUBLE_EQ(result.getCoefficient(), 6.0);
    EXPECT_EQ(result.getX(), 5);
    EXPECT_EQ(result.getY(), 7);
    EXPECT_EQ(result.getZ(), 9);
}

TEST(Monom, DivisionValid) {
    Monom m1(6.0, 5, 5, 5);
    Monom m2(2.0, 2, 3, 4);
    Monom result = m1 / m2;
    EXPECT_DOUBLE_EQ(result.getCoefficient(), 3.0);
    EXPECT_EQ(result.getX(), 3);
    EXPECT_EQ(result.getY(), 2);
    EXPECT_EQ(result.getZ(), 1);
}

TEST(Monom, DivisionInvalidDegree) {
    Monom m1(1.0, 1, 1, 1);
    Monom m2(1.0, 2, 2, 2);
    EXPECT_THROW(m1 / m2, std::invalid_argument);
}

TEST(Monom, CompoundAddition) {
    Monom m(4.0, 2, 2, 2);
    m += Monom(1.0, 2, 2, 2);
    EXPECT_DOUBLE_EQ(m.getCoefficient(), 5.0);
}

TEST(Monom, ScalarMultiplication) {
    Monom m(2.0, 1, 1, 1);
    Monom result = m * 3.0;
    EXPECT_DOUBLE_EQ(result.getCoefficient(), 6.0);
    EXPECT_EQ(result.getX(), 1);
}

TEST(Monom, DivisionByZeroScalar) {
    Monom m(2.0, 1, 1, 1);
    EXPECT_THROW(m / 0.0, std::invalid_argument);
}

TEST(Monom, MultiplyAssignScalar) {
    Monom m(3.0, 2, 3, 4);
    m *= 2.0;
    EXPECT_DOUBLE_EQ(m.getCoefficient(), 6.0);
    EXPECT_EQ(m.getX(), 2);
}

TEST(Monom, ZeroCoefficientAfterOperations) {
    Monom m1(2.0, 3, 3, 3);
    Monom m2(-2.0, 3, 3, 3);
    Monom result = m1 + m2;
    EXPECT_DOUBLE_EQ(result.getCoefficient(), 0.0);
}

TEST(Monom, SelfAssignment) {
    Monom m(5.0, 1, 1, 1);
    m *= m;
    EXPECT_DOUBLE_EQ(m.getCoefficient(), 25.0);
    EXPECT_EQ(m.getX(), 2);
    EXPECT_EQ(m.getY(), 2);
    EXPECT_EQ(m.getZ(), 2);
}


TEST(Polinom, EmptyPolinomToString) {
    Polinom p;
    EXPECT_EQ(p.toString(), "");
}

TEST(Polinom, AddSingleMonom) {
    Polinom p;
    p.addMonom(Monom(2.5, 1, 2, 3));
    EXPECT_EQ(p.toString(), "2.5*(x^1)*1*(y^2)*1*(z^3)");
}

TEST(Polinom, AddZeroCoefficientMonom) {
    Polinom p;
    p.addMonom(Monom(0.0, 1, 1, 1));
    EXPECT_EQ(p.toString(), "");
}

TEST(Polinom, MergeSimilarMonoms) {
    Polinom p;
    p.addMonom(Monom(2.0, 1, 1, 1));
    p.addMonom(Monom(3.0, 1, 1, 1));
    EXPECT_EQ(p.toString(), "5*(x^1)*1*(y^1)*1*(z^1)");
}

TEST(Polinom, RemoveZeroResultAfterMerge) {
    Polinom p;
    p.addMonom(Monom(2.0, 2, 2, 2));
    p.addMonom(Monom(-2.0, 2, 2, 2));
    EXPECT_EQ(p.toString(), "");
}

TEST(Polinom, AddDifferentMonoms) {
    Polinom p;
    p.addMonom(Monom(1.0, 3, 2, 1));
    p.addMonom(Monom(2.0, 1, 4, 2));
    EXPECT_EQ(p.toString(), 
        "1*(x^3)*1*(y^2)*1*(z^1) + 2*(x^1)*1*(y^4)*1*(z^2)");
}

TEST(Polinom, PolinomAddition) {
    Polinom p1, p2;
    p1.addMonom(Monom(2.0, 1, 1, 1));
    p2.addMonom(Monom(3.0, 1, 1, 1));
    
    Polinom result = p1 + p2;
    EXPECT_EQ(result.toString(), "5*(x^1)*1*(y^1)*1*(z^1)");
}

TEST(Polinom, PolinomSubtraction) {
    Polinom p1, p2;
    p1.addMonom(Monom(5.0, 2, 2, 2));
    p2.addMonom(Monom(3.0, 2, 2, 2));
    
    Polinom result = p1 - p2;
    EXPECT_EQ(result.toString(), "2*(x^2)*1*(y^2)*1*(z^2)");
}

TEST(Polinom, PolinomMultiplication) {
    Polinom p1, p2;
    p1.addMonom(Monom(2.0, 1, 0, 0));
    p2.addMonom(Monom(3.0, 0, 1, 0));
    
    Polinom result = p1 * p2;
    EXPECT_EQ(result.toString(), "6*(x^1)*1*(y^1)*1*(z^0)");
}

TEST(Polinom, PolinomDivisionValid) {
    Polinom p1, p2;
    p1.addMonom(Monom(6.0, 3, 2, 1));
    p2.addMonom(Monom(2.0, 1, 1, 0));
    
    Polinom result = p1 / p2;
    EXPECT_EQ(result.toString(), "3*(x^2)*1*(y^1)*1*(z^1)");
}

TEST(Polinom, PolinomDivisionInvalid) {
    Polinom p1, p2;
    p2.addMonom(Monom(1.0, 1, 0, 0));
    p2.addMonom(Monom(1.0, 0, 1, 0));
    
    EXPECT_THROW(p1 / p2, std::invalid_argument);
}

TEST(Polinom, CompoundAdditionAssignment) {
    Polinom p1, p2;
    p1.addMonom(Monom(1.0, 1, 0, 0));
    p2.addMonom(Monom(2.0, 0, 1, 0));
    
    p1 += p2;
    EXPECT_EQ(p1.toString(), 
        "1*(x^1)*1*(y^0)*1*(z^0) + 2*(x^0)*1*(y^1)*1*(z^0)");
}

TEST(Polinom, MultiplyByScalarAndCleanup) {
    Polinom p;
    p.addMonom(Monom(2.0, 1, 0, 0));
    p.addMonom(Monom(3.0, 0, 1, 0));
    
    p *= 0.0;
    EXPECT_EQ(p.toString(), "");
}

TEST(Polinom, ScalarDivision) {
    Polinom p;
    p.addMonom(Monom(4.0, 2, 0, 1));
    p.addMonom(Monom(2.0, 1, 1, 0));
    
    Polinom result = p / 2.0;
    EXPECT_EQ(result.toString(), 
        "2*(x^2)*1*(y^0)*1*(z^1) + 1*(x^1)*1*(y^1)*1*(z^0)");
}

TEST(Polinom, FriendScalarMultiplication) {
    Polinom p;
    p.addMonom(Monom(1.5, 1, 0, 0));
    
    Polinom result = 2.0 * p;
    EXPECT_EQ(result.toString(), "3*(x^1)*1*(y^0)*1*(z^0)");
}

TEST(Polinom, DivisionByZeroScalar) {
    Polinom p;
    p.addMonom(Monom(1.0, 1, 1, 1));
    EXPECT_THROW(p / 0.0, std::invalid_argument);
}

TEST(Polinom, ComplexOperationChain) {
    Polinom p1, p2;
    p1.addMonom(Monom(2.0, 2, 0, 1));
    p2.addMonom(Monom(3.0, 1, 1, 0));
    
    Polinom result = (p1 + p2) * p1;
    EXPECT_EQ(result.toString(), 
        "4*(x^4)*1*(y^0)*1*(z^2) + 6*(x^3)*1*(y^1)*1*(z^1)");
}

TEST(Polinom, SelfAssignmentOperations) {
    Polinom p;
    p.addMonom(Monom(2.0, 1, 0, 0));
    
    p *= p;
    EXPECT_EQ(p.toString(), "4*(x^2)*1*(y^0)*1*(z^0)");
}

TEST(Polinom, DegreeOrderPreservation) {
    Polinom p;
    p.addMonom(Monom(1.0, 3, 2, 1));
    p.addMonom(Monom(1.0, 1, 4, 2));
    p.addMonom(Monom(1.0, 3, 2, 1)); // Should merge
    
    EXPECT_EQ(p.toString(), 
        "2*(x^3)*1*(y^2)*1*(z^1) + 1*(x^1)*1*(y^4)*1*(z^2)");
}
/*int key1 = translator.addPolinom("x^2 + 3y - 5z");
    int key2 = translator.addPolinom("2xy + 4z^3");
*/