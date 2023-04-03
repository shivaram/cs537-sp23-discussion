#include <stdio.h>
#include <sys/time.h>
#include <stdint.h>

#define DIM 1000

long matrix_a[DIM][DIM];
long matrix_b[DIM][DIM];
long matrix_c[DIM][DIM];

uint64_t get_current_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    uint64_t result = tv.tv_sec*(uint64_t)1000000 + tv.tv_usec;
    return result;
} 

void init() {
    for(int i = 0; i < DIM; i++) {
        for(int j = 0; j < DIM; j++) {
            matrix_a[i][j] = i+j;
            matrix_b[i][j] = i-j;
            matrix_c[i][j] = 0;
        }
    }
}

void multiply() {
    for(int i = 0; i < DIM; i++) {
        for(int j = 0; j < DIM; j++) {
            for(int k = 0; k < DIM; k++) {
                matrix_c[i][j] += matrix_a[i][k] * matrix_b[k][j];
            }
        }
    }
}

void print() {
    FILE *fp = fopen("serial.txt","w");
    for(int i = 0; i < DIM; i++) {
        for(int j = 0; j < DIM; j++) {
            fprintf(fp, "%ld\n", matrix_c[i][j]);
        }
    }
    fclose(fp);
}

int main(int argc, char **argv){
    uint64_t start, end;
    start = get_current_time();
    init();
    end = get_current_time();
    printf("Initialization time: %lu\n", end-start);
    start = get_current_time();
    multiply();
    end = get_current_time();
    printf("Multiplication time: %lu\n", end-start);
    start = get_current_time();
    print();
    end = get_current_time();
    printf("Print time: %lu\n", end-start);
    return 0;
}
