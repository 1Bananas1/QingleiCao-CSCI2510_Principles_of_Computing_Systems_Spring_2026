1. ok
2. ok
3. fprintf requires a FILE \*stream argument as it's first parameter, while printf writes to stdout
4. fprintf requires a FILE \*stream argument as it's first parameter, while printf writes to stdout
5. ok
6. Section 2
7. #include <unistd.h>
8.
9.

```c
// Jimmy MacDonald
// February 19, 2026
// fprintf
// hello_fprintf.c

#include <unistd.h>

int main( int argc, char* argv[] ){
    write(STDOUT_FILENO, "why does c not have std:: but c++ does \n", 41);
    return 0;
}
```

10.

```linux
why does c not have std:: but c++ does
0����d
      ��������,���L����zRx
                         ����&D$4���� FJ
j                                       �?9*3$"\x���tp����Q���3E�C
```

I'm not really sure what I was expecting to be honest. It might be the memory register contents?

11. In my opinion you'd want to use write when you are trying to highly optimize runtime and you know that this will be running on linux. When we want to bypass c as a middleman
