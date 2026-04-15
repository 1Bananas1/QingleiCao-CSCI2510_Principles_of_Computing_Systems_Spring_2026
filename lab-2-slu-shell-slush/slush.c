#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <limits.h>
#include <errno.h>

#define MAX_LINE 256
#define MAX_ARGS  16
#define MAX_CMDS  16



//SLUSH - The SLU SHELL
//
//This assignment reads user input, and executes that input as a sequence of
//programs to execute.
//
//Program execution commands have the form:
//
//prog_n [args] ( ... prog_3 [args] ( prog_2 [args] ( prog_1 [args]
//
//For example, "grep slush ( sort -r ( ls -l -a" returns a reverse sorted list
//of all files in the current directory that contain the string "slush" in
//
//See the lab writeup for more details and other requirements.

//we just learned race conditions today so give me a break i looked up a lot of things
volatile sig_atomic_t got_sigint = 0; 
// a type qualifier used to inform the compiler 
// that a variable's value may change at any time 
// due to external factors outside the program's immediate control
// thank you wikipedia

void sigint_handler(int sig) {
    got_sigint = 1;
}

// Parses a single command string "prog arg1 arg2 etc" into argv array
// returns number of args or -1 on error
int parse_command(char *cmd_str, char *argv[]) {
    int argc = 0;
    char *token = strtok(cmd_str, " \t");
    while (token != NULL && argc < MAX_ARGS - 1) {
        argv[argc++] = token;
        token = strtok(NULL, " \t");
    }
    argv[argc] = NULL;
    return argc;
}

void run_pipeline(char *cmds[], int cmd_count, int write_fd) {
    // base check
    if (cmd_count == 0) {
        return;
    }

    char *argv[MAX_ARGS];

    // copy of strtok
    char cmd_copy[MAX_LINE];
    strncpy(cmd_copy, cmds[cmd_count - 1], MAX_LINE - 1);
    cmd_copy[MAX_LINE - 1] = '\0';

    int argc = parse_command(cmd_copy, argv);
    if(argc == 0) {
        fprintf(stderr, "Inalid null command\n");
        return;
    }

    int pipefd[2];
    int has_pipe = (cmd_count > 1);

    if (has_pipe) {
        if (pipe(pipefd) < 0) {
            perror("pipe");
            return;
        }
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return;
    }

    if (pid == 0) {
        // child process

        // If there's a next stage read from pipe read end
        if (has_pipe) {
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            close(pipefd[1]);
        }

        // if caller gave us a write end (write to the next stage)
        if (write_fd != -1) {
            dup2(write_fd, STDOUT_FILENO);
            close(write_fd);
        }

        // restore default sigint in child
        signal(SIGINT, SIG_DFL);

        execvp(argv[0], argv);
        // only reached on error
        fprintf(stderr, "%s: Not found\n", argv[0]);
        exit(1);
    }

    //parent: close write fd since child has it now
    if (write_fd != -1) {
        close(write_fd);
    }
    if (has_pipe) {
        close(pipefd[0]);
        run_pipeline(cmds, cmd_count -1, pipefd[1]);
    }
    int status;
    while (waitpid(pid, &status, 0) < 0 && errno == EINTR)
        ;

}

void parse_and_execute(char *input) {
    char *cmds[MAX_CMDS];
    int cmd_count = 0;

    char input_copy[MAX_LINE];
    strncpy(input_copy, input, MAX_LINE -1);
    input_copy[MAX_LINE - 1] = '\0';

    char *token = strtok(input_copy, "(");
    while (token != NULL && cmd_count < MAX_CMDS) {
        // Trim leading/trailing whitespace
        while (*token == ' ' || *token == '\t') token++;
        char *end = token + strlen(token) - 1;
        while (end > token && (*end == ' ' || *end == '\t')) *end-- = '\0';

        if (strlen(token) == 0) {
            fprintf(stderr, "Invalid null command\n");
            return;
        }

        cmds[cmd_count++] = token;
        token = strtok(NULL, "(");
    }

    if (cmd_count == 0) {
        fprintf(stderr, "Invalid null command\n");
        return;
    }

    run_pipeline(cmds, cmd_count, -1);
}

int main() {
    char cmd[MAX_LINE];

    // set up sigint handler
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0; // dont restart fgets on SIGINT
    sigaction(SIGINT, &sa, NULL);

    printf("SLUSH - the SLU Shell\n");

    while (1) {
        // Build prompt with relative path from $HOME
        char cwd[PATH_MAX];
        char *home = getenv("HOME");
        char prompt_path[PATH_MAX] = "";

        if (getcwd(cwd, sizeof(cwd)) != NULL && home != NULL) {
            size_t home_len = strlen(home);
            if (strncmp(cwd, home, home_len) == 0) {
                // cwd is inside HOME — show only the relative part
                char *rel = cwd + home_len;
                if (*rel == '/') rel++; // skip leading slash
                strncpy(prompt_path, rel, sizeof(prompt_path) - 1);
            } else {
                // cwd is outside HOME — show full path
                strncpy(prompt_path, cwd, sizeof(prompt_path) - 1);
            }
        }

        if (strlen(prompt_path) == 0)
            printf("slush> ");
        else
            printf("slush|%s> ", prompt_path);

        fflush(stdout);

        got_sigint = 0;

        if (fgets(cmd, sizeof(cmd), stdin) == NULL) {
            if (got_sigint) {
                printf("\n");
                continue; // ^C during input: new prompt
            }
            // EOF
            printf("\n");
            break;
        }

        // strip newline
        cmd[strcspn(cmd, "\n")] = '\0';

        if (strlen(cmd) == 0) continue;

        // built-in: cd
        if (strncmp(cmd, "cd ", 3) == 0) {
            char *dir = cmd + 3;
            while (*dir == ' ') dir++;
            if (chdir(dir) < 0) {
                perror(dir);
            }
            continue;
        }

        parse_and_execute(cmd);
    }

    return 0;
}