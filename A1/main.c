#include "main.h"

int main()
{
    /*Takes in menu input*/
    char * capture;

    capture = malloc(sizeof(char)*50);
    BOOLEAN didEnter;
    didEnter = FALSE;

    /*Command loop*/
    while(capture[0] != 'e')
    {
        capture[0]='\0';
        didEnter = FALSE;
        
        printf("Enter 'n' for newton, 'a' for ackermann, 'c' for carbon, and 'e' for exit: \n");
        
        while(!isGoodChar(capture))
        {
            if(didEnter)
            {
                printf("ERROR WRONG INPUT! PLEASE only enter either, 'n', 'a', 'c', or 'e':\n");
            }
            fgets(capture,49,stdin);
            capture[strlen(capture)-1]='\0';
            didEnter = TRUE;
        }
        callFunctions(capture[0]);

    }

    
    free(capture);
    return 0;
}

/*Checks if the menu input is valid*/
BOOLEAN isGoodChar(char * input)
{
    return strlen(input)==1 && (input[0]=='a'||input[0]=='c'||input[0]=='n'||input[0]=='e');
}

/*Checks if the string is all numbers*/
BOOLEAN isInt(char * input)
{
    int i;
    
    for(i=0;i<strlen(input);++i)
    {
        if((input[i]<'0'||input[i]>'9'))
        {
            return FALSE;
        }
    }
    return TRUE;
}

/*Checks if the string is a floating number*/
BOOLEAN isDouble(char * input)
{
    int i;
    int numOfPoints;
    
    numOfPoints = 0;
    
    for(i=0;i<strlen(input);++i)
    {
        if(input[i]=='.')
        {
            
            numOfPoints++;
        }
        
        if(input[i]!='.'&&(input[i]>'9'||input[i]<'0'||numOfPoints > 1))
        {
            return FALSE;
        }
    }
    return TRUE;
}

/*Converts a string to an integer*/
int strToNumber(char * input)
{
    int i;
    int ratio;
    int num;
    
    num =0;
    ratio = 1;
    
    for(i=strlen(input)-1;i>=0;--i)
    {
        num += (input[i]-'0')*ratio;
        ratio *= 10;
    }
    return num;
}

/*Will take in all information for the ackermann function, and will return suitable integer
 m and n values*/
void getInputAcker(char * input, char mOrN,int * m, int * n)
{
    BOOLEAN hasBeenSet;
    
    hasBeenSet = FALSE;
    input[0]='r';
    
    printf("Enter your POSITIVE INTEGER value for %c in the ackermann function: ",mOrN);
    
    /*Defensive coding*/
    while(!isInt(input))
    {
        if(hasBeenSet)
        {
            printf("Error! Please enter a valid POSITIVE INTEGER for %c: ",mOrN);

        }
      
        fgets(input,50,stdin);
        input[strlen(input)-1]='\0';
        hasBeenSet = TRUE;
    }
    
    if(mOrN == 'm')
    {
        *m = strToNumber(input);
        
    }
    else
    {
        *n = strToNumber(input);
        
    }
}

/*Will take in all information for the newton square root functions, and will return suitable double
 n and e values*/
void getInputNewton(char * input, char sOrE, double * n, double * e)
{
    BOOLEAN hasBeenSet;
    
    input[0]='r';
    hasBeenSet = FALSE;
    
    /*Differentiates on whether entering square root or epsilon*/
    if(sOrE == 's')
    {
        printf("Enter a positive number you want to take the square root of: ");
    }
    else
    {
        printf("Enter a positive epsilon you want your square root to be between: ");
    }
    
    /*Defensive coding*/
    while(!isDouble(input))
    {
        if(hasBeenSet)
        {
            printf("Error, invalid input, re-enter a positive number: ");
        }
        
        fgets(input,49,stdin);
        input[strlen(input)-1]='\0';
        
        hasBeenSet = TRUE;
    }
    
    
    if(sOrE == 's')
    {
        *n = atof(input);
    }
    else
    {
        *e = atof(input);
    }
}

/*Function to call other functions and decide on what to do according
 to what has been input in the main function*/
void callFunctions(char input)
{

    /*Max values for ackermann function*/
    int max[]={2147483645,262108,10000,13,1};
    char * input1;
    /*String that needs permuting for carbon*/
    char  c[] ={'c','a','r','b','o','n'};

    input1 = malloc(sizeof(char)*50);
    
    if(input == 'c')
    {
        perm(c,0);
    }
    else if (input == 'a')
    {
        int m;
        int n;
        n = 0;
        m = 0;

        getInputAcker(input1, 'm',&m,&n);
        getInputAcker(input1,'n',&m,&n);
        
        if(m>4 || n > max[m])
        {
  
            printf("Error aborting program, high m and/or n will cause your computer to explode!!!\n");
            exit(0);
        }
        
        printf("A(%d, %d) = %d\n",m,n,A(m,n));
         
    }
    else if(input == 'n')
    {
        double initial;
        double e;
        struct timeval t;
        double t1;
        double t2;
        double sqrt;
        double iterTime;
        double recurTime;
        
        e = 0;
        initial = 0;

        getInputNewton(input1, 's',&initial, &e);
        getInputNewton(input1, 'e',&initial, &e);
        
        /*Recursive and iterative call*/
        /*Getting time elapsed after each call*/
        gettimeofday(&t,NULL);
        t1 = t.tv_sec*1000.0+(t.tv_usec/1000.0);
        sqrt = sqrtRecurse(initial/2,initial, e);
        gettimeofday(&t,NULL);
        t2 = t.tv_sec*1000.0+(t.tv_usec/1000.0);
        recurTime = t2-t1;
        
        printf("The recursive square root function gave a value of %f for the square root.\nIt ran in %f milliseconds.\n\n",sqrt,recurTime);
        
        gettimeofday(&t,NULL);
        t1 = t.tv_sec*1000.0+(t.tv_usec/1000.0);
        sqrt = sqrtNonRecurse(initial/2.0,initial, e);
        gettimeofday(&t,NULL);
        t2 = t.tv_sec*1000.0+(t.tv_usec/1000.0);
        iterTime = t2-t1;
        
        /*Printing out all info on sqrt of iterative and recursive functions*/
        printf("The iterative square root function gave a value of %f for the square root.\nIt ran in %f milliseconds.\n\n",sqrt,iterTime);
        
        if(iterTime > recurTime)
        {
            printf("The recursive function was faster than the iterative function by %f milliseconds.\n",iterTime-recurTime);
        }
        else if(recurTime > iterTime)
        {
            printf("The iterative function was faster than the recursive function by %f milliseconds.\n",recurTime-iterTime);
        }
        else
        {
            printf("The iterative function and the recursive function executed at the same speed.\n");
        }
        
    }
    
    /*Free pointer*/
    free(input1);
}
