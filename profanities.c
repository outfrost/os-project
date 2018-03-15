#include <stdlib.h>
#include <ncurses.h>
#include <semaphore.h>

#include "environment.h"

void* run_profanities(void* arg) {
	initscr();
	cbreak();
	noecho();
	nodelay(stdscr, true);
	
	while (1) {
		sem_wait(&ui_update);
		
		int goods;
		int materials;
		sem_getvalue(&stored_goods, &goods);
		sem_getvalue(&recycled_materials, &materials);
		
		int rows;
		int columns;
 		getmaxyx(stdscr, rows, columns);
		
		mvprintw((rows >> 1) - 2, (columns >> 1) - 1, "%d", goods);
		mvprintw((rows >> 1) + 2, (columns >> 1) - 1, "%d", materials);
		
		refresh();
		
		if (getch() != ERR) {
			endwin();
			
			int* result = malloc(sizeof(int));
			*result = 0;
			return (void*)result;
		}
	}
	
}
