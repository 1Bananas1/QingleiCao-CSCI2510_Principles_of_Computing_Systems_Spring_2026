// Jimmy MacDonald
// February 26, 2026
// copy
// copy.c


#include <unistd.h>
#define bufferSize 200
char buffer[ bufferSize ];

int main() {
    ssize_t bytesRead;
    while (1) {
        bytesRead = read(STDIN_FILENO, buffer, bufferSize);
        if (bytesRead == 0) {
            break;
        }
        write(STDOUT_FILENO, buffer, bytesRead);

    }

}