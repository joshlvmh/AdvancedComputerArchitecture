#include "branch_pred.h"

void BranchTargetBuffer::addRes(int pc, int res, int tar, int type) {
  int index = -1;
  for (int i = 0; i < INSTR_BUFF; i++)
  {
    if (pc == BZ_PC[i]) index = i;
  }
  if (index != -1)
  {
    switch(RES2[index])
    {
      case 0: // strong not taken
        RES2[index] = (res == RES1[index] ? 0 : 1);
        break;
      case 1: // weak not taken
        RES2[index] = (res == RES1[index] ? 0 : 2);
        break;
      case 2: // weak taken
        RES2[index] = (res == RES1[index] ? 3 : 1);
        break;
      case 3: // strong taken
        RES2[index] = (res == RES1[index] ? 3 : 2);
        break;
      default:
        printf("RES2 not configured %d\n", RES1[index]);
        break;
    }
    RES1[index] = res;
    if (res == 1) TAR[index] = tar;
  }
  else // new BZ
  {
    BZ_PC[tail] = pc;
    RES1[tail] = res;
    RES2[tail] = res+1;
    if (res == 1) TAR[tail] = tar;
    BZ_T[tail] = type;
    tail++;
  }
}

bool BranchTargetBuffer::checkTar(int pc, int res) {
  int index = -1;
  for (int i = 0; i < INSTR_BUFF; i++)
  {
    if (pc == BZ_PC[i]) index = i;
  }
  if (index == -1) return false;
  if (res == LAST[index]) return true;
  return false;
}

int BranchTargetBuffer::pullRes(int pc) {
  int index = -1;
  for (int i = 0; i < INSTR_BUFF; i++)
  {
    if (pc == BZ_PC[i]) index = i;
  }
  if (index == -1) return -1; // not seen before

  if (BZ_T[index] == 1) {
    LAST[index] = 1;
    return TAR[index]; // if JMP, always take
  }
  switch(BPREDTYPE)
  {
    case 0: // always taken
      LAST[index] = 1;
      return TAR[index];
      break;
    case 1: // always taken if backwards
      if (TAR[index] < pc) {
        LAST[index] = 1;
        return TAR[index];
      }
      else {
        LAST[index] = 0;
        return -1;
      }
      break;
    case 2: // dynamic 1-bit
      if (RES1[index] == 1) {
        LAST[index] = 1;
        return TAR[index];
      }
      else {
        LAST[index] = 0;
        return -1;
      }
      break;
    case 3: // dynamic 2-bit
      printf("RES2 INDEX: %d\n", RES2[index]);
      if (RES2[index] > 1)
      {
        LAST[index] = 1;
        return TAR[index];
      }
      else {
        LAST[index] = 0;
        return -1;
      }
      break;
    default:
      break;
  }
  // default taken
  LAST[index] = 1;
  return TAR[index];
}
