//Runlength compression in C
// Usage: ./rle <input file> <output file> <runlength> <mode>
//        Where mode=0 is compress and mode=1 is decompress

#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) { 
    // Check for valid inputs

    // Amount of input check
    if (argc != 5) {
        perror("Usage: .rle <input> <output> <runlength> <mode> "); 
        exit(-1);
    }

    // Mode check
    int mode_input = atoi(argv[4]);
    if (mode_input != 0 && mode_input != 1) {
        perror("Error: Mode must be 0 (Compress) or 1 (Decompress).");
        exit(-1);
    }


    long rle_value = atol(argv[3]); // Convert string to long to check bounds
    if (rle_value <= 0 || rle_value > 255) {
        perror("Error: Runtime must be between length 1 and 255.");
        exit(-1);
    }
    int compression_length = (int)rle_value;

    // Validate input file
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

    // Validate output file
    int fd_out = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR); 

    if (fd_out == -1) {
        if (errno == EACCES) {
            perror("Error: No permission to write to the output file.");
        } else if (errno == EISDIR) {
            perror("Error: Output path is a directory.");
        } else if (errno == EROFS) {
            perror("Error: The file system is read-only.");
        } else {
            perror("Error: Could not create or open output file.");
        }
        exit(-1);
    }

    
    // Actual Main Execution

    // Compress Mode
    if (mode_input == 0) {
        unsigned char current[compression_length];
        unsigned char next[compression_length];

        ssize_t bytes_read = read(fd_in, current, compression_length);
        if (bytes_read == -1) {
            perror("Error reading input file");
            exit(-1);
        }

        while (bytes_read > 0) {
            int current_len = (int)bytes_read; 
            unsigned char count = 1;

            ssize_t next_read = read(fd_in, next, compression_length);
            if (next_read == -1) {
                perror("Error reading input file");
                exit(-1);
            }

            while (next_read == bytes_read &&
                memcmp(current, next, current_len) == 0 &&
                count < 255) {
                count++;
                next_read = read(fd_in,next, compression_length);
                if (next_read == -1) {
                    perror("Error reading input file");
                    exit(-1);
                }
            }

            if (write(fd_out, &count, 1) == -1) {
                perror("Error writing output file");
                exit(-1);
            }

            if (write(fd_out, current, current_len) == -1) {
                perror("Error writing output file");
                exit(-1); // i'm really getting tired of writing this same error
            }

            bytes_read = next_read;
            memcpy(current, next, next_read);
        }
    } else { 
        // Decompress 
        unsigned char count_buf[1];
        unsigned char pattern[compression_length];

        ssize_t bytes_read;

        while ((bytes_read = read(fd_in, count_buf, 1)) > 0) {
            unsigned char count = count_buf[0];

            ssize_t pat_read = read(fd_in, pattern, compression_length);
            if (pat_read == -1) {
                perror("Error reading input file"); //man...
                exit(-1);
            }

            if (pat_read == 0) { // when there is no pattern
                break;
            }

            for (int i = 0; i < count; i++) {
                if (write(fd_out,pattern, pat_read) == -1) {
                    perror("Error writing output file");
                    exit(-1);
                }
            }
        }

        if (bytes_read == -1) {
            perror("Error reading input file");
            exit(-1);
        }
    }
        
    // Close the files
    if (close(fd_in) == -1) {
        perror("Error closing input file");
        exit(-1);
    } 
    if (close(fd_out) == -1) {
        perror("Error closing output file");
        exit(-1);
    } 
    return 0;
}