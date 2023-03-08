#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define PHILOSOPHERS 5


pthread_mutex_t mutex;
pthread_cond_t cond[PHILOSOPHERS]; /* one per philosopher */
pthread_mutex_t print_lock;   /* protects against output interleaving */



typedef struct {
    int num_loops;
    int thread_id;
} arg_t;


enum states{THINKING, EATING, HUNGRY} state[PHILOSOPHERS]; /* state of each philosopher */

void space(int s) {
    pthread_mutex_lock(&print_lock);
    int i;
    for (i = 0; i < s * 10; i++)
	printf(" ");
}

int left(int p)  {
    return (p + PHILOSOPHERS -1) % PHILOSOPHERS;
}

int right(int p) {
    return (p + 1) % PHILOSOPHERS;
}

void think() {
    return;
}

void eat() {
    return;
}

	
int update_state(int p){
  if (state[p] == HUNGRY && state[left(p)] != EATING && state[right(p)] != EATING) {
     state[p] = EATING;
     pthread_cond_signal(&cond[p]);
  }
  return 0;
}

void put_forks(int p){
    pthread_mutex_lock(&mutex);    

   /*
   Code for changing state to THINKING
   */

    pthread_mutex_unlock(&mutex);
}

void get_forks(int p){
   pthread_mutex_lock (&mutex);  /* enter cs, lock mutex */
  

   /*
   Code for then change state to EATING and wait while HUNGRY
   */
 
   pthread_mutex_unlock(&mutex); /* exit cs, unlock mutex */
}

void *philosopher(void *arg) {
    arg_t *args = (arg_t *) arg;

    space(args->thread_id); printf("%d: start\n", args->thread_id);  pthread_mutex_unlock(&print_lock);

    int i;
    for (i = 0; i < args->num_loops; i++) {
        space(args->thread_id); printf("%d: think\n", args->thread_id); pthread_mutex_unlock(&print_lock);
        think();
        get_forks(args->thread_id);
        space(args->thread_id); printf("%d: eat\n", args->thread_id); pthread_mutex_unlock(&print_lock);
        eat();
        put_forks(args->thread_id);
       space(args->thread_id); printf("%d: done\n", args->thread_id); pthread_mutex_unlock(&print_lock);
    }
    return NULL;
}

int main(int argc, char *argv[]) {

    if (argc != 2) {
	fprintf(stderr, "usage: dining_philosophers <num_loops>\n");
	exit(1);
    }
    printf("dining: started\n");
    
    int i;

    /*
    initialize mutex and condition variables 
    */


    pthread_t p[PHILOSOPHERS];
    arg_t a[PHILOSOPHERS];
    for (i = 0; i < PHILOSOPHERS; i++) {
        a[i].num_loops = atoi(argv[1]);
        a[i].thread_id = i;
        pthread_create(&p[i], NULL, philosopher, &a[i]);
        state[i] = THINKING;
        }

    for (i = 0; i < PHILOSOPHERS; i++) 
	    pthread_join(p[i], NULL); 

    printf("dining: finished\n");
    return 0;
}
