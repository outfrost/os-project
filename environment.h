#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

/* const int SUPPLIERS_COUNT */
#define SUPPLIERS_COUNT 5
/* const int CONSUMERS_COUNT */
#define CONSUMERS_COUNT 5

pthread_t suppliers[SUPPLIERS_COUNT];
pthread_t consumers[CONSUMERS_COUNT];

WorkerState supplier_state[SUPPLIERS_COUNT];
WorkerState consumer_state[CONSUMERS_COUNT];

/* const unsigned int INITIAL_ITEMS */
#define INITIAL_ITEMS 10u

sem_t stored_goods;
sem_t recycled_materials;

sem_t ui_update;

void start_environment();
void terminate_environment();

#endif
