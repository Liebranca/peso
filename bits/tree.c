// ---   *   ---   *   ---
// TREE
// Natural hierarchies
//
// LIBRE SOFTWARE
// Licensed under GNU GPL3
// be a bro and inherit
//
// CONTRIBUTORS
// lyeb,

// ---   *   ---   *   ---
// guard

#ifndef __TREE_C__
#define __TREE_C__

// ---   *   ---   *   ---
// deps

  #include <stdio.h>
  #include <stdint.h>

// ---   *   ---   *   ---
// shorthands

  #define NTL(...) {__VA_ARGS__,NULL}
  #define sptr(x) ((uint16_t) ((uintptr_t) (x)))

// ---   *   ---   *   ---

typedef struct tree_t {

  char* value;

  struct tree_t* par;
  struct tree_t* chld;

  struct tree_t* prev;
  struct tree_t* next;

} Tree;

// ---   *   ---   *   ---
// fwd decls :<

size_t tree_push(Tree* t,Tree** e);

// ---   *   ---   *   ---

Tree* tree_nit(Tree* par,char* value) {

  Tree* t=malloc(sizeof(Tree));

  if(t!=NULL) {
    memset(t,0,sizeof(Tree));
    t->value=value;

    if(par) {
      Tree* ar[]=NTL(t);
      tree_push(par,ar);

    };

  };

  return t;

};

// ---   *   ---   *   ---

void tree_del(Tree* t) {

  if(t!=NULL) {

    free(t);

    tree_del(t->chld);
    tree_del(t->next);

  };

};

// ---   *   ---   *   ---

size_t unshift_tr(Tree** ar,Tree** e,size_t len) {

  size_t stride=0;

  while(*e+stride!=NULL) {stride++;};
  while(len) {ar[len+stride]=ar[--len];};

  for(int i=0;i<stride;i++) {
    ar[i]=*e++;

  };

  return stride;

};

size_t shift_tr(Tree** ar,Tree** e,size_t len) {

  *e=*ar;

  int i=0;
  while(i<len) {ar[i]=ar[++i];};

  ar[--len]=NULL;
  return len;

};

// ---   *   ---   *   ---

size_t tree_push(Tree* t,Tree** e) {

  size_t len=0;

  Tree* cur=t->chld;
  Tree* prev=NULL;

  while(cur != NULL && cur->next!=NULL) {

    prev=cur;
    cur=cur->next;

    len++;

  };

  if(!t->chld) {
    t->chld=*e;

  };

  while(*e!=NULL) {

    cur=*e++;
    cur->prev=prev;
    cur->next=*e;

    cur->par=t;

    prev=cur;
    len++;

  };

  return len;

};

// ---   *   ---   *   ---

void tree_prich(Tree* t) {

  Tree* pending=t;

  while(pending!=NULL) {

    printf(">%s\n",pending->value);
    pending=pending->chld;

  };

  tree_del(pending);

};

// ---   *   ---   *   ---

#endif // __TREE_C__
