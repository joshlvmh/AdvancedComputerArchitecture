#include "decode.h"

void Decode::decode(Pipeline* pipeline, t_instr* instructions, vector<vector<int> >* c_comp, vector<vector<int> >* c_prob, int tics)
{
  if (pipeline->regs->IF_ID_isEmpty) return;
  if (!pipeline->regs->ID_IS_isEmpty) return;
  if (pipeline->stall) return;
  printf("----------------------------------------------------------------------------------------\n");
  printf("DECODE\n");
  int counter = pipeline->regs->IF_ID_IR / 4;
  pipeline->regs->ID_IS_OPC = instructions[counter].instr; /* read in opcode [0-16] */
  pipeline->regs->ID_IS_NPC = pipeline->regs->IF_ID_NPC;
  pipeline->regs->ID_IS_IR = pipeline->regs->IF_ID_IR;
  pipeline->regs->IF_ID_isEmpty = true;
  pipeline->regs->ID_IS_isEmpty = false;

  char message[1024];                            /* error buffer          */
  switch(pipeline->regs->ID_IS_OPC)
  {
    case ADD:
    case SUB:
    case MUL:
    case AND:
    case  OR:
    case XOR:
    case LDS:
      pipeline->regs->ID_IS_OUT->rd = instructions[counter].op1;
      pipeline->regs->ID_IS_OUT->rs1 = instructions[counter].op2;
      pipeline->regs->ID_IS_OUT->rs2 = instructions[counter].op3;
      break;
    case BEQ:
    case BNQ:
      pipeline->regs->ID_IS_OUT->rs1 = instructions[counter].op1;
      pipeline->regs->ID_IS_OUT->rs2 = instructions[counter].op2;
      pipeline->regs->ID_IS_OUT->pcrel = instructions[counter].op3;
      break;
    case JMP:
      pipeline->regs->ID_IS_OUT->rd = instructions[counter].op1;
      pipeline->regs->ID_IS_OUT->pcrel = instructions[counter].op2;
      break;
    case LD:
      pipeline->regs->ID_IS_OUT->rd = instructions[counter].op1;
      pipeline->regs->ID_IS_OUT->rs1 = instructions[counter].op2;
      break;
    case LDI:
      pipeline->regs->ID_IS_OUT->rd = instructions[counter].op1;
      pipeline->regs->ID_IS_OUT->imm = instructions[counter].op2;
      break;
    case ST:
      pipeline->regs->ID_IS_OUT->rd = instructions[counter].op2;
      pipeline->regs->ID_IS_OUT->rs1 = instructions[counter].op1;
      break;
    case CMP:
    case SHR:
    case SHL:
      pipeline->regs->ID_IS_OUT->rd = instructions[counter].op3;
      pipeline->regs->ID_IS_OUT->rs1 = instructions[counter].op1;
      pipeline->regs->ID_IS_OUT->rs2 = instructions[counter].op2;
      break;
    case HLT:
      break;
    default:
      sprintf(message, "opcode not recognised: %d", pipeline->regs->ID_IS_OPC);
      error(message, __LINE__, __FILE__);
      break;
  }
  switch(pipeline->regs->ID_IS_OPC)
  {
    case ADD:
    case SUB:
    case MUL:
    case AND:
    case OR:
    case XOR:
    case CMP:
    case SHR:
    case SHL:
      pipeline->regs->ID_IS_OUT->type = T_ALU1;
      break;
    case BEQ:
    case BNQ:
    case HLT:
    case JMP:
      pipeline->regs->ID_IS_OUT->type = T_BZ;
      break;
    case LD:
    case LDI:
    case LDS:
    case ST:
      pipeline->regs->ID_IS_OUT->type = T_LDST;
      break;
    default:
      sprintf(message, "opcode not recognised: %d", pipeline->regs->ID_IS_OPC);
      error(message, __LINE__, __FILE__);
      break;
  }
  pipeline->regs->ID_IS_OUT->opcode = pipeline->regs->ID_IS_OPC;

  (*c_comp)[pipeline->regs->IF_ID_count][1] = tics;
  pipeline->regs->ID_IS_count = pipeline->regs->IF_ID_count;
  printf("IR: %d NPC: %d\n", pipeline->regs->IF_ID_IR, pipeline->regs->IF_ID_NPC);
  printf("----------------------------------------------------------------------------------------\n");

}
