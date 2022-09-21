#ifndef _SHARE_H
#define _SHARE_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;

#define WORD_SIZE  32
#define INSTR_BUFF 64
#define MEM_SIZE   1024
#define PC_INC     4
#define NUM_REGS   64
#define NULL_INT   (-10000)
#define ROBSize    64

extern int PIPELINE_C;
extern int BPREDTYPE;

/* instruction set */
#define ADD  0
#define SUB  1
#define MUL  2
#define AND  3
#define OR   4
#define XOR  5
#define BEQ  6
#define BNQ  7
#define JMP  8
#define LD   9
#define LDI 10
#define LDS 11
#define ST  12
#define CMP 13
#define SHR 14
#define SHL 15
#define HLT 16

/* instruction type */
#define T_ALU1 0
#define T_ALU2 4
#define T_LDST 1
#define T_BZ 2
#define REG 0
#define MEM 1
#define STOP 3

typedef int32_t width_t;

typedef struct
{
  char* instr;
  char* op1;
  char* op2;
  char* op3;
} t_instr_char;

typedef struct
{
  int instr;
  int op1;
  int op2;
  int op3;
} t_instr;

typedef struct
{
  int opcode;
  int rd;
  int rs1;
  int rs2;
  int imm;
  int pcrel;
  int type;
} t_dec_instr;

class Pipeline;
class Fetch;
class Decode;
class Execute;
class RegFile;
class PipelineRegs;
class BranchTargetBuffer;
class ReservationStation;

class Fetch {
  public:
    bool first = true;
    void fetch(Pipeline* pipeline, int* instr_c, vector<vector<int>>* c_comp, int* instr_count, vector<vector<int>>* c_prob, int* pc, int tics, BranchTargetBuffer* btb);
};

class Decode {
  public:
    void decode(Pipeline* pipeline, t_instr* instructions, vector<vector<int>>* c_comp, vector<vector<int>>* c_prob, int tics);
};

class Execute {
  public:
    Execute();
    virtual ~Execute();
    virtual void exe();
};

class Writeback {
  public:
    void writeback(Pipeline* pipeline, RegFile* RF, vector<vector<int> >* c_comp, width_t* mem, int* finished, int* exe_finished, vector<vector<int>>* c_prob, int tics);
};

class ALU1:Execute {
  public:
    int cycles;
    int cycle_count;
    int loc;
    int val;
    int type;
    int instr_count;
    int count_trace;
    ALU1();
    void exe(Pipeline* pipeline, vector<vector<int>>* c_comp, vector<vector<int>>* c_prob, int tics);
};

class ALU2:Execute {
  public:
    int cycles;
    int cycle_count;
    int loc;
    int val;
    int type;
    int instr_count;
    int count_trace;
    ALU2();
    void exe(Pipeline* pipeline, vector<vector<int>>* c_comp, vector<vector<int>>* c_prob, int tics);
};

class LDSTU:Execute {
  public:
    int cycles;
    int cycle_count;
    int loc;
    int val;
    int type;
    int instr_count;
    int count_trace;
    LDSTU();
    void exe(Pipeline* pipeline, width_t* mem, vector<vector<int>>* c_comp, vector<vector<int>>* c_prob, int tics);
};

class BZU:Execute {
  public:
    int cycles;
    int b_count;
    int mis_count;
    int cycle_count;
    int loc;
    int type;
    int flush;
    int branch;
    int instr_count;
    int bz_inum;
    int count_trace;
    BZU();
    int pc_res;
    void exe(Pipeline* pipeline, int* finished, vector<vector<int>>* c_comp, vector<vector<int>>* c_prob, int tics, BranchTargetBuffer* btb);
};

class Issue {
  public:
    bool traced = false;
    void issue(Pipeline* pipeline, int i, t_instr* instructions, vector<vector<int>>* c_comp, RegFile* RF, vector<vector<int>>* c_prob, int tics);
};

class Pipeline {
  public:
    bool stall;
    bool flush;
    Fetch* fetch;
    Decode* decode;
    Issue* issue;
    Writeback* writeback;
    ALU1* alu1;
    ALU2* alu2;
    LDSTU* ldstu;
    BZU* bzu;
    ReservationStation* aluRS;
    ReservationStation* bzuRS;
    ReservationStation* ldstuRS;
    PipelineRegs* regs;
    Pipeline();
    ~Pipeline();
};

class PipelineRegs {
  public:
    int IF_ID_IR;
    int IF_ID_NPC;
    int ID_IS_IR;
    int ID_IS_NPC;
    int ID_IS_OPC;
    int ID_IS_AGE;
    t_dec_instr* ID_IS_OUT;
    int IS_EX_RD[4] = {0,0,0,0};
    int IS_EX_RS1[4] = {0,0,0,0};
    int IS_EX_RS2[4] = {0,0,0,0};
    int IS_EX_IMM[4] = {0,0,0,0};
    int IS_EX_OPC[4] = {0,0,0,0};
    int IS_EX_NPC[4] = {0,0,0,0};
    int IS_EX_IR[4] = {0,0,0,0};
    int EX_WB_IR;
    int EX_WB_ALUVAL;
    int EX_WB_ALULOC;
    int EX_WB_COND;
    int EX_WB_TYPE;
    int IF_ID_count;
    int ID_IS_count;
    int IS_EX_count[4];
    int EX_WB_count;
    bool IF_ID_isEmpty;
    bool ID_IS_isEmpty;
    bool IS_EX_isEmpty[4] = {true, true, true, true};
    bool EX_WB_isEmpty;
    PipelineRegs();
    ~PipelineRegs();
};

class RegFile {
  public:
    width_t* RF;
    int* scoreboard;
    int* Qi;
    RegFile();
    ~RegFile();
};

class ReservationStation {
  public:
    int Op;
    int Qj;
    int Qk;
    int Vj;
    int Vk;
    int A;
    int busy = 0;
};

class BranchTargetBuffer {
  public:
    int tail = 0;
    int* BZ_PC;
    int* BZ_T;
    int* TAR;
    int* RES1;
    int* RES2;
    int* LAST;

    BranchTargetBuffer();
    ~BranchTargetBuffer();
    void addRes(int PC, int res, int tar, int type);
    int pullRes(int pc);
    bool checkTar(int pc, int tar);
};

/* utility functions */
void error(const char* message, const int line, const char* file);
void format(const char* input);
double wtime(void);

#endif
