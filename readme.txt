Advanced Computer Architecture, University of Bristol

Processor simulator in C++.

To compile:
  make clean
  make all
To run:
  make run BENCHMARK=<INSTR_FILE> PIPELINE=<PIPELINE_COUNT> BPPR_TYPE=<BRANCH_PREDICTOR>

Output: bin/simulator.out

Where 
    <INSTR_FILE> is a file containing the assembly instructions, with no empty lines, located in banchmarks/
    <PIPELINE_COUNT> is a numerical value, e.g. 2
    <BRANCH_PREDICTION> is a numerical value within the range 0-3,
        0 - Fixed, always taken
        1 - Static, always taken if backwards
        2 - Dynamic, 1-bit
        3 - Dynamic, 2-bit

eg:
  make all
  make run BENCHMARK=bubble PIPELINES=3 BRPR_TYPE=0
  make run BENCHMARK=vec_add PIPELINES=1 BRPR_TYPE=3
  make run BENCHMARK=ham_weight PIPELINES=2 BRPR_TYPE=2
