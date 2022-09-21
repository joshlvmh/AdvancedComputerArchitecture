#include "fetch.h"

void Fetch::fetch(Pipeline* pipeline, int* instr_c, vector<vector<int>>* c_comp, int* instr_count, vector<vector<int> >* c_prob, int* pc, int tics, BranchTargetBuffer* btb)
{
  if (!pipeline->regs->IF_ID_isEmpty) {return;}
  if (pipeline->regs->IF_ID_NPC >= (*instr_count)*4 && pipeline->bzu->branch != 1) {return;}
  if (pipeline->stall) return;
  printf("----------------------------------------------------------------------------------------\n");
  printf("FETCH\n");

  pipeline->regs->IF_ID_IR = *pc;
  if (pipeline->regs->EX_WB_TYPE == T_BZ)
  {
    printf("BRANCH FETCH___________%d____________________\n", pipeline->regs->EX_WB_ALUVAL);
    pipeline->regs->EX_WB_TYPE = -1;
  }
  if (*pc >= (*instr_count)*4) return;
                          // fetch,   dec, is, exe, wb, pc, /, ins_id, instr
  std::vector<int> row = {tics, -1, -1, -1, -1, *pc, 0, *instr_c, -1};
  int next = btb->pullRes(pipeline->regs->IF_ID_IR);
  if (next == -1)
  {
    *pc = *pc + 4;
  }
  else
  {
    *pc =  next;
    printf("PREDICTION: Branch predicted npc: %d\n", *pc);
  }
  pipeline->regs->IF_ID_NPC = *pc + 4*(PIPELINE_C-1);

  pipeline->regs->IF_ID_count = *instr_c;
  (*instr_c)++;
  (*c_comp).push_back(row);

  pipeline->regs->IF_ID_isEmpty = false;
  printf("IR: %d NPC: %d\n", pipeline->regs->IF_ID_IR, pipeline->regs->IF_ID_NPC);
  printf("----------------------------------------------------------------------------------------\n");
}
