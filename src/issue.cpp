#include "issue.h"

bool ALUscoreboardCheck(t_dec_instr *instruction, RegFile *RF)
{
  bool rd = false;
  bool rs1 = false;
  bool rs2 = false;
  if (RF->scoreboard[instruction->rd] == 1) rd = true;
  if (RF->scoreboard[instruction->rs1] == 1) rs1 = true;
  if (RF->scoreboard[instruction->rs2] == 1) rs2 = true;

  return (rd and rs1 and rs2);
}

bool BZscoreboardCheck(t_dec_instr *instruction, RegFile *RF)
{
  bool rs1 = false;
  bool rs2 = false;

  if (RF->scoreboard[instruction->rs1] == 1) rs1 = true;
  if (RF->scoreboard[instruction->rs2] == 1) rs2 = true;

  return (rs1 and rs2);
}

bool LDscoreboardCheck(t_dec_instr *instruction, RegFile *RF)
{
  bool rd = false;
  bool rs1 = false;

  if (RF->scoreboard[instruction->rd] == 1) rd = true;
  if (RF->scoreboard[instruction->rs1] == 1) rs1 = true;
  if (instruction->opcode == LDI) rs1 = true;

  return (rd and rs1);
}

void fillRegs(Pipeline* pipeline, RegFile* RF, int i)
{
  switch (pipeline->regs->ID_IS_OUT->opcode)
  {
    case ADD:
    case SUB:
    case MUL:
    case AND:
    case OR:
    case XOR:
    case LDS:
    case CMP:
    case SHR:
    case SHL:
      pipeline->regs->IS_EX_RS1[i] = RF->RF[pipeline->regs->ID_IS_OUT->rs1];
      pipeline->regs->IS_EX_RS2[i] = RF->RF[pipeline->regs->ID_IS_OUT->rs2];
      pipeline->regs->IS_EX_RD[i] = pipeline->regs->ID_IS_OUT->rd;
      break;
    case BEQ:
    case BNQ:
      pipeline->regs->IS_EX_RS1[i] = RF->RF[pipeline->regs->ID_IS_OUT->rs1];
      pipeline->regs->IS_EX_RS2[i] = RF->RF[pipeline->regs->ID_IS_OUT->rs2];
      pipeline->regs->IS_EX_IMM[i] = pipeline->regs->ID_IS_OUT->pcrel;
      break;
    case JMP:
      pipeline->regs->IS_EX_IMM[i] = pipeline->regs->ID_IS_OUT->pcrel;
      pipeline->regs->IS_EX_RD[i] = pipeline->regs->ID_IS_OUT->rd;
      break;
    case LD:
    case ST:
      pipeline->regs->IS_EX_RS1[i] = RF->RF[pipeline->regs->ID_IS_OUT->rs1];
      pipeline->regs->IS_EX_RD[i] = pipeline->regs->ID_IS_OUT->rd;
      break;
    case LDI:
      pipeline->regs->IS_EX_IMM[i] = pipeline->regs->ID_IS_OUT->imm;
      pipeline->regs->IS_EX_RD[i] = pipeline->regs->ID_IS_OUT->rd;
      break;
    case HLT:
      break;
  }
  pipeline->regs->IS_EX_OPC[i] = pipeline->regs->ID_IS_OUT->opcode;
  pipeline->regs->IS_EX_IR[i] = pipeline->regs->ID_IS_IR;
  pipeline->regs->IS_EX_NPC[i] = pipeline->regs->ID_IS_NPC;
}

bool scoreboard(t_dec_instr* instr, RegFile* RF)
{
  bool dependent = false;
  switch(instr->type)
  {
    case T_ALU1:
    case T_ALU2:
      dependent = ALUscoreboardCheck(instr, RF);
      break;
    case T_LDST:
      dependent = LDscoreboardCheck(instr, RF);
      break;
    case T_BZ:
      if (instr->opcode == HLT) dependent = true;
      else dependent = BZscoreboardCheck(instr, RF);
      break;
  }
  return dependent;
}

bool checkOldest(Pipeline* pipeline, int i, RegFile* RF)
{
  if (scoreboard(pipeline[i].regs->ID_IS_OUT, RF) && pipeline[i].regs->ID_IS_AGE > 0) return true; // if dependency free, does not need to be oldest

  int max = pipeline[0].regs->ID_IS_AGE;
  for (int j = 0; j < PIPELINE_C; j++)
  {
    if (max < pipeline[j].regs->ID_IS_AGE && pipeline[j].regs->ID_IS_AGE != -1)
    {
      max = pipeline[j].regs->ID_IS_AGE;
    }
  }
  if (max == pipeline[i].regs->ID_IS_AGE) return true;
  else return false;
}


void Issue::issue(Pipeline* pipeline, int i, t_instr* instructions, vector<vector<int> >* c_comp, RegFile* RF, vector<vector<int> >* c_prob, int tics)
{
  if (pipeline[i].regs->ID_IS_isEmpty) return;
  if (!(pipeline[i].regs->IS_EX_isEmpty[0] || pipeline[i].regs->IS_EX_isEmpty[1] || pipeline[i].regs->IS_EX_isEmpty[2] || pipeline[i].regs->IS_EX_isEmpty[3])) return;
  if (!checkOldest(pipeline, i, RF)) return;
  if (pipeline[i].stall) return;
  printf("----------------------------------------------------------------------------------------\n");
  printf("ISSUE\n");

  bool issued = false;
  if(!traced) {(*c_comp)[pipeline[i].regs->ID_IS_count][2] = tics; traced=true;}
  switch(pipeline[i].regs->ID_IS_OUT->type)
  {
    case T_ALU1:
    case T_ALU2:
      printf("ALU CASE ISSUE\n");
      if (ALUscoreboardCheck(pipeline[i].regs->ID_IS_OUT, RF))
      {
        printf("SCOREBOARD TRUE\n");
        if (pipeline[i].regs->IS_EX_isEmpty[0])
        {
          pipeline[i].regs->IS_EX_isEmpty[0] = false;
          RF->scoreboard[pipeline[i].regs->ID_IS_OUT->rd] = 0;
          fillRegs(&pipeline[i], RF, 0);
          pipeline[i].regs->ID_IS_isEmpty = true;
          issued = true;
          printf("ISSUE IR: %d NPC: %d\n", pipeline[i].regs->IS_EX_IR[0], pipeline[i].regs->IS_EX_NPC[0]);
          pipeline[i].regs->IS_EX_count[0] = pipeline[i].regs->ID_IS_count;
          break;
        }
        else if (pipeline[i].regs->IS_EX_isEmpty[1])
        {
          pipeline[i].regs->IS_EX_isEmpty[1] = false;
          RF->scoreboard[pipeline[i].regs->ID_IS_OUT->rd] = 0;
          fillRegs(&pipeline[i], RF, 1);
          pipeline[i].regs->ID_IS_isEmpty = true;
          issued = true;
          printf("ISSUE IR: %d NPC: %d\n", pipeline[i].regs->IS_EX_IR[1], pipeline[i].regs->IS_EX_NPC[1]);
          pipeline[i].regs->IS_EX_count[1] = pipeline[i].regs->ID_IS_count;
          break;
        }
      else pipeline[i].stall = true;
      }
      else pipeline[i].stall = true;
      break;
    case T_BZ:
      if (pipeline[i].regs->IS_EX_isEmpty[2] &&
          (pipeline[i].regs->ID_IS_OUT->opcode == HLT || BZscoreboardCheck(pipeline[i].regs->ID_IS_OUT, RF)))
      {
        pipeline[i].regs->IS_EX_isEmpty[2] = false;
        pipeline[i].regs->ID_IS_isEmpty = true;
        fillRegs(&pipeline[i], RF, 2);
        issued = true;
        printf("ISSUE IR: %d NPC: %d\n", pipeline[i].regs->IS_EX_IR[2], pipeline[i].regs->IS_EX_NPC[2]);
        pipeline[i].regs->IS_EX_count[2] = pipeline[i].regs->ID_IS_count;
      }
      else pipeline[i].stall = true;
      break;
    case T_LDST:
      if (pipeline[i].regs->IS_EX_isEmpty[3] && LDscoreboardCheck(pipeline[i].regs->ID_IS_OUT, RF))
      {
        pipeline[i].regs->IS_EX_isEmpty[3] = false;
        pipeline[i].regs->ID_IS_isEmpty = true;
        RF->scoreboard[pipeline[i].regs->ID_IS_OUT->rd] = 0;
        fillRegs(&pipeline[i], RF, 3);
        issued = true;
        printf("ISSUE IR: %d NPC: %d\n", pipeline[i].regs->IS_EX_IR[3], pipeline[i].regs->IS_EX_NPC[3]);
        pipeline[i].regs->IS_EX_count[3] = pipeline[i].regs->ID_IS_count;
      }
      else {
        pipeline[i].stall = true;
        if (!pipeline[i].regs->IS_EX_isEmpty[3]) printf("ldstu not free\n");
        if (!LDscoreboardCheck(pipeline[i].regs->ID_IS_OUT, RF)) printf("scoreboard fail\n");
      }
      break;
    default:
      error("not an instruction type", __LINE__, __FILE__);
      break;
  }
  if (pipeline[i].stall)
  {
    vector<int> probe = {tics, pipeline[i].regs->ID_IS_count, 8};
    (*c_prob).push_back(probe);
    pipeline[i].stall = false;
  }
  printf("ISSUE: ");
  if (issued) {
    printf("issued\n");
    pipeline[i].regs->ID_IS_AGE = 0;
    traced = false;
  }
  else {printf("not issued IR: %d NPC: %d\n", pipeline[i].regs->ID_IS_IR, pipeline[i].regs->ID_IS_NPC); pipeline[i].regs->ID_IS_AGE++;}
  printf("----------------------------------------------------------------------------------------\n");
}
