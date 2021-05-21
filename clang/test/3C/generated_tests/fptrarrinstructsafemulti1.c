// RUN: rm -rf %t*
// RUN: 3c -base-dir=%S -addcr -alltypes -output-dir=%t.checkedALL %s %S/fptrarrinstructsafemulti2.c --
// RUN: 3c -base-dir=%S -addcr -output-dir=%t.checkedNOALL %s %S/fptrarrinstructsafemulti2.c --
// RUN: %clang -working-directory=%t.checkedNOALL -c fptrarrinstructsafemulti1.c fptrarrinstructsafemulti2.c
// RUN: FileCheck -match-full-lines -check-prefixes="CHECK_NOALL","CHECK" --input-file %t.checkedNOALL/fptrarrinstructsafemulti1.c %s
// RUN: FileCheck -match-full-lines -check-prefixes="CHECK_ALL","CHECK" --input-file %t.checkedALL/fptrarrinstructsafemulti1.c %s
// RUN: 3c -base-dir=%S -alltypes -output-dir=%t.checked %S/fptrarrinstructsafemulti2.c %s --
// RUN: 3c -base-dir=%t.checked -alltypes -output-dir=%t.convert_again %t.checked/fptrarrinstructsafemulti1.c %t.checked/fptrarrinstructsafemulti2.c --
// RUN: test ! -f %t.convert_again/fptrarrinstructsafemulti1.c
// RUN: test ! -f %t.convert_again/fptrarrinstructsafemulti2.c

/******************************************************************************/

/*This file tests three functions: two callers bar and foo, and a callee sus*/
/*In particular, this file tests: how the tool behaves when there is an array
  of function pointers in a struct*/
/*For robustness, this test is identical to
fptrarrinstructprotosafe.c and fptrarrinstructsafe.c except in that
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
  //CHECK: _Ptr<struct general> next;
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

struct arrfptr *sus(struct arrfptr *, struct arrfptr *);
//CHECK: _Ptr<struct arrfptr> sus(struct arrfptr *x : itype(_Ptr<struct arrfptr>), _Ptr<struct arrfptr> y);

struct arrfptr *foo() {
  //CHECK: _Ptr<struct arrfptr> foo(void) {

  struct arrfptr *x = malloc(sizeof(struct arrfptr));
  //CHECK: _Ptr<struct arrfptr> x = malloc<struct arrfptr>(sizeof(struct arrfptr));
  struct arrfptr *y = malloc(sizeof(struct arrfptr));
  //CHECK: _Ptr<struct arrfptr> y = malloc<struct arrfptr>(sizeof(struct arrfptr));

  struct arrfptr *z = sus(x, y);
  //CHECK: _Ptr<struct arrfptr> z = sus(x, y);
  int i;
  for (i = 0; i < 5; i++) {
    //CHECK_NOALL: for (i = 0; i < 5; i++) {
    //CHECK_ALL: for (i = 0; i < 5; i++) _Checked {
    z->args[i] = z->funcs[i](z->args[i]);
  }

  return z;
}

struct arrfptr *bar() {
  //CHECK: _Ptr<struct arrfptr> bar(void) {

  struct arrfptr *x = malloc(sizeof(struct arrfptr));
  //CHECK: _Ptr<struct arrfptr> x = malloc<struct arrfptr>(sizeof(struct arrfptr));
  struct arrfptr *y = malloc(sizeof(struct arrfptr));
  //CHECK: _Ptr<struct arrfptr> y = malloc<struct arrfptr>(sizeof(struct arrfptr));

  struct arrfptr *z = sus(x, y);
  //CHECK: _Ptr<struct arrfptr> z = sus(x, y);
  int i;
  for (i = 0; i < 5; i++) {
    //CHECK_NOALL: for (i = 0; i < 5; i++) {
    //CHECK_ALL: for (i = 0; i < 5; i++) _Checked {
    z->args[i] = z->funcs[i](z->args[i]);
  }

  return z;
}
