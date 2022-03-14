# ARMv4-ISA-C
This code is a ARMv4 ISA created in C++

This code is incomplete, some functions do not work properly and need trouble shooting.


 --ISA--
  
  -This ISA includes the following commands  
   - ADC ADD AND ASR B BIC
   - BL CMN CMP EOR LDR LDRB
   - LSL LSR MOV MVN ORR ROR
   - SBC STR STRB SUB TEQ TST
   - SWI
  
  -Using the make file you are able to compile the sim.c file into a sim.exe, with this you are able to simulate commands input from the .x files in inputs.
  -to compile ARM commands into hex codes, use the arm2hex file.
  
  
  --Sim Commands--
 - 'go' -Runs program to completion
 - 'run n' -Executes program for n instructions
 - 'mdump low high' -Dump memory from low to high
 - 'rdump' -Dump the registers and bus values
 - 'input reg_num reg_val' -set GRP reg_num to reg_val
 - '?' -Displays help menu
 - 'quit' -Exit the program
  
   
