#include <stdlib.h>
#include <ncurses.h>
#include <semaphore.h>

#include "worker.h"
#include "environment.h"
#include "profanities.h"

static int item_count_length;

void* profanities_run(void* arg) {
	start_curses();
	
	item_count_length = snprintf(NULL, 0, "%u", INITIAL_ITEMS);
	
	while (1) {
		sem_wait(&ui_update);
		
		draw_ui();
		
		if (getch() == 'q') {
			terminate_curses();
			
			int* result = malloc(sizeof(int));
			*result = 0;
			return (void*)result;
		}
	}
}

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

void draw_ui() {
	erase();
	
	int rows;
	int columns;
	getmaxyx(stdscr, rows, columns);
	
	mvprintw(rows - 1, 1, "q - quit");
	
	int goods_pile_row = (rows >> 1) - 3;
	int materials_pile_row = (rows >> 1) + 2;
	
	int piles_start_column = ((columns - item_count_length) >> 1);
	
	int suppliers_top_row = ((rows - SUPPLIERS_COUNT) >> 1);
	int consumers_top_row = ((rows - CONSUMERS_COUNT) >> 1);
	
	int suppliers_interaction_column = piles_start_column - 3;
	int suppliers_idle_column = suppliers_interaction_column - 4;
	int suppliers_proc_column = suppliers_idle_column - 4;
	
	int consumers_interaction_column = piles_start_column + item_count_length + 1;
	int consumers_idle_column = consumers_interaction_column + 5;
	int consumers_proc_column = consumers_idle_column + 3;
	
	for (int i = 0; i < SUPPLIERS_COUNT; ++i) {
		if (supplier_state[i] == IDLE) {
			mvprintw(suppliers_top_row + i, suppliers_idle_column, "@");
		}
		else if (supplier_state[i] == TAKING_ITEM) {
			mvprintw(materials_pile_row, suppliers_interaction_column, "@*");
		}
		else if (supplier_state[i] == PROCESSING) {
			mvprintw(suppliers_top_row + i, suppliers_proc_column, "*@");
		}
		else if (supplier_state[i] == STORING_ITEM) {
			mvprintw(goods_pile_row, suppliers_interaction_column, "@*");
		}
	}
	
	for (int i = 0; i < CONSUMERS_COUNT; ++i) {
		if (consumer_state[i] == IDLE) {
			mvprintw(consumers_top_row + i, consumers_idle_column, "@");
		}
		else if (consumer_state[i] == TAKING_ITEM) {
			mvprintw(goods_pile_row, consumers_interaction_column, "*@");
		}
		else if (consumer_state[i] == PROCESSING) {
			mvprintw(consumers_top_row + i, consumers_proc_column, "@*");
		}
		else if (consumer_state[i] == STORING_ITEM) {
			mvprintw(materials_pile_row, consumers_interaction_column, "*@");
		}
	}
	
	int goods;
	int materials;
	sem_getvalue(&stored_goods, &goods);
	sem_getvalue(&recycled_materials, &materials);
	
	mvprintw(goods_pile_row, piles_start_column, "%*d", item_count_length, goods);
	mvprintw(materials_pile_row, piles_start_column, "%*d", item_count_length, materials);
	
	refresh();
}
