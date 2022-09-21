#include "writeback.h"

void Writeback::writeback(Pipeline* pipeline, RegFile* RF, vector<vector<int> >* c_comp, width_t* mem, int* finished, int* exe_finished, vector<vector<int> >* c_prob, int tics)
{
  if (pipeline->regs->EX_WB_isEmpty) return;
  printf("----------------------------------------------------------------------------------------\n");
  printf("WRITEBACK STARTED\n");

  (*c_comp)[pipeline->regs->EX_WB_count][4] = tics;
  switch(pipeline->regs->EX_WB_TYPE)
  {
    case T_BZ:
      if (pipeline->stall) printf("stall at bz_t\n");
      break;
    case STOP:
      printf("HLT HLT HLT\n");
      *finished = 1;
      *exe_finished = 1;
      break;
    case REG:
      RF->RF[pipeline->regs->EX_WB_ALULOC] = pipeline->regs->EX_WB_ALUVAL;
      RF->scoreboard[pipeline->regs->EX_WB_ALULOC] = 1;
      break;
    case MEM:
      mem[RF->RF[pipeline->regs->EX_WB_ALULOC]] =pipeline->regs->EX_WB_ALUVAL;
      RF->scoreboard[pipeline->regs->EX_WB_ALULOC] = 1;
      break;
  }
  pipeline->regs->EX_WB_isEmpty = true;
  printf("WRITEBACK ir : %d loc %d val %d \n", pipeline->regs->EX_WB_IR, pipeline->regs->EX_WB_ALULOC, pipeline->regs->EX_WB_ALUVAL);
  printf("----------------------------------------------------------------------------------------\n");
}
