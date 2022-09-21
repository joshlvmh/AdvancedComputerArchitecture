#include "share.h"

/* utility functions */
int PIPELINE_C = 1;
int BPREDTYPE = 3;

void error(const char* message, const int line, const char* file)
{
  fprintf(stderr, "Error at line %d of file %s:\n", line, file);
  fprintf(stderr, "%s\n", message);
  fflush(stderr);
  exit(EXIT_FAILURE);
}

void format(const char* input)
{
  fprintf(stderr, "\nCorrect format: make all run BENCHMARK=<instruction_file> PIPELINES=<pipeline_count> BR-PR-TYPE=<branch_predictor>\n or %s <instruction_file> <pipeline_count> <branch_predictor>\n", input);
  exit(EXIT_FAILURE);
}

double wtime(void)
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec + tv.tv_usec * 1e-6;
}

Execute::Execute() {}
Execute::~Execute() {}
void Execute::exe() {}

ALU1::ALU1() {
  cycles = 0;
  cycle_count = 0;
  loc = 0;
  val = 0;
  type = 0;
  instr_count=0;
  count_trace = 0;
}
ALU2::ALU2() {
  cycles = 0;
  cycle_count = 0;
  loc = 0;
  val = 0;
  type = 0;
  instr_count=0;
  count_trace = 0;
}
LDSTU::LDSTU() {
  cycles = 0;
  cycle_count = 0;
  loc = 0;
  val = 0;
  type = 0;
  instr_count=0;
  count_trace = 0;
}
BZU::BZU() {
  b_count = 0;
  mis_count = 0;
  branch = 0;
  flush = 0;
  cycle_count = 0;
  cycles = 0;
  loc = 0;
  type = 0;
  instr_count=0;
  bz_inum = 0;
  count_trace = 0;
}

PipelineRegs::PipelineRegs()
{
  IF_ID_isEmpty = true;
  ID_IS_isEmpty = true;
  EX_WB_isEmpty = true;
  ID_IS_AGE=-1;
  ID_IS_OUT = new t_dec_instr;
  IF_ID_IR = 0;
  IF_ID_NPC = 0;
  ID_IS_IR = 0;
  ID_IS_NPC = 0;
  ID_IS_OPC = 0;
  ID_IS_AGE = 0;
  EX_WB_IR = 0;
  EX_WB_ALUVAL = 0;
  EX_WB_ALULOC = 0;
  EX_WB_COND = 0;
  EX_WB_TYPE = 0;

  IF_ID_count = -1;
  ID_IS_count = -1;
  IS_EX_count[0] = -1;
  IS_EX_count[1] = -1;
  IS_EX_count[2] = -1;
  IS_EX_count[3] = -1;
  EX_WB_count = -1;
}

PipelineRegs::~PipelineRegs() {
  delete ID_IS_OUT;
}

Pipeline::Pipeline() {
  fetch = new Fetch;
  decode = new Decode;
  issue = new Issue;
  writeback = new Writeback;
  alu1 = new ALU1;
  alu2 = new ALU2;
  ldstu = new LDSTU;
  bzu = new BZU;
  regs = new PipelineRegs;

  stall = false;
  flush = false;
}

Pipeline::~Pipeline() {
  delete fetch;
  delete decode;
  delete writeback;
  delete issue;
  delete alu1;
  delete alu2;
  delete bzu;
  delete ldstu;
  delete regs;
}

RegFile::RegFile() {
  RF = new width_t[NUM_REGS];
  scoreboard = new int[NUM_REGS];
  Qi = new int[NUM_REGS];
}

RegFile::~RegFile() {
  delete [] RF;
  delete [] scoreboard;
  delete [] Qi;
}

BranchTargetBuffer::BranchTargetBuffer() {
  BZ_PC = new int[INSTR_BUFF];
  BZ_T = new int[INSTR_BUFF];
  TAR = new int[INSTR_BUFF];
  RES1 = new int[INSTR_BUFF];
  RES2 = new int[INSTR_BUFF];
  LAST = new int[INSTR_BUFF];
  for (int i = 0; i < INSTR_BUFF; i++)
  {
    BZ_PC[i] = -1;
    BZ_T[i] = -1;
    TAR[i] = -1;
    RES1[i] = -1;
    RES2[i] = -1;
  }
}

BranchTargetBuffer::~BranchTargetBuffer() {
  delete [] BZ_PC;
  delete [] BZ_T;
  delete [] TAR;
  delete [] RES1;
  delete [] RES2;
  delete [] LAST;
}
