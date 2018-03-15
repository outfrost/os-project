#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

sem_t stored_goods;
sem_t recycled_materials;

sem_t ui_update;

void start_environment();

#endif
