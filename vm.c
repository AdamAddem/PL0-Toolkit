/*
  Assignment:
  HW4 - Complete Parser and Code Generator for PL/0
        (with Procedures, Call, and Else)

  Author(s): <Ernesto Lugo>, <Anthony Casseus>

  Language: C (only)

  To Compile:
    Scanner:
      gcc -O2 -std=c11 -o lex lex.c
    Parser/Code Generator:
      gcc -O2 -std=c11 -o parsercodegen_complete parsercodegen_complete.c
    Virtual Machine:
      gcc -O2 -std=c11 -o vm vm.c

  To Execute (on Eustis):
    ./lex <input_file.txt>
    ./parsercodegen_complete
    ./vm elf.txt

  where:
    <input_file.txt> is the path to the PL/0 source program

  Notes:
    - lex.c accepts ONE command-line argument (input PL/0 source file)
    - parsercodegen_complete.c accepts NO command-line arguments
    - Input filename is hard-coded in parsercodegen_complete.c
    - Implements recursive-descent parser for extended PL/0 grammar
    - Supports procedures, call statements, and if-then-else
    - Generates PM/0 assembly code (see Appendix A for ISA)
    - VM must support EVEN instruction (OPR 0 11)
    - All development and testing performed on Eustis

  Class: COP3402 - System Software - Fall 2025

  Instructor: Dr. Jie 
  
  Due Date: Friday, November 21, 2025 at 11:59 PM ET
*/
#include <stdio.h>
#include <string.h>





/*----- ENUMERATIONS -----*/
enum IR_SEGMENTS
{
  OP,
  L,
  M
};

enum INSTRUCTIONS
{
  LIT = 1,
  OPR,
  LOD,
  STO,
  CAL,
  INC,
  JMP,
  JPC,
  SYS = 9
};

enum OPERATIONS
{
  RTN = 0,
  ADD,
  SUB,
  MUL,
  DIV,
  EQL,
  NEQ,
  LSS,
  LEQ,
  GTR,
  GEQ,
  EVEN = 11
};

enum SYSCALLS
{
  PRINT = 1,
  READ,
  HALT = 3
};
/*----- ENUMERATIONS -----*/





int PAS[500];

//registers
int PC = 499;
int SP;
int BP;
int IR[3];



/* Find base L levels down from the current activation record */
int base (int base, int numlevels) 
{
  int arb = base;

  while (numlevels>0) 
  {
    arb = PAS[arb];
    numlevels--;
  }

  return arb;
}


int main(int argc, char* argv[])
{
  /*----- Opening and Verifying File -----*/
  FILE* fp = fopen("elf.txt", "r");
  if(fp == NULL)
  {
    printf("File unable to be opened\n");
    return 1;
  }
  /*----- Opening and Verifying File -----*/
  

  /*----- Loading Text Segment -----*/
  while(fscanf(fp, "%d %d %d", &PAS[PC], &PAS[PC-1], &PAS[PC-2]) == 3)
    PC -= 3;

  BP = PC;
  SP = BP + 1;
  PC = 499;

  fclose(fp);
  /*----- Loading Text Segment -----*/

  //headers
  printf("\n\tL\tM    %s   %s   %s   %s\n", "PC", "BP", "SP", "stack");
  printf("Initial values:\t   %5d%5d%5d\n", PC, BP, SP);

  int ARS[100]; //stupid stupid stupid stupid stupid stupid stupid stupid
  memset(ARS, 0, 100*sizeof(int));
  int topARs = 0;

  
  /*----- Main Loop -----*/
  int continueProgram = 1;
  while(continueProgram)
  {
    /* Fetch */
    IR[OP] = PAS[PC];
    IR[L] = PAS[--PC];
    IR[M] = PAS[--PC];
    --PC;
    
    /* Execute */
    switch(IR[OP])
    {
      case LIT:
        SP -= 1;
        PAS[SP] = IR[M];
        printf("LIT");
        break;


      case LOD:
        PAS[--SP] = PAS[base(BP, IR[L]) - IR[M]];
        printf("LOD");
        break;

      case STO:
        PAS[base(BP, IR[L]) - IR[M]] = PAS[SP];
        SP = SP + 1;
        printf("STO");
        break;

      case CAL:
        PAS[SP-1] = base(BP, IR[L]);
        PAS[SP-2] = BP;
        PAS[SP-3] = PC;
        BP = SP - 1;
        PC = 499 - IR[M];

        ARS[topARs] = BP;
        topARs++;
        printf("CAL");
        break;

      case INC:
        SP -= IR[M];
        printf("INC");
        break;

      case JMP:
        PC = 499 - IR[M];
        printf("JMP");
        break;

      case JPC:
        if(PAS[SP] == 0) PC = 499 - IR[M];
        SP += 1;
        printf("JPC");
        break;

      case SYS:
      
        if(IR[M] == PRINT)
        {
          printf("Output result is: %d\n", PAS[SP++]);
          printf("SYS");
        }
        else if(IR[M] == READ)
        {
          printf("Please Enter an Integer: ");
          int input;
          scanf("%d", &input);
          PAS[--SP] = input;
          printf("SYS");
        }
        else // HALT
        {
          printf("SYS");
          continueProgram = 0;
        }
        break;
      

      case OPR:
        switch(IR[M])
        {
          case RTN:
            SP = BP + 1;
            BP = PAS[SP-2];
            PC = PAS[SP-3];
            ARS[topARs] = 0;
            --topARs;
            printf("RTN");
            break;

          case ADD:
            PAS[SP+1] = PAS[SP+1] + PAS[SP];
            ++SP;
            printf("ADD");
            break;

          case SUB:
            PAS[SP+1] = PAS[SP+1] - PAS[SP];
            ++SP;
            printf("SUB");
            break;

          case MUL:
            PAS[SP+1] = PAS[SP+1] * PAS[SP];
            ++SP;
            printf("MUL");
            break;

          case DIV:
            PAS[SP+1] = PAS[SP+1] / PAS[SP];
            ++SP;
            printf("DIV");
            break;

          case EQL:
            PAS[SP+1] = PAS[SP+1] == PAS[SP];
            ++SP;
            printf("EQL");
            break;

          case NEQ:
            PAS[SP+1] = PAS[SP+1] != PAS[SP];
            ++SP;
            printf("NEQ");
            break;

          case LSS:
            PAS[SP+1] = PAS[SP+1] < PAS[SP];
            ++SP;
            printf("LSS");
            break;

          case LEQ:
            PAS[SP+1] = PAS[SP+1] <= PAS[SP];
            ++SP;
            printf("LEQ");
            break;

          case GTR:
            PAS[SP+1] = PAS[SP+1] > PAS[SP];
            ++SP;
            printf("GTR");
            break;

          case GEQ:
            PAS[SP+1] = PAS[SP+1] >= PAS[SP];
            ++SP;
            printf("GEQ");
            break;

          case EVEN:
            PAS[SP] = (PAS[SP] % 2 == 0);
            printf("EVEN");
            break;

          default:
            printf("How did you get here? %d %d %d\n", IR[OP], IR[L], IR[M]);
            return 1;
            break;
        }
        break;

      default:
        return 1;
        break;
    }


    /* Printing */
    printf("\t%d\t%-2d %5d%5d%5d  ", IR[L], IR[M], PC, BP, SP);

    int baseOfStack;
    //finds # of activation records for printing purposes
    for(int ARs=0; ; ++ARs)
    {
      if(base(BP, ARs) == 0)
      {
        baseOfStack = base(BP, --ARs);
        break;
      }
    }




    //weird code that prints from bottom to top of stack cause yall wanted that for some reason
    //if printing the BP of an AR, adds | for formatting
    int tmp = topARs;
    int tmp2 = 0;
    for(int i=baseOfStack; i>=SP; --i)
    {
      if(ARS[tmp2] == i && tmp2 <topARs)
      {
        printf("| ");
        ++tmp2;
      }
      printf("%-2d ", PAS[i]);
    }


    printf("\n");
    
  }
  /*----- Main Loop -----*/




  return 0;
}