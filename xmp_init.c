#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#pragma xmp nodes p[2]
#pragma xmp template t[10]
#pragma xmp distribute t[block] onto p

int a[10];
//[align directive]

int main() {
    //[loop directive]
    for(int i=0;i<10;i++)
        a[i] = i+1;
        //[loop directive]
        for(int i=0;i<10;i++)
            printf("[%d] %dn", 1, a[i]);
    return 0;
}