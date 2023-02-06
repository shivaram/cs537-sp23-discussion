
/**
 * Executes the command "cat scores | grep bob".  In this 
 * implementation the parent doesn't wait for the child to finish and
 * so the command prompt may reappear before the child terminates.
 */
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
int main(int argc, char *argv[]){
  int pipefd[2];


  char *cat_args[] = {"/bin/cat", "scores", NULL};
  char *grep_args[] = {"/bin/grep", "bob", NULL};

  // make a pipe (fds go in pipefd[0] and pipefd[1])
  pipe(pipefd);

  int pid = fork();

  if (pid == 0){
      // child gets here and handles "grep bob"
      // replace standard input with input part of pipe
      dup2(pipefd[0], 0);

      close(pipefd[1]);

      // execute grep
      execv(grep_args[0], grep_args);
    }
  else{
      // parent gets here and handles "cat scores"
      // replace standard output with output part of pipe
      dup2(pipefd[1], 1);
      // close unused innput half of pipe
      //wait(NULL);
     
      close(pipefd[0]);

      // execute cat
      execv(cat_args[0], cat_args);
      perror(cat_args[0]);
 
    }
    return 0;
}