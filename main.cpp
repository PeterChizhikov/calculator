#include <getopt.h>
#include <cstdio>
#include "mathFunctions.h"

struct mathInfo{
    int firstNum=0;
    int secondNum=0;
    char operation=' ';
    int result=0;

};

void printHelp()
{
    printf("Usage: cals [options]\n");
    printf("Options:\n");
    printf("  -a <num>    Set the first number\n");
    printf("  -b <num>    Set the second number\n");
    printf("  -o <char>   Set the operation (+, -, '*', /, '^','!')\n");
    printf("  If you use -o ! (factorial, only a is need) print -b 0 to avoid error\n");
    printf("  -h          Show this help message\n");
    printf("Example: calc -a 4 -b 8 -o '*'\n");

}

int stringToInt(const char* str, bool &error)
{
    int val = 0;
    
    if (!str || *str == '\0') {
        error = true;
        return 0;
    }

    if (sscanf(str, "%d", &val) != 1) {
        error = true;
        return 0;
    }
    
    error = false;
    return (int)val;
}

void parseValues(int argc,char** argv, mathInfo &info)
{
    int opt;
    bool parseError = false;
    while ((opt = getopt(argc, argv, "a:b:o:h")) != -1){

        switch (opt) {
            case 'a':
                info.firstNum = stringToInt(optarg, parseError);
                if (parseError) {
                    fprintf(stderr, "Error: Argument for -a must be integer");
                    return;
                }
                break;
            case 'b':
                info.secondNum = stringToInt(optarg, parseError);
                if (parseError) {
                    fprintf(stderr, "Error: Argument for -b must be integer");
                    return;
                }
                break;
            case 'o':
                info.operation = optarg[0];
                break;
            case 'h':
                printHelp();
                break;
            case '?':
                fprintf(stderr, "Error: Print -h for help\n");
            default:
                printHelp();
                return;
        }
    }
}

void printValues(mathInfo &info)
{
    printf("Result: %d\n", info.result);
}

void calculateValues(mathInfo &info)
{
    int operationStatus = -1;
    if (info.operation == '+') {
        operationStatus = mathOperations::sum(info.firstNum, info.secondNum, info.result);
    } else if (info.operation == '-') {
        operationStatus = mathOperations::subtract(info.firstNum, info.secondNum, info.result);
    } else if (info.operation == '*') {
        operationStatus = mathOperations::multiply(info.firstNum, info.secondNum, info.result);
    } else if (info.operation == '/') {
        operationStatus = mathOperations::divide(info.firstNum, info.secondNum, info.result);
    } else if (info.operation == '!') {
        operationStatus = mathOperations::factorial(info.firstNum, info.result);
    } else if (info.operation == '^') {
        operationStatus = mathOperations::pow(info.firstNum, info.secondNum, info.result);
    } else {
        operationStatus = -1;
    }

    if (operationStatus == 0) {
        printf("Операция выполнена успешно!\n");
        return;
    }
    if (operationStatus == -1) {
        printf("Ошибка в операции!\n");
       return;
    }
    if (operationStatus == -2) {
        printf("Переполнение типа!\n");
       return;
    }
    if (operationStatus == -3) {
        printf("Недопустимая операция (например 0^0)!\n");
        return;
    }
}

void checkValues(mathInfo &info)
{
    if (info.operation == ' ') {
        fprintf(stderr, "Error: Operation (-o) is required \n");
        return;
    }

    if (info.operation != '+' && info.operation != '-' &&
        info.operation != '*' && info.operation != '/' &&
        info.operation != '^' && info.operation != '!') {
        fprintf(stderr, "Error: Invalid operation '%c'. Only +, -, *,^,!\n", info.operation);
        return;
    }

    if (info.operation == '/' && info.secondNum == 0) {
        fprintf(stderr, "Error: Division by zero is undefined.\n");
        return;
    }

}


void run(int argc,char** argv)
{
    mathInfo info;
    parseValues(argc, argv, info);
    checkValues(info);
    calculateValues(info);
    printValues(info);
    return;
}

int main(int argc, char** argv)
{
    if(argc == 1){
       printHelp();	
    }
    run(argc, argv);
    return 0;
}


