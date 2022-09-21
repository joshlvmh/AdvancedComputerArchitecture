#ifndef _INIT_H
#define _INIT_H

#include "share.h"

int initialise(RegFile** RF, width_t** mem, char* instr_file, char** instrs, int* instr_count, int* finished, t_instr** instructions, t_instr_char** single, t_dec_instr** dec_ins);

#endif
