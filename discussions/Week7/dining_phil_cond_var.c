#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define PHILOSOPHERS 5


pthread_mutex_t mutex;
pthread_cond_t cond[PHILOSOPHERS];
pthread_mutex_t print_lock;   /* protects against output interleaving */



typedef struct {
    int num_loops;
    int thread_id;
} arg_t;


void get_forks(int p);
void put_forks(int p);

enum states{THINKING, EATING, HUNGRY} state[PHILOSOPHERS];

int left(int p)  {
    return p;
}

int right(int p) {
    return (p + 1) % 5;
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

    state[p] = THINKING;   

    update_state(left(p));  /* update_state neighbors */
    update_state(right(p));

    pthread_mutex_unlock(&mutex);
}

void get_forks(int p){
   pthread_mutex_lock (&mutex);  /* enter cs, lock mutex */
   state[p] = HUNGRY;  /* set philosopher's state to HUNGRY */
   update_state(p); /* update_state philosopher */
   while (state[p] == HUNGRY){      /* loop while philosopher is hungry */
     pthread_cond_wait(&cond[p],&mutex);
   }
   pthread_mutex_unlock(&mutex); /* exit cs, unlock mutex */
}

void *philosopher(void *arg) {
    arg_t *args = (arg_t *) arg;

    pthread_mutex_lock(&print_lock); printf("%d: start\n", args->thread_id);  pthread_mutex_unlock(&print_lock);

    int i;
    for (i = 0; i < args->num_loops; i++) {
        pthread_mutex_lock(&print_lock); printf("%d: think\n", args->thread_id); pthread_mutex_unlock(&print_lock);
    think();
    get_forks(args->thread_id);
        pthread_mutex_lock(&print_lock); printf("%d: eat\n", args->thread_id); pthread_mutex_unlock(&print_lock);
    eat();
    put_forks(args->thread_id);
        pthread_mutex_lock(&print_lock); printf("%d: done\n", args->thread_id); pthread_mutex_unlock(&print_lock);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
	fprintf(stderr, "usage: dining_philosophers <num_loops>\n");
	exit(1);
    }
    printf("dining: started\n");

    pthread_mutex_init (&mutex, NULL);
    pthread_mutex_init (&print_lock, NULL);
    int i;
    for (i = 0; i < 5; i++) 
	    pthread_mutex_init(&cond[i], NULL);
    //Sem_init(&print_lock, 1);

    pthread_t p[5];
    arg_t a[5];
    for (i = 0; i < 5; i++) {
        a[i].num_loops = atoi(argv[1]);
        a[i].thread_id = i;
        pthread_create(&p[i], NULL, philosopher, &a[i]);
        state[i] = THINKING;
        }

    for (i = 0; i < 5; i++) 
	    pthread_join(p[i], NULL); 

    printf("dining: finished\n");
    return 0;
}
