// ---   *   ---   *   ---
// HASH
// Soul of ZJC
//
// LIBRE SOFTWARE
// Licensed under GNU GPL3
// be a bro and inherit
//
// CONTRIBUTORS
// lyeb,

// ---   *   ---   *   ---
// guards

#ifndef __HASH_C__
#define __HASH_C__

// ---   *   ---   *   ---
// deps

  #include <stdio.h>
  #include <stdlib.h>
  #include <stdint.h>

  #include <string.h>
  #include <errno.h>

// ---   *   ---   *   ---
// GBL

  static uint64_t strtab_top=1;
  static uint64_t keyw_top=0;

// ---   *   ---   *   ---

typedef struct {

  char* key;
  void* value;

} Hashed;

typedef struct {

  size_t    sz;
  size_t    bsz;

  uint64_t  top;
  uint64_t  keyw_cnt;

  char*     keys;
  size_t    keys_ptr;

  Hashed    elems[];

} Strtab;

// ---   *   ---   *   ---

uint64_t hashfn_str(size_t sz,char* k) {

  uint64_t idex = 0;

  uint64_t x    = 0;
  uint64_t y    = 0;

  do {

    x=((uint64_t) *k)+1;
    x*=x;idex^=x+y;y+=1;

  } while(*k++);

  return (idex%sz)*16;

};

// ---   *   ---   *   ---

Strtab* nthash(int mag) {

  size_t sz=1;
  do {sz*=2;} while(mag--);

  size_t memsz=

    sizeof(Strtab)
  + (sz*(sizeof(Hashed)*16))

  ;

  Strtab* table=(Strtab*) malloc(memsz);
  memset(table,0,memsz);

  table->sz=sz;
  table->bsz=sz*16;

  table->keys=(char*) malloc(
    0x1000*sizeof(char)

  );

  table->keys_ptr=0;

  return table;

};

void dlhash(Strtab* table) {

  free(table->keys);
  free(table);

};

// ---   *   ---   *   ---

uint64_t hash_set(

  Strtab* table,

  char*   key,
  void*   value

) {

  uint64_t idex=hashfn_str(table->sz,key);
  Hashed* h=table->elems+idex;

  int i=0;

  while(h->key!=NULL) {

    if(!strcmp(h->key,key)) {break;};

    idex++;
    h=table->elems+idex;

    i++;

    // TODO: enlarge and reloc
    if(i==16) {
      fprintf(stderr,"Fatal collision!\n");
      exit(1);

    };

  };

  // save copy of key
  char* local_key=table->keys+table->keys_ptr;
  strcpy(local_key,key);

  table->keys_ptr+=strlen(key)+1;

  h->key=local_key;
  h->value=value;

  table->top++;

  return idex;

};

// ---   *   ---   *   ---

void* hash_get(
  Strtab* table,
  char*   key

) {

  void* out=NULL;

  uint64_t idex=hashfn_str(table->sz,key);
  Hashed* h=table->elems+idex;

  int match=0;
  int i=0;

  while(i<16) {

    if(

       h->key!=NULL
    && !strcmp(h->key,key)

    ) {match=1;break;};

    idex++;
    h=table->elems+idex;

    i++;

  };

  if(match) {out=h->value;};
  return out;

};

uint64_t hash_get_idex(
  Strtab* table,
  char*   key

) {

  uint64_t out=0;

  uint64_t idex=hashfn_str(table->sz,key);
  Hashed* h=table->elems+idex;

  int match=0;
  int i=0;

  while(i<16) {

    if(

       h->key!=NULL
    && !strcmp(h->key,key)

    ) {match=1;break;};

    idex++;
    h=table->elems+idex;

    i++;

  };

  if(match) {out=idex;};
  return out;

};

// ---   *   ---   *   ---

# endif // __HASH_C__
