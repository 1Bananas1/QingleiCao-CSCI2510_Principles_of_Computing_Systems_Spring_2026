#include <stdio.h>

char* reverseString( char* input ){

    //1. First count how many characters are in the input string
    int len = 0;
    while (*(input + len) != '\0') {
        len++;
    }
    //This creates enough space to store the reversed string, plus one more byte
    //for the null terminator
    char* output = (char*)malloc( len+1 );

    //2. Copy the input string to the output string in reverse order. There are
    //multiple ways to do this- consider using a counter, or consider using two
    //pointers.
    for (int i = 0; i < len; i++) {
        output[i] = *(input + (len - 1 - i));
    }

    //REMEMBER THAT YOUR OUTPUT STRING MUST END WITH A NULL TERMINATOR. This is not
    //provided for you automatically- you must put it there!
    output[len] = '\0';

    return output; 
}

int main() {
    char *messagePtr = "HELLOWORLD!";
    // * marks that we are working with a pointer
    // messagePtr: Variable Name
    // "HELLOWORLD!" Actual text

    printf("%s\n", messagePtr);

    for (int i=0; messagePtr[i] != '\0'; i++) {
        printf("%c",messagePtr[i]);
        printf("\n");
    } 
    printf("%c\n", *messagePtr);

    printf("%c\n", *(messagePtr + 1));

    
    for (int i = 0; *(messagePtr + i) != '\0'; i++) {
        printf("%c\n", *(messagePtr + i));
    }
    for (int i = 0; *(messagePtr + i) != '\0'; i++) {
        printf("%c\n", *(messagePtr + i));
    }

    
    int len = 0;
    while (*(messagePtr + len) != '\0') {
        len++;
    }
    for (int i = len - 1; i >= 0; i--) {
        printf("%c\n", *(messagePtr + i));
    }

    char* reversedMessage = reverseString( messagePtr );
    printf("Reversed string: %s\n", reversedMessage);
    return 0;
}