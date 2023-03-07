/*Late-Night Pizza. A group of students is studying for a CS537 exam. The students can study only while eating pizza. 
Each student executes the following loop: while (true) { pick up a piece of pizza; study while eating the pizza}. 
If a student finds that the pizza is gone, the student goes to sleep until another pizza arrives. 
The first student to discover that the group is out of pizza phones Satisfactions at Brightleaf to order another pizza before going to sleep. Each pizza has S slices.
Write code to synchronize the student threads and the pizza delivery thread. 
Your solution should avoid deadlock and phone Satisfactions (i.e., wake up the delivery thread) exactly once each time a pizza is exhausted.
 No piece of pizza may be consumed by more than one student.*/


/*
This implementation will keep producing and consuming pizza. 
Modify the solution to run it for 5 loops.
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

//shared variable

int slices = 0;
int pizza_ordered = 0; 
int S = 1;
int first = 1, have_pizza = 0;

pthread_cond_t order = PTHREAD_COND_INITIALIZER;
pthread_cond_t deliver =  PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void study(){
    printf("Students are studying\n");
}

void prepare_pizza(){
    printf("Pizza is being prepared\n");
}


void *pizza_delivery(){
    while(1){
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&order, &mutex);
        prepare_pizza();
        slices = S;
        first = 1;
        pthread_cond_broadcast(&deliver);
        pthread_mutex_unlock(&mutex);
    }
}

void *student(){
    while(1){
        pthread_mutex_lock(&mutex);       
        while(!have_pizza){
            if(slices > 0){
                slices --;
                have_pizza = 1;
            }
            else{
                if(first){
                    pthread_cond_signal(&order);
                    pizza_ordered = 0;
                }
                pthread_cond_wait(&deliver, &mutex);
            }
        }
    
        pthread_mutex_unlock(&mutex);
        study();
        have_pizza = 0;
    }
}

int main(int argc, char * argv[]){
    pthread_t s, d;
    pthread_create(&s, NULL, student, NULL);
    pthread_create(&d, NULL, pizza_delivery, NULL);

    pthread_join(d, NULL);
    pthread_join(s, NULL);

    return 0;
}
