// ---   *   ---   *   ---
// deps

  #include <stdio.h>
  #include <stdlib.h>
  #include <stdint.h>

  #include <string.h>
  #include <errno.h>

  #include "bits/array.c"
  #include "bits/hash.c"
  #include "bits/tree.c"

// ---   *   ---   *   ---
// CONST

  #define F_ESCAPED 0x01
  #include "bits/flags.h"

  #define EXPLEN    0x40
  #define MEMTK     0x0400
  #define MEMSZ     0x0800

// ---   *   ---   *   ---
// GBL

  static uint64_t   rd_flags     = 0x00;
  static uint64_t   token_mem[4] = {0};
  static size_t     token_ptr    = 0;

  static uint64_t*  expr         = NULL;
  static size_t     expr_ptr     = 0;
  static size_t     expr_cnt     = 0;

  static uint64_t** expr_arr     = NULL;
  static uint64_t*  expr_mem     = NULL;
  static size_t     mem_ptr      = 0;

  static uint64_t   skip_end     = 0x00;
  static Strtab*    strtab       = NULL;

  static char*      pe_blk_name  = "non";
  static void*      mempool      = NULL;
  static size_t     mempool_ptr  = 0x00;

  #include "bits/xforms.c"
  #include "bits/xlate_tab.c"

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

  uint64_t out=0;

  // make local copy of key
  int len=strlen(key);
  char* cpy=(char*) malloc(
    sizeof(char)*len

  );

  strcpy(cpy,key);

  // up the case
  for(int i=0;i<len;i++) {
    char c=cpy[i];
    c-=0x20*(c>=0x61 && c<=0x7A);

    cpy[i]=c;

  };

// ---   *   ---   *   ---

  // is case-insensitive keyword
  out=hash_get_idex(strtab,cpy);

  if(!out) {

    // is case-sensitive keyword
    out=hash_get_idex(strtab,key);

    // is regular token
    if(!out) {

      uint64_t idex=hash_set(strtab,key,NULL);

      strtab->elems[idex].value=(void*) (
        idex+strtab->bsz

      );

      out=(uintptr_t) strtab->elems[idex].value;

    };

  };

// ---   *   ---   *   ---

  free(cpy);
  return out;

};

// ---   *   ---   *   ---

void terminate_token(void) {

  if(token_ptr) {

    char* buff=(char*) token_mem;
    uint64_t n=tryfet(buff);

    // copy over to heap
    expr[expr_ptr++]=n;

    // reset token
    memset(buff,0,strlen(buff));
    token_ptr=0;

  };

};

// ---   *   ---   *   ---

void terminate_expr(void) {

  expr_arr[expr_cnt]=expr_mem+mem_ptr;
  expr_mem[mem_ptr++]=expr_ptr;

  for(size_t i=0;i<expr_ptr;i++) {
    *(expr_mem+(mem_ptr++))=expr[i];

  };

  expr_ptr=0;
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

void tryipret(

  uint64_t  tcnt,
  uint64_t* buff

) {

  uint64_t t=*buff++;

  if(t<strtab->bsz) {

    Hashed* h=strtab->elems+t;
    uint64_t first=(uint64_t) h->value;

    #include "bits/xlate_switch.c"

  };

};

// ---   *   ---   *   ---

void main(void) {

  Array ar;array_nit(&ar);
  array_push(&ar,24);

//  Tree* trees=(Tree*) malloc(0x40*sizeof(Tree));
//  memset(trees,0,0x40*sizeof(Tree));
//
//  trees[0].value="HEYHEY";
//  trees[1].value="WHOAWHOA";
//  trees[2].value="EHEH";
//  trees[3].value="TTTEHEH";
//
//  Tree* ar[]=NTL(trees+1,trees+2,trees+3);
//
//  tree_push(trees,ar);
//  tree_prich(trees);
//
//  free(trees);
  exit(0);

  mempool=malloc(0x4000);
  memset(mempool,0,0x4000);

  // setup
  strtab=nthash(4);
  #include "bits/loadkeyw.c"

  expr_mem=(uint64_t*) malloc(
    MEMSZ*sizeof(uint64_t)

  );

  expr_arr=(uint64_t**) malloc(
    MEMTK*sizeof(uint64_t*)

  );

  memset(expr_mem,0,MEMSZ*sizeof(uint64_t));

// ---   *   ---   *   ---
// ^continued

  expr=(uint64_t*) malloc(
    EXPLEN*sizeof(uint64_t)

  );

// ---   *   ---   *   ---

  char* input=orc("tests/parse_this.pe");
  rd(input);

  for(int i=0;i<expr_cnt;i++) {

    uint64_t tcnt=*(expr_arr[i]);
    uint64_t* buff=expr_arr[i]+1;

    tryipret(tcnt,buff);

    for(int j=0;j<tcnt;j++) {
//      printf("%016X\n",*buff++);

    };

  };

// ---   *   ---   *   ---
// cleanup

  free(input);
  free(expr);

  free(expr_arr);
  free(expr_mem);

  dlhash(strtab);
  free(mempool);

};

// ---   *   ---   *   ---
