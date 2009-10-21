#ifndef __H_ENDS_DEBUGGER__H
#define __H_ENDS_DEBUGGER__H

#include "../types.h"
#include <gtk/gtk.h>

#define RUNNING	1
#define STOPPED	0
#define PAUSED	2
#define STEPPING 3

typedef struct endsdbg{
	GtkWidget *window; 			/* Main Top-level window */
	GtkWidget *box; 			/* Main frame */
	GtkWidget *scroller;	
	GtkWidget *list_disasm; 	/* Program Opcode List widget */
	GtkWidget *button_run;
	GtkWidget *button_stop;
	GtkWidget *button_step;

	u32 debugger_state;

	void (*run)(struct endsdbg *dbgr);
	
	void *nds_cpu_pointer;
} endsdbg;

extern struct endsdbg *endsdbg_create(void *cpu_ptr);
extern void endsdbg_destroy(struct endsdbg *debugger);

#endif //__H_ENDS_DEBUGGER__H


