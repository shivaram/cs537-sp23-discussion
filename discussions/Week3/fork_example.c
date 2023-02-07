#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
    printf("Starting from %d\n", getpid());
    int rc = fork();
    char  *myargs[3] = {"/bin/ls", 
                        "-l", 
                        NULL };

    if(rc == 0){
        //inside child process
        printf("Exiting from child, my id is %d and rc is %d\n", getpid(), rc);
        sleep(20);
        int exec_rc = execv(myargs[0], myargs);
        printf("Done with execv");
        
    }
    else{
        //parent process executes here
        //sleep(1);
        //use RC to wait for child process
        // int wait_rc = wait(NULL);
        int status;
        int wait_rc = waitpid(rc, &status, 0);
        printf("Exiting from parent, my id is %d and rc is %d\n", getpid(), rc);
    }
    return 0;
}
