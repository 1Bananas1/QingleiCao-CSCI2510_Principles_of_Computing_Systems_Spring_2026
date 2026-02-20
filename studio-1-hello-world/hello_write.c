// Jimmy MacDonald
// February 19, 2026
// fprintf 
// hello_fprintf.c

#include <unistd.h>

int main( int argc, char* argv[] ){
    write(STDOUT_FILENO, "why does c not have std:: but c++ does \n", 41);
    return 0;
}