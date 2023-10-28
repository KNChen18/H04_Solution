//
//  Author: Kristina Chen
//  Course: CSS 448: Intro to Compilers
//  Assignment: Homework 4, creating a recognizer
//  Date: 10/27/2023
//
//  Functions to create:
//  int main();
//  void mustbe(TokKind k);
//  void pause();
//  void rexAsg();
//  void rexExp();
//  void rexIf();
//  void rexProg();
//  void rexStm();
//  void rexVorC();

#include <stdio.h>
#include <stdlib.h>

// Global Variables
int tokNum = 0; // Current Token number within toks
int hiTokNum = 17; // Highest Token number within toks

typedef enum { TOKVAR = 1, TOKEQ, TOKCONST, TOKSEMI, TOKADD, TOKIF, TOKTHEN } TokKind;

// TestGood, to use TestBad change [8] TOKVAR to TOKTHEN
TokKind toks[] =
        { TOKVAR, TOKEQ, TOKCONST, TOKSEMI, // a = 3;
          TOKVAR, TOKEQ, TOKVAR, TOKADD, TOKVAR, TOKSEMI, // b = a + a;
          TOKIF, TOKVAR, TOKTHEN, // if a then
          TOKVAR, TOKEQ, TOKVAR, TOKADD, TOKCONST // x = a + 5
        };

const char* tokToStr(TokKind k) {
    switch (k) {
        case TOKVAR: {return "TOKVAR";}
        case TOKEQ: {return "TOKEQ";}
        case TOKCONST: {return "TOKCONST";}
        case TOKSEMI: {return "TOKSEMI";}
        case TOKADD: {return "TOKADD";}
        case TOKIF: {return "TOKIF";}
        case TOKTHEN: {return "TOKTHEN";}
        default: {return "UNKNOWN";}
    }
}

void pause() {
    printf("Invalid program syntax \n");
    printf("Press Enter to exit... \n");
    getchar(); // Wait for the user to press Enter (despite input)
    exit(1);
}

void mustbe(TokKind k) {
    TokKind t = toks[tokNum];
    if (t == k) {
        //printf("Found & expected = %s\n", tokToStr(t), tokToStr(k)); // This code is used simply for debugging
        ++tokNum;
    } else {
        printf("Error: want %s, but found %s \n", tokToStr(t), tokToStr(k));
        pause();
    }
}

// VorC: VorC => Var | Const
void rexVorC() {
    if (tokNum <= hiTokNum) {
        TokKind t = toks[tokNum];
        if (t == TOKVAR || t == TOKCONST) {
            //printf("Processing %s\n", tokToStr(t));   // Line only used for testing
            ++tokNum; // Consume the token and move to the next one
        } else {
            printf("Error: want variable or constant, but found %s\n", tokToStr(t));
            pause(); // Report an error and exit the program on unexpected tokens
        }
    } else {
        printf("Error: Unexpected end of input\n");
        pause(); // Report an error and exit the program on unexpected tokens
    }
}

// Expression: Exp => VorC | VorC + VorC
void rexExp() {
    rexVorC();
    while (toks[tokNum] == TOKADD) {
        mustbe(TOKADD);
        rexVorC();
    }
}

// Assignment: Asg => Var = Exp
void rexAsg() {
    mustbe(TOKVAR);
    mustbe(TOKEQ);
    rexExp();
}

// If: if Exp then Asg
void rexIf() {
    mustbe(TOKIF);
    rexExp();
    mustbe(TOKTHEN);
    rexAsg();
}

// Statement: Stm => Asg | If
void rexStm() {
    if (toks[tokNum] == TOKVAR) {
        rexAsg();
    } else if (toks[tokNum] == TOKIF) {
        rexIf();
    }
}

// Program: Prog => Stm; Prog | Stm
void rexProg() {
    while (tokNum < hiTokNum) {
        if (toks[tokNum] == TOKSEMI) {
            mustbe(TOKSEMI);
        }
        if (tokNum == hiTokNum) {
            rexStm(); // Process the last statement without a semicolon
        }
        rexStm();
    }


}

int main() {
    printf("Starting program recognition...\n");
    rexProg();
    printf("Valid program!\n");
    printf("Press enter to exit.");
    getchar();
}


