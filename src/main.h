#ifndef _MAIN_H
#define _MAIN_H

#include "share.h"
#include "translate.h"
#include "execute.h"
#include "fetch.h"
#include "decode.h"
#include "initialise.h"
#include "flush.h"
#include "issue.h"
#include "writeback.h"
#include "probes.h"

int process(RegFile* RF, width_t* mem, t_instr* instructions, int* instr_count, int* finished, t_dec_instr* dec_ins, vector<string>* all_instrs, Pipeline* pipeline, BranchTargetBuffer* btb);

#endif
