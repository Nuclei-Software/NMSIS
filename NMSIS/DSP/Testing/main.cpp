#include <cstdio>
#include <cstdlib>
#include "riscv_math_types.h"
extern int testmain(const char *);

extern "C" const char *patternData;


int main()
{
    int r;

    r=testmain(patternData);

    printf("Done\r\n");
    return r;
}
