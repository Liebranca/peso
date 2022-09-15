
// ---   *   ---   *   ---

char* str_isert(

  char*  isert,

  char** cuts,
  size_t sz

) {

  char* out=(char*) (mempool+mempool_ptr);

  for(int i=0;i<sz;i++) {

    strcat(out,cuts[i]);
    if(i<sz-1) {strcat(out,isert);};

  };

  mempool_ptr+=strlen(out)+1;
  return out;

};

// ---   *   ---   *   ---

char* get_keyw_or_val(uint64_t x) {

  char* out=NULL;

  if(x<strtab->bsz) {
    out=strtab->elems[x].key;

  } else {
    out=strtab->elems[x-strtab->bsz].key;

  };

  return out;

};

// ---   *   ---   *   ---

void unquote(char* src) {

  int len=strlen(src);

  if(

     (src[0]==0x22 || src[0]==0x27)
  && (src[len-1]==0x22 || src[len-1]==0x27)

  ) {

    int i=0;

    do {

      *src=*(src+1);
      i++;src++;

    } while(i<(len-2));

    *src=0x00;

  };

};

// ---   *   ---   *   ---
