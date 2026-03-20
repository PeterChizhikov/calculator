#include "Calculator.h"
#include "Checker.h"
#include "MathInfo.h"
#include "Parser.h"
#include "Printer.h"
#include "Runner.h"
#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include <vector>

MathInfo createMathInfo(int first, int second, char operation) {
    MathInfo info;
    info.firstNum = first;
    info.secondNum = second;
    info.operation = operation;
    return info;
}

TEST(MathInfoTest, StoresAndReturnsAssignedValues) {
    MathInfo info;

    info.firstNum = 12;
    info.secondNum = -7;
    info.operation = '*';
    info.result = -84;

    EXPECT_EQ(info.firstNum, 12);
    EXPECT_EQ(info.secondNum, -7);
    EXPECT_EQ(info.operation, '*');
    EXPECT_EQ(info.result, -84);
}

TEST(ParserTest, ParsesJsonArgumentIntoMathInfo) {
    char programName[] = "calc";
    char jsonArg[] = "{\"firstNum\":7,\"secondNum\":3,\"operation\":\"*\"}";
    char *argv[] = {programName, jsonArg};

    std::streambuf *oldCout = std::cout.rdbuf();
    std::ostringstream fakeOut;
    std::cout.rdbuf(fakeOut.rdbuf());

    MathInfo info = Parser::parseValues(2, argv);

    std::cout.rdbuf(oldCout);

    EXPECT_EQ(info.firstNum, 7);
    EXPECT_EQ(info.secondNum, 3);
    EXPECT_EQ(info.operation, '*');
}

TEST(ParserTest, ThrowsWhenJsonArgumentIsMissing) {
    char programName[] = "calc";
    char *argv[] = {programName};

    std::streambuf *oldCout = std::cout.rdbuf();
    std::ostringstream fakeOut;
    std::cout.rdbuf(fakeOut.rdbuf());

    EXPECT_THROW(Parser::parseValues(1, argv), std::runtime_error);

    std::cout.rdbuf(oldCout);
}

TEST(ParserTest, ThrowsWhenJsonIsInvalid) {
    char programName[] = "calc";
    char badJson[] = "not-json";
    char *argv[] = {programName, badJson};

    std::streambuf *oldCout = std::cout.rdbuf();
    std::ostringstream fakeOut;
    std::cout.rdbuf(fakeOut.rdbuf());

    EXPECT_THROW(Parser::parseValues(2, argv), json::parse_error);

    std::cout.rdbuf(oldCout);
}

TEST(CheckerTest, AcceptsPlusOperation) {
    MathInfo info = createMathInfo(8, 2, '+');
    EXPECT_NO_THROW(Checker::checkValues(info));
}

TEST(CheckerTest, AcceptsMinusOperation) {
    MathInfo info = createMathInfo(8, 2, '-');
    EXPECT_NO_THROW(Checker::checkValues(info));
}

TEST(CheckerTest, AcceptsMultiplyOperation) {
    MathInfo info = createMathInfo(8, 2, '*');
    EXPECT_NO_THROW(Checker::checkValues(info));
}

TEST(CheckerTest, AcceptsDivideOperation) {
    MathInfo info = createMathInfo(8, 2, '/');
    EXPECT_NO_THROW(Checker::checkValues(info));
}

TEST(CheckerTest, AcceptsPowerOperation) {
    MathInfo info = createMathInfo(8, 2, '^');
    EXPECT_NO_THROW(Checker::checkValues(info));
}

TEST(CheckerTest, AcceptsFactorialOperation) {
    MathInfo info = createMathInfo(8, 2, '!');
    EXPECT_NO_THROW(Checker::checkValues(info));
}

TEST(CheckerTest, RejectsUnsupportedOperation) {
    MathInfo info = createMathInfo(8, 2, '?');
    EXPECT_THROW(Checker::checkValues(info), std::invalid_argument);
}

TEST(CheckerTest, RejectsDivisionByZero) {
    MathInfo info = createMathInfo(8, 0, '/');
    EXPECT_THROW(Checker::checkValues(info), std::runtime_error);
}

TEST(CalculatorTest, CalculatesSum) {
    MathInfo info = createMathInfo(7, 3, '+');

    ASSERT_NO_THROW(Calculator::calculateValues(info));
    EXPECT_EQ(info.result, 10);
}

TEST(CalculatorTest, CalculatesSubtraction) {
    MathInfo info = createMathInfo(7, 3, '-');

    ASSERT_NO_THROW(Calculator::calculateValues(info));
    EXPECT_EQ(info.result, 4);
}

TEST(CalculatorTest, CalculatesMultiplication) {
    MathInfo info = createMathInfo(7, 3, '*');

    ASSERT_NO_THROW(Calculator::calculateValues(info));
    EXPECT_EQ(info.result, 21);
}

TEST(CalculatorTest, CalculatesDivision) {
    MathInfo info = createMathInfo(8, 2, '/');

    ASSERT_NO_THROW(Calculator::calculateValues(info));
    EXPECT_EQ(info.result, 4);
}

TEST(CalculatorTest, CalculatesPower) {
    MathInfo info = createMathInfo(2, 5, '^');

    ASSERT_NO_THROW(Calculator::calculateValues(info));
    EXPECT_EQ(info.result, 32);
}

TEST(CalculatorTest, CalculatesFactorial) {
    MathInfo info = createMathInfo(5, 0, '!');

    ASSERT_NO_THROW(Calculator::calculateValues(info));
    EXPECT_EQ(info.result, 120);
}

TEST(CalculatorTest, ThrowsForUnsupportedOperation) {
    MathInfo info = createMathInfo(5, 2, '?');
    EXPECT_THROW(Calculator::calculateValues(info), std::runtime_error);
}

TEST(PrinterTest, PrintsResultLine) {
    MathInfo info;
    info.result = 42;

    std::streambuf *oldCout = std::cout.rdbuf();
    std::ostringstream fakeOut;
    std::cout.rdbuf(fakeOut.rdbuf());

    Printer::printValues(info);

    std::cout.rdbuf(oldCout);

    EXPECT_EQ(fakeOut.str(), "Result: 42\n");
}

TEST(RunnerTest, PrintsCalculatedResult) {
    char programName[] = "calc";
    char jsonArg[] = "{\"firstNum\":9,\"secondNum\":4,\"operation\":\"-\"}";
    char *argv[] = {programName, jsonArg};

    std::streambuf *oldCout = std::cout.rdbuf();
    std::streambuf *oldCerr = std::cerr.rdbuf();
    std::ostringstream fakeOut;
    std::ostringstream fakeErr;
    std::cout.rdbuf(fakeOut.rdbuf());
    std::cerr.rdbuf(fakeErr.rdbuf());

    Runner runner(2, argv);
    runner.run();

    std::cout.rdbuf(oldCout);
    std::cerr.rdbuf(oldCerr);

    EXPECT_NE(fakeOut.str().find("Result: 5"), std::string::npos);
    EXPECT_TRUE(fakeErr.str().empty());
}

TEST(RunnerTest, CatchesDivisionByZeroError) {
    char programName[] = "calc";
    char jsonArg[] = "{\"firstNum\":9,\"secondNum\":0,\"operation\":\"/\"}";
    char *argv[] = {programName, jsonArg};

    Runner runner(2, argv);

    EXPECT_NO_THROW(runner.run());
}

TEST(RunnerTest, CatchesInvalidOperationError) {
    char programName[] = "calc";
    char jsonArg[] = "{\"firstNum\":9,\"secondNum\":4,\"operation\":\"\"}";
    char *argv[] = {programName, jsonArg};

    Runner runner(2, argv);

    EXPECT_NO_THROW(runner.run());
}