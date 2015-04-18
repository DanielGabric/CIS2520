#include "newton.h"

/*This is a function to take the square root of a number
 it is the recursive version of the newton method*/
double sqrtRecurse(double a, double x, double e)
{
    if(fabs(a*a-x) <= e)
    {
        return a;
    }
    return sqrtRecurse((a+x/a)/2.0,x,e);
}

/*This is a function to take the square root of a number
 it is the iterative version of the newton method*/
double sqrtNonRecurse(double a, double x, double e)
{
    while(fabs(a*a-x)>e)
    {
        a = (a + x/a)/2.0;
    }
    return a;
}
