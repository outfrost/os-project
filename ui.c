#include <stdlib.h>
#include <ncurses.h>
#include <semaphore.h>
#include <signal.h>

#include "worker.h"
#include "environment.h"
#include "signalhandler.h"
#include "ui.h"

static UiPositioning ui_positioning;

void* ui_run(void* arg) {
	ui_positioning.item_count_length = snprintf(NULL, 0, "%u", INITIAL_ITEMS);
	start_curses();
	update_ui_positioning();
	
	while (getch() != 'q') {
		int semaphore_result = sem_trywait(&ui_update);
		if (semaphore_result == 0) {
			draw_ui();
		}
	}
	
	terminate_curses();
	
	int* result = malloc(sizeof(int));
	*result = 0;
	return (void*)result;
}

void start_curses() {
	initscr();
	cbreak();
	noecho();
	curs_set(0);
	nodelay(stdscr, true);
	
	init_signalhandler();
}

void terminate_curses() {
	endwin();
}

void draw_ui() {
	erase();
	
	mvprintw(ui_positioning.rows - 1, 1, "q - quit");
	
	for (int i = 0; i < SUPPLIERS_COUNT; ++i) {
		if (supplier_state[i] == IDLE) {
			mvprintw(ui_positioning.suppliers_y + i, ui_positioning.suppliers_idle_x, "@");
		}
		else if (supplier_state[i] == TAKING_ITEM) {
			mvprintw(ui_positioning.materials_pile_y, ui_positioning.suppliers_interaction_x, "@*");
		}
		else if (supplier_state[i] == PROCESSING) {
			mvprintw(ui_positioning.suppliers_y + i, ui_positioning.suppliers_proc_x, "*@");
		}
		else if (supplier_state[i] == STORING_ITEM) {
			mvprintw(ui_positioning.goods_pile_y, ui_positioning.suppliers_interaction_x, "@*");
		}
	}
	
	for (int i = 0; i < CONSUMERS_COUNT; ++i) {
		if (consumer_state[i] == IDLE) {
			mvprintw(ui_positioning.consumers_y + i, ui_positioning.consumers_idle_x, "@");
		}
		else if (consumer_state[i] == TAKING_ITEM) {
			mvprintw(ui_positioning.goods_pile_y, ui_positioning.consumers_interaction_x, "*@");
		}
		else if (consumer_state[i] == PROCESSING) {
			mvprintw(ui_positioning.consumers_y + i, ui_positioning.consumers_proc_x, "@*");
		}
		else if (consumer_state[i] == STORING_ITEM) {
			mvprintw(ui_positioning.materials_pile_y, ui_positioning.consumers_interaction_x, "*@");
		}
	}
	
	int goods;
	int materials;
	sem_getvalue(&stored_goods, &goods);
	sem_getvalue(&recycled_materials, &materials);
	
	mvprintw(ui_positioning.goods_pile_y, ui_positioning.piles_x, "%*d", ui_positioning.item_count_length, goods);
	mvprintw(ui_positioning.materials_pile_y, ui_positioning.piles_x, "%*d", ui_positioning.item_count_length, materials);
	
	refresh();
}

void update_ui_positioning() {
	getmaxyx(stdscr, ui_positioning.rows, ui_positioning.columns);
	
	ui_positioning.goods_pile_y = (ui_positioning.rows >> 1) - 3;
	ui_positioning.materials_pile_y = (ui_positioning.rows >> 1) + 2;
	
	ui_positioning.piles_x = ((ui_positioning.columns - ui_positioning.item_count_length) >> 1);
	
	ui_positioning.suppliers_y = ((ui_positioning.rows - SUPPLIERS_COUNT) >> 1);
	ui_positioning.consumers_y = ((ui_positioning.rows - CONSUMERS_COUNT) >> 1);
	
	ui_positioning.suppliers_interaction_x = ui_positioning.piles_x - 3;
	ui_positioning.suppliers_idle_x = ui_positioning.suppliers_interaction_x - 4;
	ui_positioning.suppliers_proc_x = ui_positioning.suppliers_idle_x - 4;
	
	ui_positioning.consumers_interaction_x = ui_positioning.piles_x + ui_positioning.item_count_length + 1;
	ui_positioning.consumers_idle_x = ui_positioning.consumers_interaction_x + 5;
	ui_positioning.consumers_proc_x = ui_positioning.consumers_idle_x + 3;
}

void terminal_size_changed() {
	endwin();
	refresh();
	update_ui_positioning();
	sem_post(&ui_update);
}
