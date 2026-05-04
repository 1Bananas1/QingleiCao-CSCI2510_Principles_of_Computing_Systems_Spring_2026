#include <stdio.h>
#include <pthread.h>

int race = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *adder(void *arg) {
    for (int i = 0; i < 2000000; i++) {
        pthread_mutex_lock(&mutex);
        race++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *subtractor(void *arg) {
    for (int i = 0; i < 2000000; i++) {
        pthread_mutex_lock(&mutex);
        race--;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, adder, NULL);
    pthread_create(&t2, NULL, subtractor, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("race = %d\n", race);
    return 0;
}
