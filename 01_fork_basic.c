//01_fork_basic

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(void) {
    printf("hello world (pid:%d)\n", (int)getpid());
    pid_t rc = fork();
    if (rc < 0) {
        perror("fork failed");
        return 1;
    } else if (rc == 0) {
        printf("hello, I am child (pid:%d)\n", (int)getpid());
    } else {
        printf("hello, I am parent of %d (pid:%d)\n", rc, (int)getpid());
    }
    return 0;
}
