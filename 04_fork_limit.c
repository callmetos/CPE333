//04_fork_limit

#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

static long get_env_long(const char* name, long defv) {
    char* s = getenv(name);
    if (!s || !*s) return defv;
    char* end=NULL;
    long v = strtol(s, &end, 10);
    if (end==s) return defv;
    return v;
}

int main(void) {
    long max_forks = get_env_long("MAX_FORKS", 500);
    long sleep_us  = get_env_long("SLEEP_US", 500);
    long count = 0;

    pid_t parent = getpid();
    while (count < max_forks) {
        pid_t rc = fork();
        if (rc < 0) {
            perror("fork failed (likely hit limit)");
            break;
        } else if (rc == 0) {
            // Child path: increment and continue chain
            count++;
            if (sleep_us > 0) usleep(sleep_us);
            continue;
        } else {
            // Parent waits for its immediate child then exits loop
            int st=0;
            waitpid(rc, &st, 0);
            printf("[parent %d] last successful forks (in chain) ~ %ld\n", (int)parent, count);
            return 0;
        }
    }
    // If we get here, we are deepest child or hit limit
    printf("[pid %d] final count=%ld (MAX_FORKS=%ld)\n", (int)getpid(), count, max_forks);
    return 0;
}
