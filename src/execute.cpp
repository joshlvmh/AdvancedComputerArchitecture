#include "execute.h"
void ALU1::exe(Pipeline* pipeline, vector<vector<int> >* c_comp, vector<vector<int> >* c_prob, int tics)
{
  char message[1024]; /* error buffer */
  if (cycle_count != 0 && cycle_count != 1) {cycle_count--; return;}
  if (cycle_count == 0 && pipeline->regs->IS_EX_isEmpty[0]) return;
  if (cycle_count == 1 && !pipeline->regs->EX_WB_isEmpty) return;
  printf("----------------------------------------------------------------------------------------\n");
  printf("ALU1\n");

  if (cycle_count == 0) {
  if (pipeline->stall) return;
  loc = pipeline->regs->IS_EX_RD[0];
  pipeline->regs->IS_EX_isEmpty[0] = true;
  int rs1 = pipeline->regs->IS_EX_RS1[0];
  int rs2 = pipeline->regs->IS_EX_RS2[0];
  (*c_comp)[pipeline->regs->IS_EX_count[0]][3] = tics;
  switch(pipeline->regs->IS_EX_OPC[0])
  {
    case ADD:
      val = rs1 + rs2;
      cycles = 1;
      break;
    case SUB:
      val = rs1 - rs2;
      cycles = 1;
      break;
    case MUL:
      val = rs1 * rs2;
      cycles = 3;
      break;
    case AND:
      val = rs1 & rs2;
      cycles = 1;
      break;
    case  OR:
      val = rs1 | rs2;
      cycles = 1;
      break;
    case XOR:
      val = rs1 ^ rs2;
      cycles = 1;
      break;
    case CMP:
      val = (rs1 > rs2) ? 1 : (rs1 < rs2 ? -1 : 0);
      cycles = 2;
      break;
    case SHR:
      val = rs1 >> rs2;
      cycles = 1;
      break;
    case SHL:
      val = rs1 << rs2;
      cycles = 1;
      break;
    default:
      sprintf(message, "opcode not recognised");
      error(message, __LINE__, __FILE__);
      break;
  }
  type = REG;
  cycle_count = cycles;
  instr_count = pipeline->regs->IS_EX_IR[0]/4;
  }

  if (cycle_count == 1 && pipeline->regs->EX_WB_isEmpty)
  {
    pipeline->regs->EX_WB_ALUVAL = pipeline->regs->IS_EX_NPC[0];
    pipeline->regs->EX_WB_IR = pipeline->regs->IS_EX_IR[0];
    pipeline->regs->EX_WB_ALUVAL = val;
    pipeline->regs->EX_WB_ALULOC = loc;
    pipeline->regs->EX_WB_TYPE = type;
    pipeline->regs->EX_WB_isEmpty = false;
    pipeline->stall = false;
    pipeline->regs->EX_WB_count = pipeline->regs->IS_EX_count[0];
  }
  else
  {
    printf("STALL------------- cycles: %d\n", cycles);
    pipeline->stall = true;
    vector<int> probe = {tics, pipeline->regs->IS_EX_count[0], 9};
    (*c_prob).push_back(probe);
    if (cycle_count == 1) cycle_count++;
  }
  cycle_count--;
  printf("IR %d NPC %d\n", pipeline->regs->IS_EX_IR[0], pipeline->regs->IS_EX_NPC[0]);
  printf("----------------------------------------------------------------------------------------\n");
}

void ALU2::exe(Pipeline* pipeline, vector<vector<int> >* c_comp, vector<vector<int> >* c_prob, int tics)
{
  char message[1024]; /* error buffer */
  if (cycle_count != 0 && cycle_count != 1) {cycle_count--; return;}
  if (cycle_count == 0 && pipeline->regs->IS_EX_isEmpty[1]) return;
  if (cycle_count == 1 && !pipeline->regs->EX_WB_isEmpty) return;
  printf("----------------------------------------------------------------------------------------\n");
  printf("ALU2\n");

  if (cycle_count == 0) {
  if (pipeline->stall) return;
  loc = pipeline->regs->IS_EX_RD[1];
  pipeline->regs->IS_EX_isEmpty[1] = true;
  int rs1 = pipeline->regs->IS_EX_RS1[1];
  int rs2 = pipeline->regs->IS_EX_RS2[1];
  (*c_comp)[pipeline->regs->IS_EX_count[1]][3] = tics;
  switch(pipeline->regs->IS_EX_OPC[1])
  {
    case ADD:
      val = rs1 + rs2;
      cycles = 1;
      break;
    case SUB:
      val = rs1 - rs2;
      cycles = 1;
      break;
    case MUL:
      val = rs1 * rs2;
      cycles = 3;
      break;
    case AND:
      val = rs1 & rs2;
      cycles = 1;
      break;
    case  OR:
      val = rs1 | rs2;
      cycles = 1;
      break;
    case XOR:
      val = rs1 ^ rs2;
      cycles = 1;
      break;
    case CMP:
      val = (rs1 > rs2) ? 1 : (rs1 < rs2 ? -1 : 0);
      cycles = 2;
      break;
    case SHR:
      val = rs1 >> rs2;
      cycles = 1;
      break;
    case SHL:
      val = rs1 << rs2;
      cycles = 1;
      break;
    default:
      sprintf(message, "opcode not recognised");
      error(message, __LINE__, __FILE__);
      break;
  }
  cycle_count = cycles;
  type = REG;
  instr_count = pipeline->regs->IS_EX_IR[1]/4;
  }

  if (cycle_count == 1 && pipeline->regs->EX_WB_isEmpty)
  {
    pipeline->regs->EX_WB_ALUVAL = pipeline->regs->IS_EX_NPC[1];
    pipeline->regs->EX_WB_IR = pipeline->regs->IS_EX_IR[1];
    pipeline->regs->EX_WB_ALUVAL = val;
    pipeline->regs->EX_WB_ALULOC = loc;
    pipeline->regs->EX_WB_TYPE = type;
    pipeline->regs->EX_WB_isEmpty = false;
    pipeline->stall = false;
    pipeline->regs->EX_WB_count = pipeline->regs->IS_EX_count[1];
  }
  else
  {
    printf("STALL------------- cycles: %d\n", cycles);
    pipeline->stall = true;
    vector<int> probe = {tics, pipeline->regs->IS_EX_count[1], 9};
    (*c_prob).push_back(probe);
    if (cycle_count == 1) cycle_count++;
  }
  cycle_count--;
  printf("IR %d NPC %d\n", pipeline->regs->IS_EX_IR[1], pipeline->regs->IS_EX_NPC[1]);
  printf("----------------------------------------------------------------------------------------\n");
}

void BZU::exe(Pipeline* pipeline, int* finished, vector<vector<int> >* c_comp, vector<vector<int> >* c_prob, int tics, BranchTargetBuffer* btb)
{
  char message[1024]; /* error buffer */
  if (cycle_count != 0 && cycle_count != 1) {cycle_count--; return;}
  if (cycle_count == 0 && pipeline->regs->IS_EX_isEmpty[2]) return;
  if (cycle_count == 1 && !pipeline->regs->EX_WB_isEmpty) return;
  printf("----------------------------------------------------------------------------------------\n");
  printf("BZU\n");

  if (cycle_count == 0) {
  if (pipeline->stall) return;
  loc = pipeline->regs->IS_EX_RD[2];
  pipeline->regs->IS_EX_isEmpty[2] = true;
  int rs1 = pipeline->regs->IS_EX_RS1[2];
  int rs2 = pipeline->regs->IS_EX_RS2[2];
  int imm = pipeline->regs->IS_EX_IMM[2];
  int pc = pipeline->regs->IS_EX_IR[2];
  int ex_pc_out;
  int type_j_b;
  (*c_comp)[pipeline->regs->IS_EX_count[2]][3] = tics;
  switch(pipeline->regs->IS_EX_OPC[2])
  {
    case BEQ:
      branch = ((rs1 == rs2) ? 1 : 0);
      if (branch)
      {
        ex_pc_out = pc + imm;
        printf("BEQ OCCURRING %d = %d + %d\n", ex_pc_out, pc, imm);
      }
      else { ex_pc_out = pc+4;
      }
      cycles = 1;
      type = T_BZ;
      type_j_b = 0;
      break;
    case BNQ:
      branch = ((rs1 != rs2) ? 1 : 0);
      if (branch)
      {
        ex_pc_out = pc + imm;
        printf("BNQ OCCURRING %d = %d + %d\n", ex_pc_out, pc, imm);
      }
      else { ex_pc_out = pc+4;
        printf("BNQ NOT OCCURRING %d = %d + %d < 4\n", ex_pc_out, pc, imm);
      }
      cycles = 1;
      type = T_BZ;
      type_j_b = 0;
      break;
    case JMP:
      ex_pc_out = pc + imm;
      branch = 1;
      type = T_BZ;
      cycles = 1;
      type_j_b = 1;
      break;
    case HLT:
      ex_pc_out = pc + 4;
      type = STOP;
      cycles = 1;
      break;
    default:
      sprintf(message, "opcode not recognised:");
      error(message, __LINE__, __FILE__);
      break;
  }
  if (type == T_BZ) // Update Branch Target Buffer
  {
    if (!btb->checkTar(pc, branch)) flush = 1;
    btb->addRes(pc, branch, ex_pc_out, type_j_b);
    if (flush) mis_count++;
    b_count++;
  }
  cycle_count = cycles;
  pipeline->regs->EX_WB_ALUVAL = ex_pc_out;
  instr_count = pipeline->regs->IS_EX_IR[2]/4;
  if (flush) pipeline->flush = true;
  if (flush) bz_inum = pc;
  }
  if (cycle_count == 1 && pipeline->regs->EX_WB_isEmpty)
  {
    pipeline->regs->EX_WB_IR = pipeline->regs->IS_EX_IR[2];
    pipeline->regs->EX_WB_TYPE = type;
    pipeline->regs->EX_WB_isEmpty = false;
    pipeline->stall = false;
    pipeline->regs->EX_WB_count = pipeline->regs->IS_EX_count[2];
  }
  else
  {
    printf("STALL------------- cycles: %d\n", cycles);
    pipeline->stall = true;
    vector<int> probe = {tics, pipeline->regs->IS_EX_count[2], 9};
    (*c_prob).push_back(probe);
    if (cycle_count == 1) cycle_count++;
  }
  cycle_count--;
  printf("IR %d NPC %d\n", pipeline->regs->IS_EX_IR[2], pipeline->regs->IS_EX_NPC[2]);
  printf("----------------------------------------------------------------------------------------\n");
}
void LDSTU::exe(Pipeline* pipeline, width_t* mem, vector<vector<int> >* c_comp, vector<vector<int> >* c_prob, int tics)
{
  char message[1024]; /* error buffer */
  if (cycle_count != 0 && cycle_count != 1) {cycle_count--; return;}
  if (cycle_count == 0 && pipeline->regs->IS_EX_isEmpty[3]) {return;}
  if (cycle_count == 1 && !pipeline->regs->EX_WB_isEmpty) {return;}
  printf("----------------------------------------------------------------------------------------\n");
  printf("LDSTU\n");

  if (cycle_count == 0) {
  if (pipeline->stall) return;
  printf("EXECUTE LDSTU----------------\n");
  loc = pipeline->regs->IS_EX_RD[3];
  int rs1 = pipeline->regs->IS_EX_RS1[3];
  int rs2 = pipeline->regs->IS_EX_RS2[3];
  int imm = pipeline->regs->IS_EX_IMM[3];
  (*c_comp)[pipeline->regs->IS_EX_count[3]][3] = tics;
  switch(pipeline->regs->IS_EX_OPC[3])
  {
    case  LD:
      val = mem[rs1];
      printf("LD: VAL: %d rs1: %d\n", val, rs1);
      type = REG;
      cycles = 5;
      break;
    case LDI:
      printf("LDI VAL: %d, IR: %d\n", imm, pipeline->regs->IS_EX_IR[3]);
      val = imm;
      type = REG;
      cycles = 1;
      break;
    case LDS:
      val = mem[rs1 + rs2];
      type = REG;
      cycles = 6;
      break;
    case  ST:
      val = rs1;
      type = MEM;
      cycles = 5;
      break;
    default:
      error(message, __LINE__, __FILE__);
      break;
  }
  cycle_count = cycles;
  instr_count = pipeline->regs->IS_EX_IR[3]/4;
  }

  if (cycle_count == 1 && pipeline->regs->EX_WB_isEmpty)
  {
    pipeline->regs->EX_WB_IR = pipeline->regs->IS_EX_IR[3];
    pipeline->regs->EX_WB_ALUVAL = val;
    pipeline->regs->EX_WB_ALULOC = loc;
    pipeline->regs->EX_WB_TYPE = type;
    pipeline->regs->EX_WB_isEmpty = false;
    pipeline->regs->IS_EX_isEmpty[3] = true;
    pipeline->stall = false;
    pipeline->regs->EX_WB_count = pipeline->regs->IS_EX_count[3];
  }
  else
  {
    printf("STALL------------- cycles: %d\n", cycles);
    pipeline->stall = true;
    vector<int> probe = {tics, pipeline->regs->IS_EX_count[3], 9};
    (*c_prob).push_back(probe);
    if (cycle_count == 1) cycle_count++;
  }
  cycle_count--;
  printf("IR %d NPC %d\n", pipeline->regs->IS_EX_IR[3], pipeline->regs->IS_EX_NPC[3]);
  printf("----------------------------------------------------------------------------------------\n");
}
