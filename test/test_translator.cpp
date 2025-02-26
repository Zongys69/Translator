#include "artans.h"
#include <gtest.h>



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

TEST(TranslatorTest, SimplePolynomial) {
    ArithmeticTranslator translator;
    
    EXPECT_DOUBLE_EQ(5.0, translator.getAnswer("2x + 3", 1.0));
}

TEST(TranslatorTest, PolynomialWithPower) {
    ArithmeticTranslator translator;
    
    EXPECT_NEAR(0.0, translator.getAnswer("x^2 -4x +4", 2.0), 1e-9);
}

TEST(TranslatorTest, PolynomialWithFractions) {
    ArithmeticTranslator translator;
    
    EXPECT_NEAR(32.2, translator.getAnswer("3.5x^3 +2.1x", 2.0), 1e-9);
}

TEST(TranslatorTest, NegativeXValue) {
    ArithmeticTranslator translator;
    
    EXPECT_DOUBLE_EQ(-9.0, translator.getAnswer("-x^2 +5x -3", -1.0));
}

TEST(TranslatorTest, PolynomialWithZeroCoefficient) {
    ArithmeticTranslator translator;
    
    EXPECT_DOUBLE_EQ(5.0, translator.getAnswer("0x^3 +4x -7", 3.0));
}

TEST(TranslatorTest, ImplicitMultiplication) {
    ArithmeticTranslator translator;
    
    EXPECT_DOUBLE_EQ(24.0, translator.getAnswer("2x(x+1)", 3.0));
}

TEST(TranslatorTest, CombinedOperations) {
    ArithmeticTranslator translator;
    
    EXPECT_DOUBLE_EQ(5.0, translator.getAnswer("(x^2 +1)/(x-1)", 2.0));
}

TEST(TranslatorTest, HighDegreePolynomial) {
    ArithmeticTranslator translator;
    
    EXPECT_DOUBLE_EQ(18.0, translator.getAnswer("x^5 -2x^3 +x", 2.0));
}

TEST(TranslatorTest, ZeroXValue) {
    ArithmeticTranslator translator;
   
    EXPECT_DOUBLE_EQ(10.0, translator.getAnswer("5x^4 -2x +10", 0.0));
}

TEST(TranslatorTest, ComplexPolynomialExpression) {
    ArithmeticTranslator translator;
    
    EXPECT_DOUBLE_EQ(27.0, translator.getAnswer("2(x+3)^2 -5x", 1.0));
}