
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    printf("hello world (pid:%d)\n", (int)getpid());
    pid_t rc = fork();
    if (rc < 0) {
        perror("fork failed");
        return 1;
    } else if (rc == 0) {
        printf("hello, I am child (pid:%d)\n", (int)getpid());
        return 0;
    } else {
        int status = 0;
        pid_t wc = waitpid(rc, &status, 0);
        if (wc < 0) perror("waitpid");
        printf("hello, I am parent of %d (wc:%d) (pid:%d)\n", rc, wc, (int)getpid());
    }
    return 0;
}
