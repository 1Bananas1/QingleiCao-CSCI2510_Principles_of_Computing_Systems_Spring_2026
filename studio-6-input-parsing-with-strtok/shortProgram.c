#include "stdio.h"
#include "string.h"
#include "unistd.h"

int main() {
    char buff[100];
    int n = 100;
    int max_args = 15;
    int max_argv_size = max_args + 2; //one for argv[0], one for null terminator
    char* cmd;
    char* my_argv[max_argv_size];
    printf("Enter a string: \n");
    if (fgets(buff, sizeof(buff), stdin) != NULL) {
        buff[strcspn(buff, "\n\r")] = '\0';
    }
    const char s[4] = " ";
    char* tok;

    tok = strtok(buff, s);
    cmd = tok;
    my_argv[0] = cmd;
    int i = 1;
    tok = strtok(0, s);
    while (tok != NULL) {
        my_argv[i] = tok;
        i++;
        tok = strtok(0, s);
    }
    my_argv[i] = '\0';
    // printf("%s\n", buff);
    execvp(cmd, my_argv);
    return 0;
}