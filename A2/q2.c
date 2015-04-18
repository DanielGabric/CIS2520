#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*Array based stack*/
typedef struct
{
    double *postFix;
    int top;
} stack;
/*Parameters: value to push, stack, and max value of array*/
int push(double val, stack * s, int max);
/*Parameters: stack*/
double pop(stack * s);
/*Parameters: command line postfix expression, max value of array*/
double getPostFix(char expression[],int max);

int main(int argc, char * argv[])
{
    int max;
    double result;
    max = strlen(argv[1]);

    result = getPostFix(argv[1],max);
    printf("The result is %.2lf\n",result);
    
    return 0;
}


/*Takes in the postfix notation expression, then uses a stack
 to evaluate the final result and returns it*/
double getPostFix(char expression[], int max)
{
    /*initial stack vals*/
    stack *s = malloc(sizeof(stack));
    s->postFix = malloc(sizeof(double)*max);
    s->top = 0;
    double result;
    int i;
    
    /*Loops through the expression and adds number values to the stack
     if it reaches an operator it pops two values off the stack and 
     evaluates them, then pushes the new value onto the stack
     this is done until the end*/
    for(i=0;i<max;++i)
    {
        if(expression[i]>='0'&&expression[i]<='9')
        {
            if(!push(expression[i]-'0',s,max))
            {
                printf("FATAL ERROR! STACK FULL!\n");
                return 0;
            }
        }
        else
        {
            double num1;
            double num2;
            if(s->top > 1)
            {
                num2 = pop(s);
                num1 = pop(s);
                if(expression[i]=='*')
                {
                    if(!push(num2*num1,s,max))
                    {
                        printf("FATAL ERROR! STACK FULL!\n");
                        exit(0);
                    }
                }
                else if(expression[i]=='/')
                {
                    if(!push(num1/num2,s,max))
                    {
                        printf("FATAL ERROR! STACK FULL!\n");
                        exit(0);
                    }
                }
                else if(expression[i]=='+')
                {
                    if(!push(num2+num1,s,max))
                    {
                        printf("FATAL ERROR! STACK FULL!\n");
                        exit(0);
                    }
                }
                else if(expression[i]=='-')
                {
                    if(!push(num1-num2,s,max))
                    {
                        printf("FATAL ERROR! STACK FULL!\n");
                        exit(0);
                    }
                }
            }
            else
            {
                printf("ERROR! STACK IS EMPTY!");
                exit(0);
                
            }
        }
    }
    
    result = pop(s);
    /*Freeing the stack pointers*/
    free(s->postFix);
    free(s);
    
    return result;
}

/*Pushing values onto an array based stack*/
int push(double val, stack * s, int max)
{
    if(s->top == max)
    {
        return 0;
    }
    s->postFix[s->top++]=val;
    return 1;
}
/*Popping a value off of an array based stack*/
double pop(stack * s)
{
    if(s->top == 0)
    {
        return -1;
    }
    return s->postFix[--s->top];
}



