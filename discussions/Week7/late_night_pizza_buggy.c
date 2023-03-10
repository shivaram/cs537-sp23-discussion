
/*
This implementation will keep producing and consuming pizza. 
Exercise: Modify the solution to run it for n iterations.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


pthread_cond_t order = PTHREAD_COND_INITIALIZER;
pthread_cond_t deliver =  PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void study(){
    printf("Students are studying\n");
}

void prepare_pizza(){
    printf("Pizza is being prepared\n");
}

//shared variable
volatile int slices = 0;

int S = 2;
volatile int first = 1; 

void *pizza_delivery(){
    while(1){        
        pthread_mutex_lock(&mutex);

        //wait for the order
        pthread_cond_wait(&order, &mutex);
        prepare_pizza();
        slices = S;
        first = 0;

        //wake up all the students when order is ready
        pthread_cond_broadcast(&deliver);
        pthread_mutex_unlock(&mutex);
    }
}

void *student(){
    while(1){
        pthread_mutex_lock(&mutex);  

        /*
        If pizza is not available, order and wait for delivery
        */
        while(slices == 0){
            if(first){
		first = 1;
                pthread_cond_signal(&order);
            }
            pthread_cond_wait(&deliver, &mutex);
        }

        /*
        If pizza is  available, eat and study
        */
        slices--;
        pthread_mutex_unlock(&mutex);
        study();
    }
}

int main(int argc, char * argv[]){
    pthread_t s, d;
    pthread_create(&s, NULL, student, NULL);
    pthread_create(&d, NULL, pizza_delivery, NULL);

    pthread_join(s, NULL);
    pthread_join(d, NULL);

    return 0;
}
