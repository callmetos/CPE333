//03_child is dead while parent is still running

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    pid_t rc = fork();
    if (rc < 0) { perror("fork"); return 1; }
    if (rc == 0) {
        // Child exits quickly
        printf("[child] pid=%d exiting early\n", (int)getpid());
        return 0;
    } else {
        // Parent delays before waiting -> child may appear as <defunct> until we wait
        printf("[parent] pid=%d child=%d sleeping before wait\n", (int)getpid(), rc);
        sleep(5);
        printf("[parent] now wait() to reap child\n");
        int st=0; waitpid(rc, &st, 0);
        printf("[parent] child reaped, status=%d\n", st);
    }
    return 0;
}
