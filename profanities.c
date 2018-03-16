#include <stdlib.h>
#include <ncurses.h>
#include <semaphore.h>

#include "typedefs.h"
#include "environment.h"

void start_curses() {
	initscr();
	cbreak();
	noecho();
	curs_set(0);
	nodelay(stdscr, true);
}

void terminate_curses() {
	endwin();
}

void* profanities_run(void* arg) {
	start_curses();
	
	while (1) {
		sem_wait(&ui_update);
		
		int rows;
		int columns;
 		getmaxyx(stdscr, rows, columns);
		
		int goods;
		int materials;
		sem_getvalue(&stored_goods, &goods);
		sem_getvalue(&recycled_materials, &materials);
		
		
		mvprintw((rows >> 1) - 2, (columns >> 1) - 1, "%d", goods);
		mvprintw((rows >> 1) + 2, (columns >> 1) - 1, "%d", materials);
		
		refresh();
		
		if (getch() != ERR) {
			terminate_curses();
			
			int* result = malloc(sizeof(int));
			*result = 0;
			return (void*)result;
		}
	}
	
}
