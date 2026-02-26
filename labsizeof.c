#include <stdio.h>

int main(){
    char myChar;
    int myInt;
    double myDouble;
    float myFloat;

    printf("%zu\n", sizeof(myChar));
    printf("%zu\n", sizeof(myInt));
    printf("%zu\n", sizeof(myDouble));
    printf("%zu\n", sizeof(myFloat));

    return 0;
}