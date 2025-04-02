#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

const char *read_input(char *buffer, size_t buffer_size) {
  static const char* argv[] = { "dmenu", "-P", "-p", "Authentication is required", NULL };
  int pipes[2];

  if (pipe(pipes) == -1) {
    fprintf(stderr, "failed to create pipes\n");
  }
  int pipe_read = pipes[0];
  int pipe_write = pipes[1];

  int child_pid = fork();
  if (child_pid == -1) {
    fprintf(stderr, "failed to fork\n");
    close(pipe_read);
    close(pipe_write);
    return NULL;
  }

  if (child_pid == 0) {
    // CHILD PROCESS
    dup2(pipe_write, STDOUT_FILENO);
    close(pipe_read);
    close(pipe_write);

    if (execvp(argv[0], (void *)argv) == -1) {
      fprintf(stderr, "failed to launch program: %s\n", argv[0]);
      exit(1);
    }

    exit(0);
  } else {
    // PARENT PROCESS
    close(pipe_write);

    ssize_t len = read(pipe_read, buffer, buffer_size - 1);
    if (len == -1) {
      fprintf(stderr, "failed to read stdout from child process\n");
      close(pipe_read);
      wait(NULL);
      return NULL;
    } else {
      if (len != 0) {
        // terminate string at newline
        buffer[len - 1] = 0;
      } else {
        // terminate string
        buffer[0] = 0;
      }
      close(pipe_read);
      wait(NULL);
      return buffer;
    }
  }
}
