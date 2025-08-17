//05_pipe_basic

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    int fd[2];
    if (pipe(fd) < 0) { perror("pipe"); return 1; }

    pid_t rc = fork();
    if (rc < 0) { perror("fork"); return 1; }

    if (rc == 0) {
        // Child: sender
        close(fd[0]); // close read-end
        pid_t cpid = getpid();
        printf("Child: Child PID: %d\n", (int)cpid);
        char buf[128];
        int n = snprintf(buf, sizeof(buf), "Hello from child PID: %d", (int)cpid);
        if (write(fd[1], buf, (size_t)n) < 0) perror("write");
        close(fd[1]);
        _exit(0);
    } else {
        // Parent: receiver
        close(fd[1]); // close write-end
        pid_t ppid = getpid();
        printf("Parent: Parent PID: %d\n", (int)ppid);
        char rbuf[256] = {0};
        ssize_t r = read(fd[0], rbuf, sizeof(rbuf)-1);
        if (r < 0) { perror("read"); }
        else {
            rbuf[r] = '\0';
            printf("Parent: %s\n", rbuf);
        }
        close(fd[0]);
        int st=0; waitpid(rc, &st, 0);
    }
    return 0;
}
