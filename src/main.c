#include <stdio.h>
#include <ctype.h>


int main() {
    printf("FormulaGuard Compiler\n");

    const char *formula = "=A1+B2*10";

    while(*formula!='\0'){

        if(isalpha(*formula)){
            printf("Cell: ");
            while(isalpha(*formula)){
                printf("%c",*formula);
                formula++;
            }
            while(isdigit(*formula)){
                printf("%c",*formula);
                formula++;
            }
            printf("\n");
        }

        else if(isdigit(*formula)){
            printf("Number: ");
            while(isdigit(*formula)){
                printf("%c",*formula);
                formula++;
            }
            printf("\n");
        }
        else if(*formula=='/'||*formula=='*'||*formula=='+'||*formula=='-'){
            printf("Operator: %c",*formula);
            formula++;
            printf("\n");
        }
        else if (*formula == '=') {
            printf("Equal: %c\n", *formula);
            formula++;
            printf("\n");
        }

        else{
            printf("Error in expression");
            formula++;
            printf("\n");
        }
    }
    return 0;
}
