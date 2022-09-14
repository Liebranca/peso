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

typedef struct {

  char* key;
  void* value;

} Hashed;

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

void* nthash(int mag) {

  size_t sz=1;
  do {sz*=2;} while(mag--);

  void* table=malloc(

    sizeof(size_t)
  + (sz*(sizeof(Hashed)*16))

  );

  *((size_t*) table)=sz;

  return table;

};

// ---   *   ---   *   ---

void hash_set(
  void* table,

  char* key,
  void* value

) {

  size_t* sz=(size_t*) table;
  Hashed* elems=(Hashed*) (sz+1);

  uint64_t idex=hashfn_str(*sz,key);
  Hashed* h=elems+idex;

  int i=0;

  while(h->key!=NULL) {

    if(!strcmp(h->key,key)) {break;};

    idex++;
    h=elems+idex;

    i++;

    // TODO: enlarge and reloc
    if(i==16) {
      fprintf(stderr,"Fatal collision!\n");
      exit(1);

    };

  };

  h->key=key;
  h->value=value;

};

// ---   *   ---   *   ---

void* hash_get(
  void* table,
  char* key

) {

  void* out=NULL;

  size_t* sz=(size_t*) table;
  Hashed* elems=(Hashed*) (sz+1);

  uint64_t idex=hashfn_str(*sz,key);
  Hashed* h=elems+idex;

  int match=0;
  int i=0;

  while(i<16) {

    if(

       h->key!=NULL
    && !strcmp(h->key,key)

    ) {match=1;break;};

    idex++;
    h=elems+idex;

    i++;

  };

  if(match) {out=h->value;};
  return out;

};

// ---   *   ---   *   ---

# endif // __HASH_C__
