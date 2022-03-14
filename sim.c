/***************************************************************/
/*                                                             */
/*   ARMv8-32 Instruction Level Simulator                      */
/*                                                             */
/*   ECEN 4243                                                 */
/*   Oklahoma State University                                 */
/*                                                             */
/***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"
#include "isa.h"


char *byte_to_binary12 (int x) {

  static char b[13];
  b[0] = '\0';

  int z;
  for (z = 2048; z > 0; z >>= 1) {
    strcat(b, ((x & z) == z) ? "1" : "0");
  }

  return b;
}

char *byte_to_binary4 (int x) {

  static char b[5];
  b[0] = '\0';

  int z;
  for (z = 8; z > 0; z >>= 1) {
    strcat(b, ((x & z) == z) ? "1" : "0");
  }

  return b;
}

char *byte_to_binary32(int x) {

  static char b[33];
  b[0] = '\0';

  unsigned int z;
  for (z = 2147483648; z > 0; z >>= 1) {
    strcat(b, ((x & z) == z) ? "1" : "0");
  }

  return b;
}

int bchar_to_int(char* rsa) {

  int i = 0;
  int result = 0;
  int t = 0;
  while(rsa[i] != '\0')i++;
  while(i>0)
    {
      --i;
      // printf("%d\n", (rsa[i]-'0')<<t);
      result += (rsa[i] - '0')<<t;
      t++;
    }
  return result;
}


int data_process(char* i_) {

  /*
    This function further decode and execute subset of data processing 
    instructions of ARM ISA.

    0000 = AND - Rd:= Op1 AND Op2
    0001 = EOR - Rd:= Op1 EOR Op2
    0010 = SUB - Rd:= Op1 - Op2
    0011 = RSB - Rd:= Op2 - Op1
    0100 = ADD - Rd:= Op1 + Op2
    0101 = ADC - Rd:= Op1 + Op2 + C
    0110 = SBC - Rd:= Op1 - Op2 + C - 1
    0111 = RSC - Rd:= Op2 - Op1 + C - 1
    1000 = TST - set condition codes on Op1 AND Op2 
    1001 = TEQ - set condition codes on Op1 EOR Op2 
    1010 = CMP - set condition codes on Op1 - Op2 
    1011 = CMN - set condition codes on Op1 + Op2 
    1100 = ORR - Rd:= Op1 OR Op2
    1101 = MOV - Rd:= Op2
    1110 = BIC - Rd:= Op1 AND NOT Op2 
    1111 = MVN - Rd:= NOT Op2
  */

  char d_opcode[5];//cmd
  d_opcode[0] = i_[7]; 
  d_opcode[1] = i_[8]; 
  d_opcode[2] = i_[9]; 
  d_opcode[3] = i_[10]; 
  d_opcode[4] = '\0';
  char d_cond[5];
  d_cond[0] = i_[0]; 
  d_cond[1] = i_[1]; 
  d_cond[2] = i_[2]; 
  d_cond[3] = i_[3]; 
  d_cond[4] = '\0';
  char rn[5]; rn[4] = '\0';
  char rd[5]; rd[4] = '\0';
  char operand2[13]; operand2[12] = '\0';
  for(int i = 0; i < 4; i++) {
    rn[i] = i_[12+i];
    rd[i] = i_[16+i];
  }
  for(int i = 0; i < 12; i++) {
    operand2[i] = i_[20+i];
  }
  char d_op[3];
  d_op[0]=i_[4];
  d_op[1]=i_[5];
  d_op[2]='\0';
  int Rn = bchar_to_int(rn);
  int Rd = bchar_to_int(rd);
  int Operand2 = bchar_to_int(operand2);
  int I = i_[6]-'0';
  int S = i_[11]-'0';
  int CC = bchar_to_int(d_cond);
  printf("Opcode = %s\n Rn = %d\n Rd = %d\n Operand2 = %s\n I = %d\n S = %d\n COND = %s", d_opcode, Rn, Rd, byte_to_binary12(Operand2), I, S, byte_to_binary4(CC));
  printf("\n");

 
 
  if(!strcmp(d_opcode,"0000")) {
    printf("--- This is an AND instruction. \n\n\n");
    AND(Rd, Rn, Operand2, I, S, CC);
    return 0;
  }

  if(!strcmp(d_opcode,"0001")) {
    printf("--- This is an EOR instruction. \n\n\n");
    EOR(Rd, Rn, Operand2, I, S, CC);
    return 0;
  }

  if(!strcmp(d_opcode,"0010")) {
    printf("--- This is an SUB instruction. \n\n\n");
    SUB(Rd, Rn, Operand2, I, S, CC);
    return 0;
  }
  

  if(!strcmp(d_opcode,"0100")) {
    printf("--- This is an ADD instruction. \n\n\n");
    ADD(Rd, Rn, Operand2, I, S, CC);
    return 0;
  }	

  if(!strcmp(d_opcode,"0101")) {
    printf("--- This is an ADC instruction. \n\n\n");
    ADC(Rd, Rn, Operand2, I, S, CC);
    return 0;
  }
  if(!strcmp(d_opcode,"0110")) {
    printf("--- This is an SBC instruction. \n\n\n");
    SBC(Rd, Rn, Operand2, I, S, CC);
    return 0;
  }
  
  if(!strcmp(d_opcode,"1000")) {
    printf("--- This is an TST instruction. \n\n\n");
    TST(Rd, Rn, Operand2, I, S);
    return 0;
  }
  if(!strcmp(d_opcode,"1001")) {
    printf("--- This is an TEQ instruction. \n\n\n");
    TEQ(Rd, Rn, Operand2, I, S);
    return 0;
  }
  if(!strcmp(d_opcode,"1010")) {
    printf("--- This is an CMP instruction. \n\n\n");
    CMP(Rd, Rn, Operand2, I, S, CC);
    return 0;
  }
  if(!strcmp(d_opcode,"1011")) {
    printf("--- This is an CMN instruction. \n\n\n");
    CMN( Rn, Operand2, S, I);
    return 0;
  }
  if(!strcmp(d_opcode,"1100")) {
    if(!strcmp(d_op,"00")){
    printf("--- This is an ORR instruction. \n\n\n");
   ORR(Rd, Rn, Operand2, I, S, CC);
    return 0;
    }
  }
  
  if(!strcmp(d_opcode,"1101")) {
    printf("--- This is an MOV instruction. \n\n\n");
    MOV(Rd, Rn, Operand2, I, S, CC);
    return 0;
  }

    /* this stuff is wrong, but I was trying to get the other types of shifts to work
    because they all have the same opcode as mov it is causing problems

    if(!I==1){
    int sh =Operand2&0x00000060 >>5;
    switch(sh){s
      case 0:dss
      LSL(Rd,Rn,Operand2,I,S,CC);
      printf("--- This is an LSL instruction. \n\n\n");
      return 0;
	  break;
      case 1: 
      LSR(Rd,Rn,Operand2,I,S,CC);
      printf("--- This is an LSR instructison. \n\n\n");
      return 0;
	  break;
      }
    }else{
  printf("--- This is an MOV instruction. \n\n\n");
    MOV(Rd, Rn, Operand2, I, S, CC);
    return 0;
    
        }
  }
*/

  
  
  if(!strcmp(d_opcode,"1110")) {
    if(!strcmp(d_op,"00")){
    printf("--- This is an BIC instruction. \n\n\n");
   BIC(Rd, Rn, Operand2, I, S, CC);
    return 0;
    }
  }
if(!strcmp(d_opcode,"1111")) {
    printf("--- This is an MVN instruction. \n\n\n");
    MVN(Rd, Rn, Operand2, I, S, CC);
    return 0;
  }

if(!strcmp(d_opcode,"1000")) {
    printf("--- This is an SWI instruction. \n\n\n");
    interruption_process(i_);
    return 0;
  }

  
  return 1;	
}

int branch_process(char* i_) {
  char b_opcode[5];
  b_opcode[0]=i_[0];
  b_opcode[1]=i_[1];
  b_opcode[2]=i_[2];
  b_opcode[3]=i_[3];
  b_opcode[4]='\0';
  char operand2[24]; operand2[23]='\0';
  for(int i = 0; i < 24;i++){
    operand2[i]=i_[8+i];
  }
  int Operand2=bchar_to_int(operand2);

  int L = i_[7];
if(L==1) {
    printf("--- This is an BL instruction. \n\n\n");
    BL(L, Operand2);
    return 0;
  }else{
    printf("--- This is an B instruction. \n\n\n");
     B(L, Operand2);
     return 0;

  }
  /* This function execute branch instruction */

  /* Adds branch sinstructisons here s*/ 

  return 1;

}

int mul_process(char* i_) {
  char m_opcode[4];
  m_opcode[0] = i_[8]; 
  m_opcode[1] = i_[9]; 
  m_opcode[2] = i_[10]; 
  m_opcode[3] = '\0' ;

  char m_cond[5];
  m_cond[0] = i_[0]; 
  m_cond[1] = i_[1]; 
  m_cond[2] = i_[2]; 
  m_cond[3] = i_[3]; 
  m_cond[4] = '\0';

  char buff[5];
  buff[0] = i_[24];
  buff[1] = i_[25];
  buff[2] = i_[26];
  buff[3] = i_[27];
  buff[4] = '\0';

  char rn[5]; rn[4] = '\0';
  char rd[5]; rd[4] = '\0';
  char ra[5]; ra[4] = '\0';
  char rm[5]; rm[4] = '\0';
  char operand2[13]; operand2[12] = '\0';
  for(int i = 0; i < 12; i++) {
    operand2[i] = i_[20+i];
  }
  int Operand2 = bchar_to_int(operand2);
  for(int i = 0; i < 4; i++) {
    rd[i] = i_[12+i];
    ra[i] = i_[16+i];
    rm[i] = i_[20+i];
    rn[i] = i_[28+i];

  }
  int I = i_[6]-'0';
  
  
  int Rn = bchar_to_int(rn);
  int Rd = bchar_to_int(rd);
  int Ra = bchar_to_int(ra);
  int Rm = bchar_to_int(rm);
  printf("Rn = %d\n Rd = %d\n Ra = %d\n Rm= %d\n",Rn,Rd,Ra,Rm);

 
  int S = i_[11]-'0';
  int CC = bchar_to_int(m_cond);
  printf("Opcode = %s\n Rn = %d\n Rd = %d\n Operand2 = %s\n S = %d\n COND = %s\n", m_opcode, Rn, Rd, S, byte_to_binary4(CC));
  printf("\n");

/* 
000 = MUL - Rd: = Rn * Rm 
001 = MLA - Rd: = ( Rn * Rm ) + Ra
*/
if(!strcmp(m_opcode,"000")) {
  printf("--- This is an MUL instruction. \n");
  MUL(Rd, Ra, Operand2, I, S, CC);
  return 0;
}
if(!strcmp(m_opcode,"000")) {
  printf("--- This is an MLA instruction. \n");
  MLA(Rd, Ra, Rm, Rn);
  return 0;
}
  /* This function execute multiply instruction */

  /* Add multiply instructions here s*/ 

  return 1;

}

int transfer_process(char* i_) {

char t_opcode[3];//cmd
  t_opcode[0] = i_[4]; 
  t_opcode[1] = i_[5]; 
  t_opcode[2] = '\0';
  char t_cond[5];//cmd
  //t_cond[0] = i_[0]; 
  //t_cond[1] = i_[1]; 
  //t_cond[2] = i_[2]; 
  //t_cond[3] = i_[3]; s
  //t_cond[4] = '\0';


i_[0]='\0';
i_[1]='\0';
i_[2]='\0';
i_[3]='\0';
i_[4]='\0';
  char rn[5]; rn[4] = '\0';
  char rd[5]; rd[4] = '\0';
  char operand2[13]; operand2[12] = '\0';
  for(int i = 0; i < 4; i++) {
    rn[i] = i_[12+i];
    rd[i] = i_[16+i];
  }
  for(int i = 0; i < 12; i++) {
    operand2[i] = i_[20+i];
  }
  int Rn = bchar_to_int(rn);
  int Rd = bchar_to_int(rd);
  int Ibar = byte_to_binary12(Ibar);

  int Operand2 = bchar_to_int(operand2);
  char memid[7];
   Ibar = i_[6]-'0';//I bar
  memid[1] = i_[7]-'0';//P
  memid[2] = i_[8]-'0';//U
  int b = i_[9]-'0';//B
  memid[4] = i_[10]-'0';//Ws
  int l = i_[11]-'0';//Ld
  memid[6]='\0';
  int CC = bchar_to_int(t_cond);
  int mem_id = bchar_to_int(memid);
  printf("Opcode = %s\n Rn = %d\n Rd = %d\n Operand2 = %s\n Ibar = %d\n P = %d\n U = %d\n B = %d\n W = %d\n L = %d\n COND = %s\n", t_opcode, Rn, Rd, byte_to_binary12(Operand2), memid[0], memid[1],memid[2],memid[3],memid[4],l, byte_to_binary4(CC));
  printf("\n");
  /* This function execute smemory instructions ss*/

  /* Add memory instructions here */ 
if(!strcmp(t_opcode,"01")){
  printf("T_opcode is correct. \n");
  if((b==0)&(l==0)) {
  printf("--- This is an STR instruction. \n");
  STR(Rn, Rd, Operand2, Ibar);
  return 0;
  }
}
if(!strcmp(t_opcode,"01")){
  printf("T_opcode is correct. \n");
  if((b==0)&(l==1)) {
  printf("--- This is an LDR instruction. \n");
  LDR(Rn, Rd, Operand2, Ibar);
  
  return 0;
  }
}
if(!strcmp(t_opcode,"01")){
  printf("T_opcode is correct. \n");
  if((b==1)&(l==1)) {
  printf("--- This is an LDRB instruction. \n");
  LDRB(Rn, Rd, Operand2, Ibar);
  return 0;
  }
}
if(!strcmp(t_opcode,"01")){
  printf("T_opcode is correct. \n");
  if((b==1)&(l==0)) {
  printf("--- This is an STRB instruction. \n");
  STRB(Rn, Rd, Operand2, Ibar);
  return 0;
  }
}
  return 0;

}

int interruption_process(char* i_) {


  SWI(i_);
  RUN_BIT = 0;
  
  
    
  
 //SWI(i_);ss
   
 
  
  
  return 0;

}

unsigned int COND(unsigned int i_word) {

  return (i_word>>28);

}

unsigned int OPCODE(unsigned int i_word) {

  return ((i_word<<7)>>27);

}


int decode_and_execute(char* i_) {

  /* 
     This function decode the instruction and update s
     CPU_State (NEXT_STATE)ss
  */

  if((i_[4] == '1') && (i_[5] == '0') && (i_[6] == '1')) {
    printf("- This is a Branch Instruction. \n");
    branch_process(i_);
  }

  if((i_[4] == '0') && (i_[5] == '0') && (i_[6] == '0') && (i_[7] == '0') && (i_[24] == '1') && (i_[25] == '0') && (i_[26] == '0') && (i_[27] == '1')) {
    printf("- This is a Multiply Instruction. \n");
    mul_process(i_);
  }
 
  if((i_[4] == '0') && (i_[5] == '1')) {
    printf("- This is a Single Data Transfer Instruction. \n");
    transfer_process(i_);
  }

  if((i_[4] == '1') && (i_[5] == '1') && (i_[6] == '1') && (i_[7] == '1')) {
    printf("- This is a Software Interruption Instruction. \n");
    interruption_process(i_);
  }

   else {
    printf("- This is a Data Processing Instruction. \n");
    data_process(i_);
  }
  return 0;

}

void process_instruction() {

  /* 
     execute one instruction here. You should use CURRENT_STATE and modify
     values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
     access memory. 
  */   
unsigned int inst_word = mem_read_32(CURRENT_STATE.PC);

  
  printf("The instruction is: %x \n", inst_word);
  printf("33222222222211111111110000000000\n");
  printf("10987654321098765432109876543210\n");
  printf("--------------------------------\n");
  printf("%s \n", byte_to_binary32(inst_word));
  printf("\n");
  decode_and_execute(byte_to_binary32(inst_word));

  NEXT_STATE.PC += 4;

}
