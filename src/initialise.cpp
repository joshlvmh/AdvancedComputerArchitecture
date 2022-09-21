#include "initialise.h"

int initialise(RegFile** RF, width_t** mem, char* instr_file, char** instrs, int* instr_count, int* finished, t_instr** instructions, t_instr_char** single, t_dec_instr** dec_ins)
{
  FILE* fp;           /* file pointer        */
  char message[1024]; /* error buffer        */
  char ch;            /* character buffer    */
  int lines;          /* instruction counter */
  int i;              /* loop counter        */
  int lptr;           /* line position       */
  int cptr;           /* character position  */

  /* open instruction file */
  fp = fopen(instr_file, "r");
  if (fp == NULL)
  {
    sprintf(message, "could not open file: %s", instr_file);
    error(message, __LINE__, __FILE__);
  }

  /* count lines in file */
  lines = 0;
  while((ch=fgetc(fp))!=EOF) {
    if(ch=='\n') lines++;
  }

  /* reset file pointer */
  fseek(fp, 0, SEEK_SET);

  /* allocate memory */
  *instrs = new char[lines*INSTR_BUFF];
  *mem = new width_t[MEM_SIZE];
  *instructions = new t_instr[lines];
  *single = new t_instr_char[40];
  *dec_ins = new t_dec_instr;
  *RF = new RegFile;

  /* read in instructions */
  for (i=0;i<lines;i++)
  {
    lptr = i*INSTR_BUFF;
    cptr = 0;
    while(((ch=fgetc(fp)) != '\n'))
    {
      if (ch == EOF) break;
      (*instrs)[lptr + cptr] = ch;
      cptr++;
    }
  }

  /* initialise variables */
  for (i = 0; i < NUM_REGS; i++)
  {
    (*RF)->RF[i] = NULL_INT;
    (*RF)->scoreboard[i] = 1;
    (*RF)->Qi[i] = 0;
  }
  (*RF)->RF[0] = 0; // r0 always = 0

  *finished                 = 0;
  *instr_count              = lines;

  /* close file */
  fclose(fp);

  return EXIT_SUCCESS;
}
