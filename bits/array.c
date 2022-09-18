// ---   *   ---   *   ---
// ARRAY
// Because high level
//
// LIBRE SOFTWARE
// Licensed under GNU GPL3
// be a bro and inherit
//
// CONTRIBUTORS
// lyeb,

// ---   *   ---   *   ---
// guards

#ifndef __ARRAY_C__
#define __ARRAY_C__

// ---   *   ---   *   ---
// deps

  #include <stdlib.h>
  #include <errno.h>

// ---   *   ---   *   ---
// const

  #define DEF_ARR_SZ 0x40

// ---   *   ---   *   ---

typedef struct {

  size_t    size;
  size_t    used;

  uint64_t* buff;

} Array;

// ---   *   ---   *   ---

void array_nit(Array* ar) {

  size_t x=DEF_ARR_SZ*sizeof(uint64_t);

  ar->buff=malloc(x);

  ar->size=DEF_ARR_SZ;
  ar->used=0;

  memset(ar->buff,0,x);

};

void array_del(Array* ar) {free(ar);};

// ---   *   ---   *   ---

void array_push(Array* ar,uint64_t e) {

  if(ar->used==ar->size) {

    ar->size*=2;

    uint64_t* re=realloc(
      ar->buff,
      ar->size * sizeof(uint64_t)

    );

    if(re==NULL) {
      free(ar->buff);
      fprintf(stderr,"%s\n",strerror(errno));

      exit(1);

    };

    ar->buff=re;

  };

  ar->buff[ar->used++]=e;

};

// ---   *   ---   *   ---

#endif // __ARRAY_C__
