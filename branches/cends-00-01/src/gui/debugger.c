#include "debugger.h"
#include "../cpu/cpu.h"
#include "../cpu/opcodes.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

/* Consts */
static const char *INSTRUCTIONS[ ] = {"undefined",
                         "and", "eor", "sub", "rsb", "add", "adc", "sbc", "rsc", "tst", "teq", "cmp",
                         "cmn", "orr", "mov", "bic", "mvn", "mul", "mla", "smull","umull","smlal", "umlal",
                         "br",  "bl",  "blx", "bx",  "clz", "mrs", "msr", "ldr", "ldrb", "ldrbt",
                         "ldrbh",      "ldrh",       "ldrsb",      "ldrsh",      "ldrt", "str",  "strb",
                         "strbt",      "strh",       "strt",       "ldm",      "ldm",      "ldm",
                         "stm",      "stm",      "swp", "swpb",       "bkpt",        "swi",
                         "cdp", "ldc", "mcr", "mrc", "stc", "qadd",       "qsub",       "qdadd",
                         "qdsub",      "smlaxy",     "smlawy",     "smulwy",     "smlalxy",    "smulxy",
                         "ldrd",       "strd",       "pld", "stc2",       "ldc2","cdp2","mcr2", "mrc2",
                         "unimplemented", "adc", "add", "and", "asr", "b", "bic", "bkpt", "bl", "blx",
						"bx", "cmn", "cmp", "eor", "ldmia",
						"ldr", "ldrb", "ldrh", "ldrsb", "ldrsh", "lsl", "lsr", "mov", "mul",
						"mvn", "neg", "orr", "pop", "push", "ror","sbc", "stmia", "str", "strb" ,"strh", "sub", "swi", "tst" };

static const char *CONDITIONS[] = { "eq", "ne", "cs", "cc", "mi", "pl", "vs", "vc", "hi", "ls", "ge", "lt", "gt", "le", "al", "nv"};

/* Function declarations */
static void debugger_run(struct endsdbg *dbgr);
static void debugger_construct_window(struct endsdbg *dbgr);
static void debugger_bind_events(struct endsdbg *dbgr);
static void debugger_create_opcode_string(char *buffer, ndsCPU *cpu);
static void debugger_cpu_runner(gpointer data, gint source_fd, GdkInputCondition condition);
static void debugger_button_step(gpointer data);

/* Create an instance of the debugger */
struct endsdbg *endsdbg_create(void *cpu_ptr)
{
	struct endsdbg *dbgr = malloc( sizeof(*dbgr) );

	/* Bind run function */
	dbgr->run = &debugger_run;
	dbgr->nds_cpu_pointer = cpu_ptr;

	debugger_construct_window(dbgr);
	debugger_bind_events(dbgr);

	gchar *item[1];
   	gchar buffer[80];
	item[0]	= buffer;
	/* Fill in the list:
	 * we dont actually run the cpu here, instead the 'exec' process is
	 * gutted here and re-done w/o the executor part */
	ndsCPU *cpu = (ndsCPU*)(cpu_ptr);
	//cpu->R[15] = 0x02008000;
	unsigned int j = 0;
	for(j = 0; j < 0x1000; j++){
		cpu->carry = 0;
		cpu->adressing = UNDEFINED;
		cpu->instruction = UNDEFINED;
		cpu->op.value = read_word(PC, cpu->memory);
		arm9_decoder(cpu_ptr);
		debugger_create_opcode_string(buffer, cpu);
		gtk_clist_append(GTK_CLIST(dbgr->list_disasm), item);
		PC += 4;
	}

	PC = 0x02000000;
	dbgr->debugger_state = STOPPED;

	return dbgr;
}
/* Create the main window */
void debugger_construct_window(struct endsdbg *dbgr)
{
	/* Create Elements */
	dbgr->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	dbgr->box = gtk_vbox_new(FALSE, 5);
	dbgr->scroller = gtk_scrolled_window_new(NULL, NULL);
	dbgr->list_disasm = gtk_clist_new(1);
	dbgr->button_step = gtk_button_new_with_label("Step");

	/* Main Window */
	gtk_window_set_title(GTK_WINDOW(dbgr->window), "eNDS Debugger");
	gtk_window_set_default_size(GTK_WINDOW(dbgr->window), 800, 600);
	gtk_window_set_position(GTK_WINDOW(dbgr->window), GTK_WIN_POS_CENTER);

	/* Opcode Box, Scroller, List */
	gtk_container_set_border_width(GTK_CONTAINER(dbgr->box), 5);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(dbgr->scroller),
									GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
	gtk_clist_set_selection_mode(GTK_CLIST(dbgr->list_disasm), GTK_SELECTION_SINGLE);
	gtk_clist_set_shadow_type(GTK_CLIST(dbgr->list_disasm), GTK_SHADOW_OUT);
	gtk_clist_set_column_width(GTK_CLIST(dbgr->list_disasm), 0, 150);
	gtk_clist_set_row_height(GTK_CLIST(dbgr->list_disasm), 18);

	/* Bind Widgets Together */
	gtk_container_add(GTK_CONTAINER(dbgr->window), dbgr->box);
	gtk_box_pack_start(GTK_BOX(dbgr->box), dbgr->scroller, TRUE, TRUE, 0);
	gtk_container_add(GTK_CONTAINER(dbgr->scroller), dbgr->list_disasm);
	gtk_box_pack_start(GTK_BOX(dbgr->box), dbgr->button_step, TRUE, TRUE, 0);

}
/* Signals, events etc., */
void debugger_bind_events(struct endsdbg *dbgr)
{
	gtk_signal_connect(GTK_OBJECT(dbgr->window), "destroy", GTK_SIGNAL_FUNC(gtk_main_quit), NULL);
	gtk_idle_add((GSourceFunc)debugger_cpu_runner, dbgr);
	gtk_signal_connect_object(GTK_OBJECT(dbgr->button_step), "clicked", GTK_SIGNAL_FUNC(debugger_button_step), (gpointer)(dbgr));
}
/* Destroy the debugger */
void endsdbg_destroy(struct endsdbg *debugger)
{
	assert( debugger );
	if( debugger ){
		free(debugger);
	}
}

/* eNDS Debugger main 'run' function */
void debugger_run(struct endsdbg *dbgr)
{
	assert(dbgr);

	/* Show Widgets */
	gtk_widget_show(dbgr->scroller);
	gtk_widget_show(dbgr->list_disasm);
	gtk_widget_show(dbgr->box);
	gtk_widget_show(dbgr->window);
	gtk_widget_show(dbgr->button_step);

	gtk_clist_select_row(GTK_CLIST(dbgr->list_disasm), 0, 0);

	gtk_main();
}

/* On Idle cpu runner */
void debugger_cpu_runner(gpointer data, gint source_fd, GdkInputCondition condition)
{
	assert(data);
	endsdbg *dbgr = (endsdbg*)(data);
	static ndsCPU *cpu;
    cpu	= (ndsCPU*)(dbgr->nds_cpu_pointer);

	assert(cpu);

	if( dbgr->debugger_state == RUNNING ){
		cpu->exec(cpu);
		gtk_clist_select_row(GTK_CLIST(dbgr->list_disasm), (PC - 0x02000000)/4, 0);
		g_print("%08X\n", PC);
	} else if( dbgr->debugger_state == STEPPING ){
		cpu->exec(cpu);
		gtk_clist_select_row(GTK_CLIST(dbgr->list_disasm), (PC - 0x02000000)/4, 0);
		g_print("%08X\n", PC);
		dbgr->debugger_state = STOPPED;
	}
}

void debugger_button_step(gpointer data)
{
	endsdbg *dbgr = (endsdbg *)(data);
	dbgr->debugger_state = STEPPING;
	g_print("Stepped\n");
}

/* Opcode string generator */
void debugger_create_opcode_string(char *buffer, ndsCPU *cpu)
{
	char operands[30] = "\0";
	switch( cpu->adressing ){
		case AD_DATA_IMMEDIATE:
			sprintf(operands,"\t: r%02u, r%02u, #%X", cpu->op.Rd, cpu->op.Rn, cpu->operand);
			break;
		case AD_DATA_REGISTER:
			sprintf(operands,"\t: r%02u, r%02u, r%02u", cpu->op.Rd, cpu->op.Rn, cpu->op.Rm);
			break;
		case AD_DATA_LSL_IMMEDIATE:
		case AD_DATA_LSR_IMMEDIATE:
		case AD_DATA_ASR_IMMEDIATE:
		case AD_DATA_ROR_IMMEDIATE:
		case AD_DATA_RRX_IMMEDIATE:
			sprintf(operands,"\t: r%02u, r%02u, r%02u, #%X", cpu->op.Rd, cpu->op.Rn, cpu->op.Rm, cpu->operand);
			break;
		case AD_DATA_LSL_REGISTER:
		case AD_DATA_LSR_REGISTER:
		case AD_DATA_ASR_REGISTER:
		case AD_DATA_ROR_REGISTER:
			if( cpu->op.S ){
				sprintf(operands,"s\t: r%02u, r%02u, r%02u", cpu->op.Rd, cpu->op.Rn, cpu->op.Rs);
			} else {
				sprintf(operands,"\t: r%02u, r%02u", cpu->op.Rd, cpu->op.Rs);
			}
			break;
		case AD_LOAD_OFFSET_IMMEDIATE:
		case AD_LOAD_OFFSET_REGISTER:
		case AD_LOAD_PRE_IMMEDIATE:
		case AD_LOAD_PRE_REGISTER:
		case AD_LOAD_POST_REGISTER:
		case AD_LOAD_SCALED_OFFSET:
		case AD_LOAD_SCALED_PRE:
		case AD_LOAD_SCALED_POST:
		case AD_MISCLOAD_OFFSET_IMMEDIATE:
		case AD_MISCLOAD_PRE_IMMEDIATE:
		case AD_MISCLOAD_POST_IMMEDIATE:
		case AD_MISCLOAD_OFFSET_REGISTER:
			break;
		default:
			switch( cpu->instruction ){
				case BR:
				case BL:
					sprintf(operands, "\t: #%08X", cpu->adress);
					break;
				default:
					sprintf(operands, "\t: unknown");
			}
	}
	sprintf(buffer, "%08X : %s %s%s", PC, INSTRUCTIONS[cpu->instruction], CONDITIONS[cpu->op.cond.val], operands);

}
