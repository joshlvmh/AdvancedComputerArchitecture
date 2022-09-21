#include "flush.h"

void flush(Pipeline* pipeline, vector<vector<int> >* c_comp, vector<vector<int> >* c_prob, RegFile* RF, int tics, int* pc)
{
  bool flush = false;
  int inum = 0; // assumption only only branch across all pipelines at once
  int j;
  for (int i=0; i < PIPELINE_C; i++)
  {
    if (pipeline[i].flush) j = i;
    flush = flush || pipeline[i].flush;
    inum += pipeline[i].bzu->bz_inum;
  }
  if (flush)  // due to branch only
  {
    printf("FLUSH FLUSH FLUSH inum: %d\n", inum);
    int ID = -1;
    for (int i=0; i < PIPELINE_C; i++)
    {
      if (pipeline[i].regs->IF_ID_IR > inum) pipeline[i].regs->IF_ID_isEmpty = true;
      if (pipeline[i].regs->ID_IS_IR > inum) pipeline[i].regs->ID_IS_isEmpty = true;
      if (pipeline[i].regs->IS_EX_IR[0] > inum) pipeline[i].regs->IS_EX_isEmpty[0] = true;
      if (pipeline[i].regs->IS_EX_IR[1] > inum) pipeline[i].regs->IS_EX_isEmpty[1] = true;
      if (pipeline[i].regs->IS_EX_IR[2] > inum) pipeline[i].regs->IS_EX_isEmpty[2] = true;
      if (pipeline[i].regs->IS_EX_IR[3] > inum) pipeline[i].regs->IS_EX_isEmpty[3] = true;
      if (pipeline[i].regs->IS_EX_IR[0] > inum) pipeline[i].alu1->cycle_count = 0;
      if (pipeline[i].regs->IS_EX_IR[1] > inum) pipeline[i].alu2->cycle_count = 0;
      if (pipeline[i].regs->IS_EX_IR[2] > inum) pipeline[i].bzu->cycle_count = 0;
      if (pipeline[i].regs->IS_EX_IR[3] > inum) pipeline[i].ldstu->cycle_count = 0;

    if (i == j)
    {
      ID = pipeline[i].regs->IS_EX_count[2];
    }
    // reset RF scoreboard as flush
    for (int i=0; i < NUM_REGS; i++)
    {
      RF->scoreboard[i] = 1;
    }
  }
    vector<int> probe = {tics, ID, 14};
    (*c_prob).push_back(probe);
    *pc = pipeline[j].regs->EX_WB_ALUVAL;
  }

  for (int i = 0;i<PIPELINE_C; i++)
  {
    pipeline[i].flush = false;
    pipeline[i].bzu->bz_inum = 0;
    pipeline[i].bzu->flush = 0;
  }
}
