//Parents are dead while child is running.

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(void) {
    pid_t rc = fork();
    if (rc < 0) { perror("fork"); return 1; }
    if (rc == 0) {
        // Child outlives parent a bit -> becomes orphan and gets reparented to PID 1
        printf("[child] pid=%d started; sleeping while parent may exit\n", (int)getpid());
        sleep(6);
        printf("[child] pid=%d still alive after parent exit (check my PPID with ps)\n", (int)getpid());
        return 0;
    } else {
        printf("[parent] pid=%d exiting early; child=%d will be orphaned\n", (int)getpid(), rc);
        _exit(0); // exit immediately, no wait
    }
}
