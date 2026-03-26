#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#define bufferSize 1024
char buffer[ bufferSize ];


int main(int argc, char *argv[]) {
    if (argc != 2) {
        perror("Too many arguments");
        exit(-1);
    }

    int fd_in = open(argv[1], O_RDONLY);
    if (fd_in == -1) {
        if (errno == ENOENT) {
            perror("File does not exist.");
        } else if (errno == EACCES) {
            perror("No permission to read file.");
        } else if (errno == EISDIR) {
            perror("Path is directory, not a file.");
        } else {
            perror("Path is invalid or another error occured.");
        }
        exit(-1);
    }

    ssize_t bytesRead;

    while ((bytesRead = read (fd_in, buffer, bufferSize)) > 0) {
        ssize_t bytes_written = write(STDIN_FILENO, buffer, bytesRead);
        if (bytes_written == 1) {
            perror("Write");
            close(fd_in);
            exit(-1);
        }
    }
    close(fd_in);
}