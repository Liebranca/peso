// ---   *   ---   *   ---
// deps

  #include <stdio.h>
  #include <stdlib.h>
  #include <stdint.h>

  #include <string.h>
  #include <errno.h>

  #include "bits/hash.c"

// ---   *   ---   *   ---
// CONST

  #define F_ESCAPED 0x01
  #include "bits/flags.h"

  #define EXPLEN    0x40
  #define MEMTK     0x0400
  #define MEMSZ     0x4000

// ---   *   ---   *   ---
// GBL

  static uint64_t rd_flags      = 0x00;
  static uint64_t token_mem[4]  = {0};
  static size_t   token_ptr     = 0;

  static char**   expr          = NULL;
  static size_t   expr_ptr      = 0;
  static size_t   expr_len      = 0;
  static size_t   expr_cnt      = 0;

  static char**   expr_arr      = NULL;
  static char*    expr_mem      = NULL;
  static size_t   mem_ptr       = 0;

  static uint64_t skip_end      = 0x00;
  static void*    strtab        = NULL;

// ---   *   ---   *   ---
// open-read-close

char* orc(const char* fpath) {

  FILE* f=fopen(fpath,"rb");

  if(f==NULL) {
    fprintf(stderr,"%s\n",strerror(errno));
    exit(1);

  };

  fseek(f,0,SEEK_END);
  size_t sz=ftell(f);

  rewind(f);

  char* buff=(char*) malloc(sz*sizeof(char));
  fread(buff,sizeof(char),sz,f);

  fclose(f);

  return buff;

};

// ---   *   ---   *   ---

uint64_t tryfet(char* key) {

  int len=strlen(key);

  char* cpy=(char*) malloc(
    sizeof(char)*len

  );

  strcpy(cpy,key);

  for(int i=0;i<len;i++) {
    char c=cpy[i];
    c-=0x20*(c>=0x61 && c<=0x7A);

    cpy[i]=c;

  };

  void* value=hash_get(strtab,cpy);
  if(value==NULL) {
    value=hash_get(strtab,key);

    if(value!=NULL) {
      printf("CASE   %s : %016X\n",key,value);

    };

  } else {
    printf("NOCASE %s : %016X\n",key,value);

  };

  free(cpy);
  return 0;

};

// ---   *   ---   *   ---

void terminate_token(void) {

  if(token_ptr) {

    char* buff=(char*) token_mem;

    tryfet(buff);

    // copy over to heap
    strcpy(expr[expr_ptr],buff);
    expr_ptr++;
    expr_len+=token_ptr;

    // reset token
    memset(buff,0,token_ptr);
    token_ptr=0;

  };

};

// ---   *   ---   *   ---

void terminate_expr(void) {

  expr_arr[expr_cnt]=expr_mem+mem_ptr;

  uint32_t* idx=(uint32_t*) (expr_mem+mem_ptr);
  *idx=expr_ptr;

  mem_ptr+=4;

  for(size_t i=0;i<expr_ptr;i++) {
    char* buff=expr[i];
    strcpy(expr_mem+mem_ptr,buff);

    mem_ptr+=strlen(buff)+1;

  };

  expr_ptr=0;
  expr_len=0;

  expr_cnt++;

};

// ---   *   ---   *   ---

void rd(char* input) {

  while(*input) {

    char c=*input;

    char escd=rd_flags&F_ESCAPED;
    char match;

    if(rd_flags&F_SKIP) {

      input++;

      if(c==skip_end) {
        rd_flags&=~F_SKIP;

      };

      continue;

    };

    #include "bits/cases.c"

// ---   *   ---   *   ---

TAIL:

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

};

// ---   *   ---   *   ---

void main(void) {

  // setup
  strtab=nthash(4);
  #include "bits/loadkeyw.c"

  expr_mem=(char*) malloc(
    MEMSZ*sizeof(char)

  );

  expr_arr=(char**) malloc(
    MEMTK*sizeof(char**)

  );

  memset(expr_mem,0,MEMSZ*sizeof(char));

// ---   *   ---   *   ---
// ^continued

  expr=(char**) malloc(
    EXPLEN*sizeof(char*)

  );

  for(int x=0;x<EXPLEN;x++) {
    size_t sz=(sizeof(uint64_t)*4)*sizeof(char);

    expr[x]=(char*) malloc(sz);
    memset(expr[x],0,sz);

  };

// ---   *   ---   *   ---

  char* input=orc("tests/parse_this.pe");
  rd(input);

  for(int i=0;i<expr_cnt;i++) {
    uint32_t idx=*((uint32_t*) (expr_arr[i]));
    char* buff=expr_arr[i]+4;

    for(int j=0;j<idx;j++) {
      printf("%s\n",buff);
      buff+=strlen(buff)+1;

    };

    printf("_______\n\n");

  };

// ---   *   ---   *   ---
// cleanup

  free(input);
  for(int x=0;x<EXPLEN;x++) {
    free(expr[x]);

  };

  free(expr);

  free(expr_arr);
  free(expr_mem);

  free(strtab);

};

// ---   *   ---   *   ---
