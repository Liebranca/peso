// ---   *   ---   *   ---
// deps

  #include <stdio.h>
  #include <stdlib.h>
  #include <stdint.h>

  #include <string.h>

// ---   *   ---   *   ---
// CONST

  #define F_ESCAPED 0x01
  #include "bits/flags.h"

  #define EXPLEN    0x40

// ---   *   ---   *   ---
// GBL

static uint64_t rd_flags      = 0x00;
static uint64_t token_mem[4]  = {0};
static size_t   token_ptr     = 0;

static char**   expr          = NULL;
static size_t   expr_ptr      = 0;

// ---   *   ---   *   ---

void terminate_token(void) {

  if(token_ptr) {

    char* buff=(char*) token_mem;

    // copy over to heap
    strcpy(expr[expr_ptr],buff);
    expr_ptr++;

    // reset token
    memset(buff,0,token_ptr);
    token_ptr=0;

  };

};

// ---   *   ---   *   ---

void rd(char* input) {

  while(*input) {

    char escd=rd_flags&F_ESCAPED;
    char match;

    #include "bits/cases.c"

// ---   *   ---   *   ---

TAIL:

    char c=*input;
    switch (c) {

      case 0x5C:

        if(!escd) {
          rd_flags|=F_ESCAPED;
          break;

        };

      default:
        rd_flags&=~F_ESCAPED;
        break;

    };

    input++;

  };

  terminate_token();

};

// ---   *   ---   *   ---

void main(void) {

  // setup
  expr=(char**) malloc(
    EXPLEN*sizeof(char*)

  );

  for(int x=0;x<EXPLEN;x++) {
    expr[x]=(char*) malloc(

      (sizeof(uint64_t)*4)
    * sizeof(char)

    );

  };

// ---   *   ---   *   ---

  char* input="x,y,2\n";
  rd(input);

// ---   *   ---   *   ---
// cleanup

  for(int x=0;x<EXPLEN;x++) {

    if(strlen(expr[x])) {
      printf("%s\n",expr[x]);

    };

    free(expr[x]);

  };

  free(expr);

};

// ---   *   ---   *   ---
