// RUN: cconv-standalone -base-dir=%S -alltypes -output-postfix=checked2 %s %S/fptrunsafecalleemulti1.c
//RUN: FileCheck -match-full-lines --input-file %S/fptrunsafecalleemulti2.checked2.c %s
//RUN: rm %S/fptrunsafecalleemulti1.checked2.c %S/fptrunsafecalleemulti2.checked2.c
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

int * sus(struct general *x, struct general *y) {

        x = (struct general *) 5;
        int *z = calloc(5, sizeof(int)); 
        struct general *p = y;
        for(int i = 0; i < 5; p = p->next, i++) { 
            z[i] = p->data; 
        } 
        
z += 2;
return z; }
//CHECK: int * sus(struct general *x, struct general *y) {
