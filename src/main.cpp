#include "main.h"
#include <cstdlib>

int main(int argc, char* argv[])
{
  RegFile*      RF           = NULL; /* register file                 */
  width_t*      mem          = NULL; /* memory                        */
  char*         instrs       = NULL; /* instructions to execute       */
  int           instr_count;         /* number of instructions        */
  char*         instr_file   = NULL; /* file containing assembly code */
  int           finished;            /* loop control                  */
  t_instr*      instructions = NULL; /* struct to hold int instrs     */
  t_instr_char* single       = NULL; /* struct to hold char instr     */
  t_dec_instr*  dec_ins      = NULL; /* struct to hold decoded instr  */

  /* parse command line */
  if (argc != 4)
  {
    format(argv[0]);
  }
  else {
    instr_file = argv[1];
    PIPELINE_C = atoi(argv[2]);
    BPREDTYPE = atoi(argv[3]);
  }

  Pipeline* pipeline = new Pipeline[PIPELINE_C];
  BranchTargetBuffer* btb = new BranchTargetBuffer;

  vector<string>* all_instrs = new vector<string>;

  /* initialise varaibles and read assembly instructions from file */
  initialise(&RF, &mem, instr_file, &instrs, &instr_count, &finished, &instructions, &single, &dec_ins);

  /* translate text instructions to decodable */
  translate_instrs(&mem, instrs, &instr_count, instructions, single, all_instrs);

  /* processor simulator: fetch, decode, issue, execute, writeback */
  process(RF, mem, instructions, &instr_count, &finished, dec_ins, all_instrs, pipeline, btb);

  /* free memory etc */
  delete btb;
  delete RF;
  delete [] instrs;
  delete [] mem;
  delete [] instructions;
  delete [] single;
  delete dec_ins;
  delete [] pipeline;

  return EXIT_SUCCESS;
}

int process(RegFile* RF, width_t* mem, t_instr* instructions, int* instr_count, int* finished, t_dec_instr* dec_ins, vector<string>* all_instrs, Pipeline* pipeline, BranchTargetBuffer* btb)
{
  int instr_c = 0;
  int i;
  int exe_finished = 0;
  int pc = 0;
  int tics = 0;
  *finished = 0;
  int not_halt = 0;

  vector<vector<int> >* c_comp = new vector<vector<int> >;
  vector<vector<int> >* c_prob = new vector<vector<int> >;

  bool stall = false;
  double tic  = wtime();
  while (!not_halt)
  {
    for (i = 0; i < PIPELINE_C; i++)
    {
      /* writeback results */
      pipeline[i].writeback->writeback(&pipeline[i], RF, c_comp, mem, &not_halt, &exe_finished, c_prob, tics);
    }

    stall = false;
    for (i = 0; i < PIPELINE_C; i++)
    {
      /* execute current instruction */
      pipeline[i].alu1->exe(&pipeline[i], c_comp, c_prob, tics);
      pipeline[i].alu2->exe(&pipeline[i], c_comp, c_prob, tics);
      if (!stall) pipeline[i].bzu->exe(&pipeline[i], &exe_finished, c_comp, c_prob, tics, btb);
      pipeline[i].ldstu->exe(&pipeline[i], mem, c_comp, c_prob, tics);
      /* check for flushes */
      flush(pipeline, c_comp, c_prob, RF, tics, &pc);
      /* check for stalls */
      stall = stall || pipeline[i].stall;
    }

    for (i = 0; i < PIPELINE_C; i++)
    {
      /* issue to particular execution unit if scoreboard checks out */
      if (!stall) pipeline[i].issue->issue(pipeline, i, instructions, c_comp, RF, c_prob, tics);
    }

    for(i = 0; i < PIPELINE_C; i++)
    {
      /* store current instruction in struct */
      if (!stall) pipeline[i].decode->decode(&pipeline[i], instructions, c_comp, c_prob, tics);
    }

    for(i = 0; i < PIPELINE_C; i++)
    {
      /* get instruction number */
      if (!exe_finished && !stall) pipeline[i].fetch->fetch(&pipeline[i], &instr_c, c_comp, instr_count, c_prob, &pc, tics, btb);
    }

    tics++;
    if (tics > 5000) break;
    printf("TIC TIC TIC ");
    printf("TIC TIC TIC ");
    printf("TIC TIC TIC\n");
  }
  double toc  = wtime();

  trace_file(c_comp, c_prob, instructions, all_instrs, tics);
  int total_b_count = 0;
  int total_mis_count = 0;

  for (int i = 0; i < PIPELINE_C; i++)
  {
    total_b_count += pipeline[i].bzu->b_count;
    total_mis_count += pipeline[i].bzu->mis_count;
  }

  /* memory dump */
  printf("----------------------------------------------------------------------------------------------\n");
  printf("RF:\n");
  printf("----------------------------------------------------------------------------------------------\n");
  for(int i = 0; i < NUM_REGS; i++)
  {
    printf("%2d: %6d  ", i, RF->RF[i]);
    if ((i+1)%8==0) printf("\n");
  }
  printf("----------------------------------------------------------------------------------------------\n");
  printf("Memory[0-63]:\n");
  printf("-------------------------------------------------------------\n");
  for(int i = 0; i < 64; i++)
  {
    printf("%d: %d\t", i, mem[i]);
    if ((i+1) % 8 == 0) printf("\n");
  }
  /* statistics */
  double instr_cycle = (double)instr_c/(double)tics;
  double pred_rate = 0;
  if (total_b_count != 0) pred_rate = (double)total_mis_count/(double)total_b_count;
  time_t rawtime;
  struct tm * timeinfo;
  time(&rawtime);
  timeinfo = localtime(&rawtime);
  printf("-------------------------------------------------------------\n");
  printf(" runtime: %.3lfs\n", toc - tic);
  printf(" cycles: %d\n", tics);
  printf(" instructions: %d\n", instr_c);
  printf(" instructions/cycle: %.3f\n", instr_cycle);
  printf(" Branch count: %d Misprediction count: %d\n", total_b_count, total_mis_count);
  printf(" Branch misprediction: %.3f%%\n", pred_rate*100);
  printf(" |Pipelines|: %d  Branch Prediction Type: %d\n", PIPELINE_C, BPREDTYPE);
  printf(" TIME: %s", asctime(timeinfo));
  printf("-------------------------------------------------------------\n");

  std::cout << std::endl;
  return EXIT_SUCCESS;
}

