## Contents
- `01_fork_basic.c` — fork() without wait()
- `02_fork_wait.c` — fork() with wait()
- `03_sleep_child_first.c` — child exits first (may become zombie until parent waits)
- `03_sleep_parent_first.c` — parent exits first (child becomes orphan; reparented to PID 1)
- `04_fork_limit.c` — count how many forks succeed (safe mode; adjustable)
- `05_pipe_basic.c` — pipe() + fork(): child sends, parent receives
- `05_pipe_cases.c` — pipe() scenarios: sender-readback, receiver-read-before-send, burst writes