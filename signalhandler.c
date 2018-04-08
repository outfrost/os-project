#include <stdlib.h>
#include <signal.h>

#include "ui.h"
#include "signalhandler.h"

void init_signalhandler() {
	struct sigaction* signal_action = malloc(sizeof(struct sigaction));
	if (signal_action != NULL) {
		(*signal_action).sa_handler = handle_signal;
		int sighandler_assign_result = sigaction(SIGWINCH, signal_action, NULL);
	}
}

void handle_signal(int signum) {
	if (signum == SIGWINCH) {
		update_ui_positioning();
	}
}
