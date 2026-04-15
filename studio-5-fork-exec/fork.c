#include <stdio.h> // for printf
#include <sys/types.h> // for getpid
#include <unistd.h> // for getpid, sleep
#include <sys/wait.h> // for waitpid

int main() {
    char* child_argv[] = {"ls", "-l", NULL};
    char* child_prog = child_argv[0];

    int ret = fork();
    if ( ret == 0) { // child
        execvp( child_prog, child_argv );
    }
    if ( ret > 0) { // parent
        waitpid( ret, NULL, 0 );
    }
    pid_t pid = getpid();
    printf("The process ID of this program is: %d\n", pid);
    return 0;
}