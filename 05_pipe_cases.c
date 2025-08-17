//05_pipe_cases

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

static void usage(const char* prog) {
    fprintf(stderr, "Usage: %s [readback|earlyread|burst]\n", prog);
}

int main(int argc, char** argv) {
    if (argc != 2) { usage(argv[0]); return 1; }
    const char* mode = argv[1];
    int fd[2];
    if (pipe(fd) < 0) { perror("pipe"); return 1; }

    pid_t rc = fork();
    if (rc < 0) { perror("fork"); return 1; }

    if (rc == 0) {
        // Child: by default as sender, but may try to readback in "readback" mode
        if (strcmp(mode, "readback") == 0) {
            // Intentionally keep both ends open to demonstrate readback (NOT best practice)
            pid_t cpid = getpid();
            const char* msg = "child-msg";
            write(fd[1], msg, strlen(msg));
            char buf[64] = {0};
            ssize_t n = read(fd[0], buf, sizeof(buf)-1);
            printf("[child %d] readback got: '%s' (n=%zd)\n", (int)cpid, buf, n);
        } else if (strcmp(mode, "earlyread") == 0) {
            // Receiver reads before sender sends: child will be receiver; parent will send later
            close(fd[1]); // child = receiver
            char buf[64] = {0};
            printf("[child %d] reading before parent writes ... (will block)\n", (int)getpid());
            ssize_t n = read(fd[0], buf, sizeof(buf)-1);
            printf("[child %d] read returned n=%zd, buf='%s'\n", (int)getpid(), n, buf);
            close(fd[0]);
        } else if (strcmp(mode, "burst") == 0) {
            // Sender sends several messages before receiver reads
            close(fd[0]); // child = sender
            for (int i=0;i<5;i++) {
                char buf[64];
                int n = snprintf(buf, sizeof(buf), "msg-%d\n", i);
                if (write(fd[1], buf, (size_t)n) < 0) { perror("write"); break; }
            }
            close(fd[1]);
        } else {
            usage(argv[0]); _exit(2);
        }
        _exit(0);
    } else {
        if (strcmp(mode, "readback") == 0) {
            // Parent: do nothing special; close both ends to let child proceed
            close(fd[0]); close(fd[1]);
        } else if (strcmp(mode, "earlyread") == 0) {
            // Parent: sender writes after a delay
            close(fd[0]); // parent = sender
            sleep(2);
            const char* msg = "late-parent-msg";
            write(fd[1], msg, strlen(msg));
            close(fd[1]);
        } else if (strcmp(mode, "burst") == 0) {
            // Parent: receiver reads after child sends a burst
            close(fd[1]); // parent = receiver
            char buf[256] = {0};
            ssize_t n = read(fd[0], buf, sizeof(buf)-1);
            printf("[parent %d] got (n=%zd):\n%s", (int)getpid(), n, buf);
            close(fd[0]);
        }
        int st=0; waitpid(rc, &st, 0);
    }
    return 0;
}
