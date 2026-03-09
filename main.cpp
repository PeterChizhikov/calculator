#include <iostream>
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


void parseValues(int argc,char** argv, mathInfo &info)
{
    int opt;
    char *endptr; //указатель на первый "не цифровой" символ
    while ((opt = getopt(argc, argv, "a:b:o:h")) != -1){

        switch (opt) {
            case 'a':
                info.firstNum = (int)strtol(optarg, &endptr, 10);
                if (*endptr != '\0' || optarg == endptr) {
                    fprintf(stderr, "Error: Argument for -a must be integer");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'b':
                info.secondNum = (int)strtol(optarg, &endptr, 10);
                if (*endptr != '\0' || optarg == endptr) {
                    fprintf(stderr, "Error: Argument for -b must be integer");
                    exit(EXIT_FAILURE);
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
                exit(EXIT_FAILURE);
        }
    }
}

void printValues(mathInfo &info)
{
    printf("Result: %d\n", info.result);
}

void calculateValues(mathInfo &info)
{
    int operationStatus = 0;
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
        exit(EXIT_FAILURE);
    }
    if (operationStatus == -2) {
        printf("Переполнение типа!\n");
        exit(EXIT_FAILURE);
    }
    if (operationStatus == -3) {
        printf("Недопустимая операция (например 0^0)!\n");
        exit(EXIT_FAILURE);
    }
}

void checkValues(mathInfo &info)
{
    if (info.operation == ' ') {
        fprintf(stderr, "Error: Operation (-o) is required \n");
        exit(EXIT_FAILURE);
    }

    if (info.operation != '+' && info.operation != '-' &&
        info.operation != '*' && info.operation != '/' &&
        info.operation != '^' && info.operation != '!') {
        fprintf(stderr, "Error: Invalid operation '%c'. Only +, -, *,^,!\n", info.operation);
        exit(EXIT_FAILURE);
    }

    if (info.operation == '/' && info.secondNum == 0) {
        fprintf(stderr, "Error: Division by zero is undefined.\n");
        exit(EXIT_FAILURE);
    }

}


void run(int argc,char** argv)
{
    mathInfo info;
    parseValues(argc, argv, info);
    checkValues(info);
    calculateValues(info);
    printValues(info);
    exit(EXIT_SUCCESS);
}

int main(int argc, char** argv)
{
    if(argc == 1){
       printHelp();	
    }
    run(argc, argv);
    return 0;
}


