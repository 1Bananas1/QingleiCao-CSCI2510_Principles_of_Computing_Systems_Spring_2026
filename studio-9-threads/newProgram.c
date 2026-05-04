#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 5

struct thread_data {
    int arg1;
    char arg2[100];
    int ret;
};

void* thread_entry(void* args) {
    struct thread_data* arg_ptr = (struct thread_data*) args;
    
    printf("Thread ID %d executing: %s\n", arg_ptr->arg1, arg_ptr->arg2);
    arg_ptr->ret = arg_ptr->arg1 * 10; 
    
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    struct thread_data args[NUM_THREADS];
    unsigned bufSize = 100;
    
    for (int i = 0; i < NUM_THREADS; i++) {
        args[i].arg1 = i;
        snprintf(args[i].arg2, bufSize, "This is thread %d", i);
        
        int ret = pthread_create(&threads[i], NULL, thread_entry, &args[i]);
        if (ret != 0) {
            fprintf(stderr, "Error creating thread %d\n", i);
            return 1;
        }
    }
    
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
        printf("Main joined thread %d, returned: %d\n", i, args[i].ret);
    }
    
    return 0;
}