#include <iostream>
#include <getopt.h>
#include <cstdio>


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
    printf("  -o <char>   Set the operation (+, -, *, /, ^,!)\n");
    printf("  If you use -o ! (factorial, only a is need) print -b 0 to avoid error");
    printf("  -h          Show this help message\n");

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


}

void calculateValues(mathInfo &info)
{


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
        fprintf(stderr, "Error: Invalid operation '%c'. Only +, -, *,^,!", info.operation);
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
}


