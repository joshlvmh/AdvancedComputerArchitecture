#include <stdio.h>
#include "translate.h"


int translate_line(int count, char* instrs, t_instr* instructions, t_instr_char* single, vector<string>* all_instrs)
{
  char line[INSTR_BUFF];     /* line buffer        */
  char message[1024];        /* error buffer       */
  char *word;                /* string buffer      */
  int loop=0;                /* loop counter       */
  const char delim[2] = " "; /* delimeter          */
  int ret = 0;               /* instruction return */
  int op_re1 = 0;            /* operand 1 return   */
  int op_re2 = 0;            /* operand 2 return   */
  int op_re3 = 0;            /* operand 3 return   */

  string line_vec;
  /* copy over single instruction */
  for(int i=0; i<INSTR_BUFF; i++)
  {
    line[i] = instrs[i+count*INSTR_BUFF];
    if (line[i] <= 122 && line[i] >= 32) line_vec = line_vec + line[i];
  }
  (*all_instrs).push_back(line_vec);

  /* clear previous instruction */
  single->instr = NULL;
  single->op1   = NULL;
  single->op2   = NULL;
  single->op3   = NULL;

  /* find first word */
  word = strtok(line, delim);

  /* load full instruction */
  single->instr = word;
  while( word != NULL  ) {
        word = strtok(NULL, delim);
        switch(loop)
        {
          case 0:
                single->op1 = word; break;
          case 1:
                single->op2 = word; break;
          case 2:
                single->op3 = word; break;
          default:
                if (word == NULL) break;
                sprintf(message, "too many operands: %d", loop);
                error(message, __LINE__, __FILE__); break;
        }
        loop++;
  }

  /* translate string instructions to integers */
  ret = _operator(single->instr);
  op_re1 = _operand(single->op1);
  op_re2 = _operand(single->op2);
  op_re3 = _operand(single->op3);

  /* copy to struct for all instructions */
  instructions[count].instr = ret;
  instructions[count].op1   = op_re1;
  instructions[count].op2   = op_re2;
  instructions[count].op3   = op_re3;

  return EXIT_SUCCESS;
}

int _operator(char* op)
{
  char message[1024];  /* error buffer */

  /* string comparands */
  const char add[4]  = "add";
  const char sub[4]  = "sub";
  const char mul[4]  = "mul";
  const char _and[4] = "and";
  const char _or[4]  = "or" ;
  const char _xor[4] = "xor";
  const char beq[4]  = "beq";
  const char bnq[4]  = "bnq";
  const char jmp[4]  = "jmp";
  const char  ld[4]  = "ld" ;
  const char ldi[4]  = "ldi";
  const char lds[4]  = "lds";
  const char  st[4]  = "st" ;
  const char cmp[4]  = "cmp";
  const char shr[4]  = "shr";
  const char shl[4]  = "shl";
  const char hlt[4]  = "hlt";

  /* string comparisons */
  if      (strcasecmp(op, add)  == 0) return  ADD;
  else if (strcasecmp(op, sub)  == 0) return  SUB;
  else if (strcasecmp(op, mul)  == 0) return  MUL;
  else if (strcasecmp(op, _and) == 0) return  AND;
  else if (strcasecmp(op,  _or) == 0) return   OR;
  else if (strcasecmp(op, _xor) == 0) return  XOR;
  else if (strcasecmp(op, beq)  == 0) return  BEQ;
  else if (strcasecmp(op, bnq)  == 0) return  BNQ;
  else if (strcasecmp(op, jmp)  == 0) return  JMP;
  else if (strcasecmp(op,  ld)  == 0) return   LD;
  else if (strcasecmp(op, ldi)  == 0) return  LDI;
  else if (strcasecmp(op, lds)  == 0) return  LDS;
  else if (strcasecmp(op,  st)  == 0) return   ST;
  else if (strcasecmp(op, cmp)  == 0) return  CMP;
  else if (strcasecmp(op, shr)  == 0) return  SHR;
  else if (strcasecmp(op, shl)  == 0) return  SHL;
  else if (strcasecmp(op, hlt)  == 0) return  HLT;

  sprintf(message,"Instruction not recognised: %s", op);
  error(message, __LINE__, __FILE__ );
  return EXIT_FAILURE;
}

int _operand(char* op) 
{
  /* if empty, return 0 (decode will differentiate between r0, imm0 and null) */
  if (op == NULL) return 0;

  int counter = strlen(op);  /* loop counter    */
  int reg     = 0;           /* register flag   */
  int res     = 0;           /* operand value   */
  int digits  = 0;           /* base 10 counter */
  int neg     = 0;

  /* check if register operand */
  if ((op[0] == 82) | (op[0] == 114)) reg = 1;

  /* calculate operand value */
  while(1)
  {
    counter--;
    /* end of instruction */
    if (counter < 0) break;
    /* skip R letter */
    if (counter == 0 && reg == 1) continue;
    /* skip any commas */
    if (op[counter] == 44) continue;
    /* account for negative numbers */
    if (op[counter] == 45){ neg = 1; continue;}
    /* calculate value in base 10 */
    res = res + (op[counter]-48) * pow(10,digits);
    digits++;

  }
  if (neg == 1) res = res * -1;
  return res;
}

int translate_instrs(width_t** mem, char* instrs, int* instr_count, t_instr* instructions, t_instr_char* single, vector<string>* all_instrs)
{
  for(int i=0; i<*instr_count; i++)
  {
    translate_line(i, instrs, instructions, single, all_instrs);
  }
  return EXIT_SUCCESS;
}
