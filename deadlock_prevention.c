#include <stdio.h>
#include <pthread.h>

pthread_mutex_t lock1;
pthread_mutex_t lock2;

void *worker(void *arg)
{
    pthread_mutex_lock(&lock1);
    pthread_mutex_lock(&lock2);

    printf("Resources acquired successfully.\n");

    pthread_mutex_unlock(&lock2);
    pthread_mutex_unlock(&lock1);

    return NULL;
}

int main()
{
    pthread_t t1, t2;

    pthread_mutex_init(&lock1, NULL);
    pthread_mutex_init(&lock2, NULL);

    pthread_create(&t1, NULL, worker, NULL);
    pthread_create(&t2, NULL, worker, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&lock1);
    pthread_mutex_destroy(&lock2);

    return 0;
}
