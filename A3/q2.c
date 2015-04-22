#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Function prototypes*/
int * readFile();
int strToNumber();
void downheap(int * heap,int i);

int main()
{
    int * heap=readFile();
    int i;
    int j;
    
    /*Performs parental node down heap algorithm to array*/
    for(i=9;i>=0;i--)
    {
        downheap(heap,i);
    }
    
    /*Prints the 2D array heap*/
    for(i=0;i<20;++i)
    {
        for(j=0;j<10;++j)
        {
            if(heap[i*10+j]>=0&&heap[i*10+j]<=9)
            {
                printf("0");
            }
            printf("%d ",heap[i*10+j]);
        }
        printf("\n");
    }
    return 0;
}

/*Downheap on array representation of heap*/
void downheap(int * heap,int i)
{
    int leftChild = 2*i+1;
    int rightChild = 2*i+2;
    int largest = i;
    if(leftChild <=19 && (heap[leftChild*10]+heap[leftChild*10+1]+heap[leftChild*10+2])<(heap[largest*10]+heap[largest*10+1]+heap[largest*10+2])){
        largest = leftChild;
    }
    if(rightChild <=19 && (heap[rightChild*10]+heap[rightChild*10+1]+heap[rightChild*10+2])<(heap[largest*10]+heap[largest*10+1]+heap[largest*10+2])){
        largest = rightChild;
    }
    if(largest != i)
    {
        int j;
        int k;
        for(j=largest*10,k=i*10;j<largest*10+10;++j,++k){
            int temp = heap[j];
            heap[j]=heap[k];
            heap[k]=temp;
        }
        downheap(heap,largest);
    }
}

/*Reads file and returns the array containing the 200 digits*/
int * readFile()
{
    FILE * filePtr;
    char text[50];
    int index;
    int i;
    int j;
    int * input = malloc(sizeof(int)*200);
    filePtr = fopen("f.dat","r");
    index =0;
    
    if(filePtr == NULL)
    {
        printf("error in opening file\n");
        exit(0);
    }
    
    for(i=0;i<20;++i)
    {
        fgets(text,49,filePtr);
        text[strlen(text)-1]='\0';
        input[index++]=strToNumber(strtok(text," "));
        for(j=0;j<9;++j)
        {
            input[index++]=strToNumber(strtok(NULL," "));
        }
        
    }
    return input;
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
