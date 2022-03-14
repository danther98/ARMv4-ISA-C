/***************************************************************/
/*                                                             */
/*   ARMv4-32 Instruction Level Simulator                      */
/*                                                             */
/*   ECEN 4243                                                 */
/*   Oklahoma State University                                 */
/*                                                             */
/***************************************************************/

#ifndef _SIM_ISA_H_
#define _SIM_ISA_H_
#define N_CUR ( (CURRENT_STATE.CPSR>>31) & 0x00000001 )
#define Z_CUR ( (CURRENT_STATE.CPSR>>30) & 0x00000001 )
#define C_CUR ( (CURRENT_STATE.CPSR>>29) & 0x00000001 )
#define V_CUR ( (CURRENT_STATE.CPSR>>28) & 0x00000001 )
#define N_NXT ( (NEXT_STATE.CPSR>>31) & 0x00000001 )
#define Z_NXT ( (NEXT_STATE.CPSR>>30) & 0x00000001 )
#define C_NXT ( (NEXT_STATE.CPSR>>29) & 0x00000001 )
#define V_NXT ( (NEXT_STATE.CPSR>>28) & 0x00000001 )

#define N_N 0x80000000
#define Z_N 0x40000000
#define C_N 0x20000000
#define V_N 0x10000000

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"






int ADD (int Rd, int Rn, int Operand2, int I, int S, int CC) {

  int cur = 0;
  if(I == 0) {//if I ==0 then it is either Register, or Register shifted register, These depend on bit4 value.
    int sh = (Operand2 & 0x00000060) >> 5; //& # is the bit place in the command code
    int shamt5 = (Operand2 & 0x00000F80) >> 7;//>># is shifted value 
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 == 0) //if bit 4 value is 0 then it is register 
      switch (sh) {//sh is the shift direction 
      case 0: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] << shamt5);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
    	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] + 
	      ((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
	  break;
      }     
    else//if bit 4 value =1 then register shifted register (Look at Figure B.1 in appendix B)
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] + 
	      ((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
	  break;
      }      
  }
  if (I == 1) {//If I==1... is immediate 
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rn] + (Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
  }	
  return 0;

}

int AND (int Rd, int Rn, int Operand2, int I, int S, int CC){
int cur = 0;
  if(I == 0) {//if I ==0 then it is either Register, or Register shifted register, These depend on bit4 value.
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;//>># is shifted value 
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 == 0) //if bit 4 value is 0 then it is register 
      switch (sh) {//sh is the shift direction 
      case 0: cur = CURRENT_STATE.REGS[Rd] & 
	  (CURRENT_STATE.REGS[Rm] << shamt5);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rd] & 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rd] & 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
    	  break;
      case 3: cur = CURRENT_STATE.REGS[Rd] &
	      ((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
	  break;
      }     
    else//if bit 4 value =1 then register shifted register (Look at Figure B.1 in appendix B)
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rd] &
	  (CURRENT_STATE.REGS[Rn] << CURRENT_STATE.REGS[Rs]);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rd] &
	  (CURRENT_STATE.REGS[Rn] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rd] &
	  (CURRENT_STATE.REGS[Rn] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 3: cur = CURRENT_STATE.REGS[Rd] &
	      ((CURRENT_STATE.REGS[Rn] >> CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rn] << (32 - CURRENT_STATE.REGS[Rs])));
	  break;
      }      
  }
  if (I == 1) {//If I==1... is immediate 
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rn] & (Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
  }	
  return 0;
}

int BIC (int Rd, int Rn, int Operand2, int I, int S, int CC){
int cur = 0;
  if(I == 0) {//if I ==0 then it is either Register, or Register shifted register, These depend on bit4 value.
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;//>># is shifted value 
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 == 0) //if bit 4 value is 0 then it is register 
      switch (sh) {//sh is the shift direction 
      case 0: cur = CURRENT_STATE.REGS[Rn] &
	  (!(CURRENT_STATE.REGS[Rm] << shamt5));
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn]&
	  (!(CURRENT_STATE.REGS[Rm] >> shamt5));
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] &
	  (!(CURRENT_STATE.REGS[Rm] >> shamt5));
    	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] &
	      (!((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5))));
	  break;
      }     
    else//if bit 4 value =1 then register shifted register (Look at Figure B.1 in appendix B)
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] & 
	  (!(CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]));
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] &
	  (!(CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]));
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] &
	  (!(CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]));
	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] &
	      (!((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs]))));
	  break;
      }      
  }
  if (I == 1) {//If I==1... is immediate 
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rn] + (Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
  }	
  return 0;
}

int EOR (int Rd, int Rn, int Operand2, int I, int S, int CC){
int cur = 0;
  if(I == 0) {//if I ==0 then it is either Register, or Register shifted register, These depend on bit4 value.
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;//>># is shifted value 
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 == 0) //if bit 4 value is 0 then it is register 
      switch (sh) {//sh is the shift direction 
      case 0: cur = CURRENT_STATE.REGS[Rn] ^
	  (CURRENT_STATE.REGS[Rm] << shamt5);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] ^
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] ^
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
    	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] ^
	      ((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
	  break;
      }     
    else//if bit 4 value =1 then register shifted register (Look at Figure B.1 in appendix B)
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] ^ 
	  (CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] ^
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] ^
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] ^
	      ((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
	  break;
      }      
  }
  if (I == 1) {//If I==1... is immediate 
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rn] + (Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
  }	
  return 0;
}

int SUB (int Rd, int Rn, int Operand2, int I, int S, int CC){
int cur = 0;
  if(I == 0) {//if I ==0 then it is either Register, or Register shifted register, These depend on bit4 value.
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;//>># is shifted value 
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 == 0) //if bit 4 value is 0 then it is register 
      switch (sh) {//sh is the shift direction 
      case 0: cur = CURRENT_STATE.REGS[Rn] -
	  (CURRENT_STATE.REGS[Rm] << shamt5);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] -
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
    	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] -
	      ((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
	  break;
      }     
    else//if bit 4 value =1 then register shifted register (Look at Figure B.1 in appendix B)
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] - 
	      ((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
	  break;
      }      
  }
  if (I == 1) {//If I==1... is immediate 
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rn] - (Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
  }	
  return 0;
}

int MVN (int Rd, int Rn, int Operand2, int I, int S, int CC){
int cur = 0;
  if(I == 0) {//if I ==0 then it is either Register, or Register shifted register, These depend on bit4 value.
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;//>># is shifted value 
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 == 0) //if bit 4 value is 0 then it is register 
      switch (sh) {//sh is the shift direction 
      case 0: cur = ~CURRENT_STATE.REGS[Rn] << shamt5;
	  break;
      case 1: cur =  ~CURRENT_STATE.REGS[Rn]>> shamt5;
	  break;
      case 2: cur =  ~CURRENT_STATE.REGS[Rn]>>shamt5;
    	break;
      case 3: cur =  ~CURRENT_STATE.REGS[Rn]>> shamt5|~CURRENT_STATE.REGS[Rn]<<(32-shamt5);
	  break;
      }     
    else//if bit 4 value =1 then register shifted register (Look at Figure B.1 in appendix B)
      switch (sh) {
      case 0: cur =  ~CURRENT_STATE.REGS[Rn]<< CURRENT_STATE.REGS[Rs];
	  
	  break;
      case 1: cur =  ~CURRENT_STATE.REGS[Rn]>> CURRENT_STATE.REGS[Rs]; 
	  
	  break;
      case 2: cur =  ~CURRENT_STATE.REGS[Rn]>> CURRENT_STATE.REGS[Rs];
	  
	  break;
      case 3: cur =  ~CURRENT_STATE.REGS[Rn]>>CURRENT_STATE.REGS[Rs]|~CURRENT_STATE.REGS[Rn]<<(32-CURRENT_STATE.REGS[Rs]); 
	    
	  break;
      }      
  }
  if (I == 1) {//If I==1... is immediate 
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rn] + (Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
  }	
  return 0;
}



int LSL (int Rd, int Rn, int Operand2, int I, int S, int CC){
int cur = 0;
  if(I == 0) {//if I ==0 then it is either Register, or Register shifted register, These depend on bit4 value.
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;//>># is shifted value 
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 !=0 & sh==0){
      
      

      CURRENT_STATE.REGS[Rd] =  (CURRENT_STATE.REGS[Rn] << shamt5);
    }  
       
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
  }	
  return 0;
}

int LSR (int Rd, int Rn, int Operand2, int I, int S, int CC){
int cur = 0;
  if(I == 0) {//if I ==0 then it is either Register, or Register shifted register, These depend on bit4 value.
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;//>># is shifted value 
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 !=0 & sh==0x00000002){
      cur=CURRENT_STATE.REGS[Rm]>>Operand2;
    } 
       
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
  }	
  return 0;
}

int SBC (int Rd, int Rn, int Operand2, int I, int S, int CC){
int cur = 0;
  if(I == 0) {//if I ==0 then it is either Register, or Register shifted register, These depend on bit4 value.
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;//>># is shifted value 
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 == 0) //if bit 4 value is 0 then it is register 
      switch (sh) {//sh is the shift direction 
      case 0: cur = CURRENT_STATE.REGS[Rn] -
	  (CURRENT_STATE.REGS[Rm] << shamt5);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] -
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
    	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] -
	      ((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
	  break;
      }     
    else//if bit 4 value =1 then register shifted register (Look at Figure B.1 in appendix B)
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] - 
	      ((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
	  break;
      }      
  }
  if (I == 1) {//If I==1... is immediate 
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rn] + (Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
    if (cur>=0x100000000)
      NEXT_STATE.CPSR |= C_N;

  }	
  return 0;
}

int TST (int Rd, int Rn, int Operand2, int I, int S){
   int cur=0;
   if(I == 0) {//if I ==0 then it is either Register, or Register shifted register, These depend on bit4 value.
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;//>># is shifted value 
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;//test (sets flags based on Rn & Src2)
    if (S == 1) {
      cur=CURRENT_STATE.REGS[Rn]&Operand2;

    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
    if (cur>=0x100000000)
      NEXT_STATE.CPSR |= C_N;

      }	

    }  
}
int TEQ (int Rd, int Rn, int Operand2, int I, int S){//set flags based n Rn^Src2

int cur = 0;
  if (I==1){
    int rotate = Operand2>>8;
    int Imm = Operand2 & 0x000000FF;
    cur=CURRENT_STATE.REGS[Rn]^Operand2;
    }
    
    if(S==1){
      if(cur<0)
        NEXT_STATE.CPSR |=N_N;
      if(cur==0)
        NEXT_STATE.CPSR |=Z_N;
      if(cur>=10)
        NEXT_STATE.CPSR |=C_N;


    }




  }

int CMP (int Rd, int Rn, int Operand2, int I, int S, int CC){ //compare
  int cur = 0;
  if (I==1){
    int rotate = Operand2>>8;
    int Imm = Operand2 & 0x000000FF;
    cur=CURRENT_STATE.REGS[Rn]-Operand2;
    }
    
    if(S==1){
      if(cur<0)
        NEXT_STATE.CPSR |= N_N;
      if(cur==0)
        NEXT_STATE.CPSR |=Z_N;
      if(cur>=10)
        NEXT_STATE.CPSR |=C_N;


    }
}


int MUL (int Rd, int Rn, int Operand2, int I, int S, int CC){
  int cur = 0;
  if(I == 0) {//if I ==0 then it is either Register, or Register shifted register, These depend on bit4 value.
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;//>># is shifted value 
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x00F00000;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 == 0) //if bit 4 value is 0 then it is register 
      switch (sh) {//sh is the shift direction 
      case 0: cur = CURRENT_STATE.REGS[Rd] * 
	  (CURRENT_STATE.REGS[Rm] << shamt5);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rd] * 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rd] * 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
    	  break;
      case 3: cur = CURRENT_STATE.REGS[Rd] *
	      ((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
	  break;
      }
  }
  if (I == 1) {//If I==1... is immediate 
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rn] * (Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
  }	
  return 0;
}

int MLA (int Rd, int Ra, int Rm, int Rn){
  int cur=0;
  //int Rn=0x0000000F;
  //int Rm= 0x000000F0;
  //int Ra=0x0000F000;
  //int Rd=0x000F0000;
  //int buffer=0x00000090;
  cur=(CURRENT_STATE.REGS[Rn]*CURRENT_STATE.REGS[Rm])+CURRENT_STATE.REGS[Ra];
  NEXT_STATE.REGS[Rd]=cur;
return 0;

}


int ORR(int Rd, int Rn, int Operand2, int I, int S, int CC){//bitwise OR
int cur = 0;
  if(I == 0) {//if I ==0 then it is either Register, or Register shifted register, These depend on bit4 value.
    int sh = (Operand2 & 0x00000060) >> 5; //& # is the bit place in the command code
    int shamt5 = (Operand2 & 0x00000F80) >> 7;//>># is shifted value 
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 == 0) //if bit 4 value is 0 then it is register 
      switch (sh) {//sh is the shift direction 
      case 0: cur = CURRENT_STATE.REGS[Rn] | 
	  (CURRENT_STATE.REGS[Rm] << shamt5);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] |
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] |
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
    	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] | 
	      ((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
	  break;
      }     
    else//if bit 4 value =1 then register shifted register (Look at Figure B.1 in appendix B)
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] |
	  (CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] |
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] |
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] |
	      ((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
	  break;
      }      
  }
  if (I == 1) {//If I==1... is immediate 
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rn] | (Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
  }	
  return 0;

}



 

int STR (int Rn, int Rd, int Operand2, int Ibar){
//int cur=CURRENT_STATE.REGS[Rn]; //Change
int cur =0;
if(Ibar==1){
int Rm = Operand2 & 0x0000000F;
int bit4 = (Operand2 & 0x00000010) >> 4;
int sh = (Operand2 & 0x00000060) >> 5;
int shamt5 = (Operand2 & 0x00000F80) >> 7;
//uint32_t ad;
switch(sh){
     case 0: cur = (CURRENT_STATE.REGS[Rn] << shamt5); //Change
          //ad = CURRENT_STATE.REGS[Rn] + (CURRENT_STATE.REGS[Rm]<<shamt5);
    //cur =mem_read_32(ad);
    break;
     case 1: cur = (CURRENT_STATE.REGS[Rn] >> shamt5);
     //ad = CURRENT_STATE.REGS[Rn] + (CURRENT_STATE.REGS[Rm]>>shamt5);
     //cur =mem_read_32(ad);
    break;
     case 2: cur = (CURRENT_STATE.REGS[Rn] >> shamt5);
     //ad = CURRENT_STATE.REGS[Rn] + (CURRENT_STATE.REGS[Rm]>>shamt5);
     //cur =mem_read_32(ad);
    break;
     case 3: cur = ((CURRENT_STATE.REGS[Rn] >> shamt5)| (CURRENT_STATE.REGS[Rn] << shamt5));
      //ad = CURRENT_STATE.REGS[Rn] + ((CURRENT_STATE.REGS[Rm]>> shamt5)|(CURRENT_STATE.REGS[Rm]<<(32-shamt5)));
      //cur =mem_read_32(ad);
    break;
  }
  //cur; mem_read_32(ad);
  //NEXT_STATE.REGS[Rd]=cur;
  
}
else{ 
  int imm = Operand2 & 0x00000FFF;
 int cur=CURRENT_STATE.REGS[Rd]+CURRENT_STATE.REGS[Rn];
  NEXT_STATE.REGS[Rd]=cur+imm; //Change
  //cur; mem_write_32(Rd,imm);
  //NEXT_STATE.REGS[Rd]=cur;

}
return 0;
}

int LDR (int Rn, int Rd, int Operand2, int Ibar){
//int cur = CURRENT_STATE.REGS[Rn];
int cur=0;

if(Ibar==1){
int Rm = Operand2 & 0x0000000F;
int bit4 = (Operand2 & 0x00000010) >> 4;
int sh = (Operand2 & 0x00000060) >> 5;
int shamt5 = (Operand2 & 0x00000F80) >> 7;
switch(sh){
     case 0: cur=(CURRENT_STATE.REGS[Rn] << shamt5);
    break;
     case 1:cur=(CURRENT_STATE.REGS[Rn] >> shamt5);
      
    break;
     case 2: //CURRENT_STATE.REGS[Rd] = (cur >> shamt5);
     cur=(CURRENT_STATE.REGS[Rn] >> shamt5);
    break;
      case 3:// CURRENT_STATE.REGS[Rd] = ((cur >> shamt5) | (CURRENT_STATE.REGS[Rd] << shamt5));
      cur=((CURRENT_STATE.REGS[Rn] >> shamt5)| (CURRENT_STATE.REGS[Rn] << shamt5));
    break;
  }
  //NEXT_STATE.REGS[Rd] = CURRENT_STATE.REGS[Rd];
}
else{ 
int imm = Operand2 & 0x00000FFF;
//CURRENT_STATE.REGS[Rd]=CURRENT_STATE.REGS[Rn]+imm;
int cur=CURRENT_STATE.REGS[Rn]+CURRENT_STATE.REGS[Rd];
  NEXT_STATE.REGS[Rd]=cur+imm;
}
}



int ASR (char* i_);//Arithmetic shift right

int ROR (char* i_);//rotate right

int ADC (int Rd, int Rn, int Operand2, int I, int S, int CC){//add with carry
int cur = 0;
  if(I == 0) {//if I ==0 then it is either Register, or Register shifted register, These depend on bit4 value.
    int sh = (Operand2 & 0x00000060) >> 5; //& # is the bit place in the command code
    int shamt5 = (Operand2 & 0x00000F80) >> 7;//>># is shifted value 
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 == 0) //if bit 4 value is 0 then it is register 
      switch (sh) {//sh is the shift direction 
      case 0: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] << shamt5);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
    	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] + 
	      ((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
	  break;
      }     
    else//if bit 4 value =1 then register shifted register (Look at Figure B.1 in appendix B)
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] + 
	      ((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
	  break;
      }      
  }
  if (I == 1) {//If I==1... is immediate 
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rn] + (Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
  	if(cur>=0x100000000)
      NEXT_STATE.CPSR |= C_N;
      }
  return 0;
}


int LDRB (int Rn, int Rd, int Operand2, int Ibar ){
int cur = CURRENT_STATE.REGS[Rn & 0x000000FF]; //cur is getting memory adress of  Rn[7:0]
if(~Ibar==1){
int Rm = Operand2 & 0x0000000F;
int bit4 = (Operand2 & 0x00000010) >> 4;
int sh = (Operand2 & 0x00000060) >> 5;
int shamt5 = (Operand2 & 0x00000F80) >> 7;
switch(sh){
     case 0: CURRENT_STATE.REGS[Rd] = (cur << shamt5);
    break;
     case 1: CURRENT_STATE.REGS[Rd] = (cur >> shamt5);
    break;
     case 2: CURRENT_STATE.REGS[Rd] = (cur >> shamt5);
    break;
      case 3: CURRENT_STATE.REGS[Rd] = ((cur >> shamt5) | (CURRENT_STATE.REGS[Rd] << shamt5));
    break;
  }
  NEXT_STATE.REGS[Rd] = CURRENT_STATE.REGS[Rd];
}
else{ 
int imm = Operand2 & 0x00000FFF;
CURRENT_STATE.REGS[Rd]=CURRENT_STATE.REGS[Rn & 0x000000FF]+imm; //Rd is getting Rn[7:0]
  }
}


int MOV (int Rd, int Rn, int Operand2, int I, int S, int CC){
 int cur = 0;
  if (I == 1) {//If I==1... is immediate 
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rd] + (Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }else{  
    int Rm = Operand2 & 0x0000000F;
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    switch(sh){
      case 0: //LSL
      CURRENT_STATE.REGS[Rd]=CURRENT_STATE.REGS[Rm]<<shamt5;
      cur = CURRENT_STATE.REGS[Rd];
      break;
      case 1://LSR
      CURRENT_STATE.REGS[Rd]=CURRENT_STATE.REGS[Rm]>>shamt5;
      cur = CURRENT_STATE.REGS[Rd];
      break;
    }
  
   // cur=CURRENT_STATE.REGS[Rd] + (CURRENT_STATE.REGS[Rm]<<Operand2);

  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
  }	
  return 0;

}

 
      


int STRB (int Rn, int Rd, int Operand2, int Ibar){
int cur =0;
if(Ibar==1){
int Rm = Operand2 & 0x0000000F;
int bit4 = (Operand2 & 0x00000010) >> 4;
int sh = (Operand2 & 0x00000060) >> 5;
int shamt5 = (Operand2 & 0x00000F80) >> 7;
//rn=Rn&0x000000FF;
int rd=Rd&0x000000FF;
//uint32_t ad;

switch(sh){
     case 0: cur = (CURRENT_STATE.REGS[Rn] << shamt5)-24; //Change
          //ad = CURRENT_STATE.REGS[Rn] + (CURRENT_STATE.REGS[Rm]<<shamt5);
    //cur =mem_read_32(ad);
    break;
     case 1: cur = (CURRENT_STATE.REGS[Rn] >> shamt5)-24;
     //ad = CURRENT_STATE.REGS[Rn] + (CURRENT_STATE.REGS[Rm]>>shamt5);
     //cur =mem_read_32(ad);
    break;
     case 2: cur = (CURRENT_STATE.REGS[Rn] >> shamt5)-24;
     //ad = CURRENT_STATE.REGS[Rn] + (CURRENT_STATE.REGS[Rm]>>shamt5);
     //cur =mem_read_32(ad);
    break;
     case 3: cur = ((CURRENT_STATE.REGS[Rn] >> shamt5)| (CURRENT_STATE.REGS[Rn] << shamt5))-24;
      //ad = CURRENT_STATE.REGS[Rn] + ((CURRENT_STATE.REGS[Rm]>> shamt5)|(CURRENT_STATE.REGS[Rm]<<(32-shamt5)));
      //cur =mem_read_32(ad);
    break;
  }
  //cur; mem_read_32(ad);
  //NEXT_STATE.REGS[Rd]=cur;
  
}
else{ 
  int imm = Operand2 & 0x00000FFF;
 int cur=CURRENT_STATE.REGS[Rd]+CURRENT_STATE.REGS[Rn];
  NEXT_STATE.REGS[Rd]=cur+imm; //Change
  //cur; mem_write_32(Rd,imm);
  //NEXT_STATE.REGS[Rd]=cur;

}
return 0;
}


int B (int L, int Operand2){
  int imm;
  int PC8 = CURRENT_STATE.PC + 0x00000008;
  if(L==0){
    imm =  Operand2 & 0x00FFFFFF;
    CURRENT_STATE.PC = ((CURRENT_STATE.REGS[PC8]+imm)<<2);  
  }
}



int BL (int L, int Operand2){
  int imm;
  int PC8 = CURRENT_STATE.PC + 0x00000008;
  if(L==1){
    imm =  Operand2 & 0x00FFFFFF;
    CURRENT_STATE.PC = ((CURRENT_STATE.REGS[PC8]+imm)<<2);  
  }

}








int CMN (int Rn, int Operand2, int S, int I){ // cond 1011 signed less than flag: ge: 19:16
  int cur = CURRENT_STATE.REGS[Rn]+Operand2;
  int LT = cur & 0x000F0000;
  int N = CURRENT_STATE.CPSR & 0x80000000;
  int C = CURRENT_STATE.CPSR & 0x40000000;
  int Z = CURRENT_STATE.CPSR & 0x20000000;
  int V = CURRENT_STATE.CPSR & 0x10000000;
if(S==1){
    NEXT_STATE.CPSR |=N_N;
    NEXT_STATE.CPSR |=V_N;
    NEXT_STATE.CPSR = LT;
  }	

}

//compare negative



int SWI (char* i_){
  //int i;
  //for(i=0;i>7;i++){
  //  if(i_[i]=='0'){
  //    if(i_[i]-1=='A'){
  //      return 1;

      

    return 0;
  
}
#endif




//if(i_[strlen(i_)-1]='A' && i_[strlen(i_)-2]='0'){







