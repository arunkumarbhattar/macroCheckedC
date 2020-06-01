// RUN: cconv-standalone -base-dir=%S -alltypes -output-postfix=checked %s %S/fptrinstructcallermulti2.c
//RUN: FileCheck -match-full-lines --input-file %S/fptrinstructcallermulti1.checked.c %s
//RUN: rm %S/fptrinstructcallermulti1.checked.c %S/fptrinstructcallermulti2.checked.c
#define size_t int
#define NULL 0
extern _Itype_for_any(T) void *calloc(size_t nmemb, size_t size) : itype(_Array_ptr<T>) byte_count(nmemb * size);
extern _Itype_for_any(T) void free(void *pointer : itype(_Array_ptr<T>) byte_count(0));
extern _Itype_for_any(T) void *malloc(size_t size) : itype(_Array_ptr<T>) byte_count(size);
extern _Itype_for_any(T) void *realloc(void *pointer : itype(_Array_ptr<T>) byte_count(1), size_t size) : itype(_Array_ptr<T>) byte_count(size);
extern int printf(const char * restrict format : itype(restrict _Nt_array_ptr<const char>), ...);
extern _Unchecked char *strcpy(char * restrict dest, const char * restrict src : itype(restrict _Nt_array_ptr<const char>));

struct general { 
    int data; 
    struct general *next;
};

struct warr { 
    int data1[5];
    char name[];
};

struct fptrarr { 
    int *values; 
    char *name;
    int (*mapper)(int);
};

struct fptr { 
    int *value; 
    int (*func)(int*);
};  

struct arrfptr { 
    int args[5]; 
    int (*funcs[5]) (int);
};

int add1(int x) { 
    return x+1;
} 

int sub1(int x) { 
    return x-1; 
} 

int fact(int n) { 
    if(n==0) { 
        return 1;
    } 
    return n*fact(n-1);
} 

int fib(int n) { 
    if(n==0) { return 0; } 
    if(n==1) { return 1; } 
    return fib(n-1) + fib(n-2);
} 

int zerohuh(int n) { 
    return !n;
}

int *mul2(int *x) { 
    *x *= 2; 
    return x;
}
//CHECK:     _Ptr<struct general> next;

//CHECK:     int data1 _Checked[5];
//CHECK:     _Ptr<char> name;

//CHECK:     _Ptr<int> values; 

//CHECK:     _Ptr<int (int )> mapper;

//CHECK:     _Ptr<int (_Ptr<int> )> func;
//CHECK:     int args _Checked[5]; 
//CHECK:     _Ptr<int (int )> funcs _Checked[5];

struct fptr * sus(struct fptr *, struct fptr *);
//CHECK: struct fptr * sus(struct fptr *, struct fptr *);

struct fptr * foo() {
 
        struct fptr * x = malloc(sizeof(struct fptr)); 
        struct fptr *y =  malloc(sizeof(struct fptr));
        struct fptr *z = sus(x, y);
        
return z; }
//CHECK: struct fptr * foo() {

struct fptr * bar() {
 
        struct fptr * x = malloc(sizeof(struct fptr)); 
        struct fptr *y =  malloc(sizeof(struct fptr));
        struct fptr *z = sus(x, y);
        
z += 2;
return z; }
//CHECK: struct fptr * bar() {
