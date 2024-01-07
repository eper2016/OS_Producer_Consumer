#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 5

typedef struct {
  int buffer[BUFFER_SIZE];
  int in, out;
  sem_t empty, full, mutex;
  int insert_item, read_item, count;
} buf;

void *producer_1(void *arg);
void *consumer_1(void *arg);
void *insert_item(void *arg);
void *remove_item(void *arg);

int main(int arg, char *argv[]) {
  buf *m;
  m = (buf *)malloc(sizeof(buf));

  printf("Main thread beginning\n");

  // get arguments from the command line
  int sTime = atoi(argv[1]);
  int num_prod = atoi(argv[2]);
  int num_cons = atoi(argv[3]);

  // initializing semaphores
  sem_init(&m->empty, 0, BUFFER_SIZE);
  sem_init(&m->full, 0, 0);
  sem_init(&m->mutex, 0, 1);
  m->in = 0;
  m->out = 0;

  // initializing the array of b
  for (int i = 0; i < BUFFER_SIZE; i++) {
    m->buffer[i] = -1;
  }

  // declare threads
  pthread_t pro_thread, con_thread;

  // create producer threads
  for (int p = 0; p < num_prod; p++) {
    pthread_create(&pro_thread, NULL, producer_1, (void *)m);
    printf("Creating producer thread with ID %lu\n", pro_thread);
  }

  // create consumer threads
  for (int c = 0; c < num_cons; c++) {
    pthread_create(&con_thread, NULL, consumer_1, (void *)m);
    printf("Creating consumer thread with ID %lu\n", con_thread);
  }

  // main thread sleeps now
  sleep(sTime);

  // after the main thread sleeps, exit and semaphores will be destroy then
  printf("Main thread exiting\n");
  exit(0);
}

void *insert_item(void *arg) {

  buf *ins = (buf *)arg;

  // Decrement empty and wait if 0
  // Decrement mutex to enter critical section

  sem_wait(&ins->empty);
  sem_wait(&ins->mutex);

  while (1) {
    while (ins->count == BUFFER_SIZE) { // do nothing
    }

    ins->buffer[ins->in] = ins->insert_item;

    printf("Insert_item inserted item %d at position %d \n", ins->insert_item, ins->in);

    ins->in++;
    ins->in = (ins->in) % BUFFER_SIZE;
    ins->count++;

    for (int i = 0; i < BUFFER_SIZE; i++) {
        if (ins->buffer[i] == -1)
        {
            printf("[Empty] ");
        }
        else
        {
            printf("[%d] ", ins->buffer[i]);
        }
    }
    printf("\t\tin = %d, out = %d", ins->in, ins->out);
    printf("\n");

    break;
  }
  // Exit critical section first
  // Increment full

  sem_post(&ins->mutex);
  sem_post(&ins->full);
  return (void *)ins;
}

void *remove_item(void *arg) {

  buf *rem = (buf *)arg;

  // Decrement mutex to enter critical section

  sem_wait(&rem->full);
  sem_wait(&rem->mutex);

  while (1) {
    while (rem->count == 0) {
    }
    rem->read_item = rem->buffer[rem->out];

    rem->buffer[rem->out] = -1;
    printf("Remove_item removed item %d from position %d \n", rem->read_item, rem->out);

    rem->out++;
    rem->out = (rem->out) % BUFFER_SIZE;
    rem->count--;

    for (int i = 0; i < BUFFER_SIZE; i++) {

        if (rem->buffer[i] == -1)
        {
            printf("[Empty] ");
        }
        else if (rem->buffer[i] == rem->read_item)
        {
            printf("[Empty] ");
        }
        else
        {
            printf("[%d] ", rem->buffer[i]);
        }
    }
    printf("\t\tin = %d, out = %d", rem->in, rem->out);

    printf("\n");

    break;
  }

  // signal the empty and mutex semaphore

  sem_post(&rem->empty);
  sem_post(&rem->mutex);
  return (void *)rem;
}

void *producer_1(void *arg) {

  buf *pro = (buf *)arg;
  while (1) {

    // generate random number from 1 to 4
    int r = rand() % 4 + 1;

    printf("Producer thread %ld sleeping for %d seconds\n", pthread_self(), r);

    // sleep for random time
    sleep(r);

    // generate random number from 1 to 50
    int r2 = rand() % 50 + 1;

    // set the value of item to the generated random value
    pro->insert_item = r2;

    // call the function and pass in the set of arguments
    insert_item(pro);

    printf("Producer thread %ld inserted value %d\n", pthread_self(), pro->insert_item);
  }
  return (void *)pro;
}
void *consumer_1(void *arg) {

  buf *con = (buf *)arg;

  while (1) {

    // generate random number from 1 to 4
    int r = rand() % 4 + 1;

    printf("Consumer thread %ld sleeping for %d seconds\n", pthread_self(), r);

    // sleep for random time
    sleep(r);

    // remove item from the b
    remove_item(con);

    printf("Consumer thread %ld removed value %d\n", pthread_self(), con->read_item);

  }
  return (void *)con;
}
