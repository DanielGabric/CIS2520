#include "carbon.h"

/*Generate all permutation of c and print them*/
/*Uses backtracking*/
/*Swap two characters, call the function with the new string*/
/*Then swap them back, hence the backtrack*/
void perm(char * c, int start)
{
    if(start == MAX_SIZE)
    {
        printf("%s\n",c);
    }else{
        int i = 0;
        for(i=start;i<6;++i)
        {
            swap(&c[i],&c[start]);
            perm(c,start+1);
            swap(&c[i],&c[start]);
        }
    }
}

/*Swap values of a character array when needed*/
void swap(char * c, char* a)
{
    char temp = *c;
    *c = *a;
    *a = temp;
}
