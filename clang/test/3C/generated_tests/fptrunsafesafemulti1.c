// RUN: rm -rf %t*
// RUN: 3c -base-dir=%S -addcr -alltypes -output-dir=%t.checkedALL %s %S/fptrunsafesafemulti2.c --
// RUN: 3c -base-dir=%S -addcr -output-dir=%t.checkedNOALL %s %S/fptrunsafesafemulti2.c --
// RUN: %clang -working-directory=%t.checkedNOALL -c fptrunsafesafemulti1.c fptrunsafesafemulti2.c
// RUN: FileCheck -match-full-lines -check-prefixes="CHECK_NOALL","CHECK" --input-file %t.checkedNOALL/fptrunsafesafemulti1.c %s
// RUN: FileCheck -match-full-lines -check-prefixes="CHECK_ALL","CHECK" --input-file %t.checkedALL/fptrunsafesafemulti1.c %s
// RUN: 3c -base-dir=%S -alltypes -output-dir=%t.checked %S/fptrunsafesafemulti2.c %s --
// RUN: 3c -base-dir=%t.checked -alltypes -output-dir=%t.convert_again %t.checked/fptrunsafesafemulti1.c %t.checked/fptrunsafesafemulti2.c --
// RUN: test ! -f %t.convert_again/fptrunsafesafemulti1.c
// RUN: test ! -f %t.convert_again/fptrunsafesafemulti2.c

/******************************************************************************/

/*This file tests three functions: two callers bar and foo, and a callee sus*/
/*In particular, this file tests: converting the callee into a function pointer
  unsafely via cast and using that pointer for computations*/
/*For robustness, this test is identical to
fptrunsafeprotosafe.c and fptrunsafesafe.c except in that
the callee and callers are split amongst two files to see how
the tool performs conversions*/
/*In this test, foo, bar, and sus will all treat their return values safely*/

/******************************************************************************/

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct general {
  int data;
  struct general *next;
  //CHECK: struct general *next;
};

struct warr {
  int data1[5];
  //CHECK_NOALL: int data1[5];
  //CHECK_ALL: int data1 _Checked[5];
  char *name;
  //CHECK: _Ptr<char> name;
};

struct fptrarr {
  int *values;
  //CHECK: _Ptr<int> values;
  char *name;
  //CHECK: _Ptr<char> name;
  int (*mapper)(int);
  //CHECK: _Ptr<int (int)> mapper;
};

struct fptr {
  int *value;
  //CHECK: _Ptr<int> value;
  int (*func)(int);
  //CHECK: _Ptr<int (int)> func;
};

struct arrfptr {
  int args[5];
  //CHECK_NOALL: int args[5];
  //CHECK_ALL: int args _Checked[5];
  int (*funcs[5])(int);
  //CHECK_NOALL: int (*funcs[5])(int);
  //CHECK_ALL: _Ptr<int (int)> funcs _Checked[5];
};

static int add1(int x) {
  //CHECK: static int add1(int x) _Checked {
  return x + 1;
}

static int sub1(int x) {
  //CHECK: static int sub1(int x) _Checked {
  return x - 1;
}

static int fact(int n) {
  //CHECK: static int fact(int n) _Checked {
  if (n == 0) {
    return 1;
  }
  return n * fact(n - 1);
}

static int fib(int n) {
  //CHECK: static int fib(int n) _Checked {
  if (n == 0) {
    return 0;
  }
  if (n == 1) {
    return 1;
  }
  return fib(n - 1) + fib(n - 2);
}

static int zerohuh(int n) {
  //CHECK: static int zerohuh(int n) _Checked {
  return !n;
}

static int *mul2(int *x) {
  //CHECK: static _Ptr<int> mul2(_Ptr<int> x) _Checked {
  *x *= 2;
  return x;
}

int *sus(struct general *, struct general *);
//CHECK: int *sus(struct general *, struct general *);

int *foo() {
  //CHECK: int *foo(void) : itype(_Ptr<int>) {

  struct general *x = malloc(sizeof(struct general));
  //CHECK: struct general *x = malloc<struct general>(sizeof(struct general));
  struct general *y = malloc(sizeof(struct general));
  //CHECK: struct general *y = malloc<struct general>(sizeof(struct general));
  struct general *curr = y;
  //CHECK: struct general *curr = y;
  int i;
  for (i = 1; i < 5; i++, curr = curr->next) {
    curr->data = i;
    curr->next = malloc(sizeof(struct general));
    curr->next->data = i + 1;
  }
  int (*sus_ptr)(struct fptr *, struct fptr *) = sus;
  //CHECK: int (*sus_ptr)(struct fptr *, struct fptr *) = sus;
  int *z = (int *)sus_ptr(x, y);
  //CHECK: int *z = (int *)sus_ptr(x, y);

  return z;
}

int *bar() {
  //CHECK: int *bar(void) : itype(_Ptr<int>) {

  struct general *x = malloc(sizeof(struct general));
  //CHECK: struct general *x = malloc<struct general>(sizeof(struct general));
  struct general *y = malloc(sizeof(struct general));
  //CHECK: struct general *y = malloc<struct general>(sizeof(struct general));
  struct general *curr = y;
  //CHECK: struct general *curr = y;
  int i;
  for (i = 1; i < 5; i++, curr = curr->next) {
    curr->data = i;
    curr->next = malloc(sizeof(struct general));
    curr->next->data = i + 1;
  }
  int (*sus_ptr)(struct fptr *, struct fptr *) = sus;
  //CHECK: int (*sus_ptr)(struct fptr *, struct fptr *) = sus;
  int *z = (int *)sus_ptr(x, y);
  //CHECK: int *z = (int *)sus_ptr(x, y);

  return z;
}
