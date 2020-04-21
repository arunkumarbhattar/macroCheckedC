//
// These is a regression test case for
// https://github.com/Microsoft/checkedc-clang/issues/487
//
// The TreeTransform code may alter an implicit cast expression returned by a
// Transform method.  It assumes that they have been regenerated by semantic
// checking.  We substitute argument expressions into bounds expressions at
// calls in SemaBounds using a TreeTransform.  When the argument
// expression was an implicit cast, it was altered by the TreeTransform.  The 
// argument expression is used elsewhere, so that is incorrect.  This 
// triggered an assert in code generation.
//
// RUN: %clang -Wno-check-bounds-decls-checked-scope -c -o %t %s

#pragma CHECKED_SCOPE ON

 // Cut-down case.
extern unsigned short f(const unsigned char *buf : count(len),
		                    unsigned short len);
                        
unsigned short g(void) {
   return f(0 , 0U);
}

// From user report, modified to not an include file.
extern unsigned short h(unsigned short s, 
                        const unsigned char *buf : count(len),
		                    unsigned short len, unsigned int accum_len);

typedef struct mystruct {
	void *data : byte_count(size);
	unsigned int size;
} mystruct;

static inline unsigned short
myfunc(_Ptr<mystruct> payload)
{
    return h(0, payload->data, payload->size, 23);
}

int
main(void)
{
    myfunc(0);
}
