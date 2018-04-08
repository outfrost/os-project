#ifndef UI_H_
#define UI_H_

void* ui_run(void* arg);
void start_curses();
void terminate_curses();
void draw_ui();
void update_ui_positioning();
void terminal_size_changed();

typedef struct {
	int item_count_length;
	int rows;
	int columns;
	int goods_pile_y;
	int materials_pile_y;
	int piles_x;
	int suppliers_y;
	int consumers_y;
	int suppliers_interaction_x;
	int suppliers_idle_x;
	int suppliers_proc_x;
	int consumers_interaction_x;
	int consumers_idle_x;
	int consumers_proc_x;
} UiPositioning;

#endif
