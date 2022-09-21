#ifndef _TRANSLATE_H
#define _TRANSLATE_H
#include "share.h"

int translate_instrs(width_t** mem, char* instrs, int* instr_count, t_instr* instructions, t_instr_char* single, vector<string>* all_instrs);
int translate_line(int count, char* instrs, t_instr* instructions, t_instr_char* single);
int _operator(char* op);
int _operand(char* op);

#endif
