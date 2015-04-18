#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*Pointer based linked list*/
struct node{
    int mileage;
    char * plateNum;
    struct node * next;
    char * returnDate;
};

typedef struct node list;

/*Parameters: pointer to a pointer to the root, and the toAdd node*/
void insertRent(list ** root,list * toAdd);
void insertAvail(list ** root, list * toAdd);
int search(list ** root, list * node);
void addToFront(list **root,list * toAdd);
/*Parameters: list root, and plate string*/
list * searchRepair(list ** root, char * plate);
int searchIfInList(list * root, char * plateNum);
/*Parameters: pointer to a pointer to the root*/
list * popFront(list ** root);
/*Parameters: attributes of a node*/
list * newNode(int mileage, char * plateNum);
/*Parameters: single node*/
void freeNode(list * toFree);
int getInput();
/*Parameter: input string*/
int isGood(char * in);
/*Parameters: all three lists*/
list * addInput(list * available, list * repair, list * rented);
/*Parameters: mileage string*/
int isNum(char * num);
/*Parameters: string*/
int strToNumber(char * input);
/*Parameters: root of list*/
int length(list * root);
/*Parameters: all three lists*/
void printLists(list * avail,list * repair, list * rented);
/*Parameters: pointers to pointers to all three lists*/
void deleteAllLists(list ** av, list ** rented, list ** repair);
/*Parameters:filePath and all three lists*/
void getFromTextFile(char * filePath,list ** av, list ** rented, list ** repair);
void writeToFile(char * filePath,list * av,list * rented, list * repair);
/*Parameters: date string*/
int stringToDate(char * date);
/*Paramters: plate string*/
int isGoodPlate(char * plate);

int main()
{
    int choice;
    float totalIncome;
    list *avForRent;
    list *repair;
    list *rented;
    
    totalIncome = 0;
    avForRent = NULL;
    repair = NULL;
    rented = NULL;
    /*load values into the three lists*/
    getFromTextFile("./DISK_FILE.txt",&avForRent,&rented,&repair);
    
    /*command loop*/
    do
    {
        list * new;
        choice = getInput();
        float cost = 0;
        int newMileage=0;
        char * returnIn;
        
        /*Switch based on which option you entered*/
        switch(choice)
        {
            /*add a new car to avForRent list*/
            case 1:
                new = addInput(avForRent,repair,rented);
                if(searchIfInList(avForRent,new->plateNum))
                {
                    printf("Fatal error! We already have that plate number on record!\n\n");
                    freeNode(new);
                }
                else
                {
                    printf("Successfully added a new car to the available-for-rent list.\n\n");
                    insertAvail(&avForRent,new);
                }
                break;
            /*Transfer a car from rented list to avForRent list*/
            case 2:

                if(length(rented)==0)
                {
					printf("Fatal error! No cars to be transferred!\n\n");
				}
				else
				{
					new = addInput(avForRent,repair,rented);
                    newMileage = search(&rented,new);
                    if(newMileage<=0)
                    {
                        printf("Fatal error! Either you entered a wrong plate number or your new mileage is less than or equal to the old.\n\n");
                        freeNode(new);
                    }
                    else
                    {
                        if(newMileage <=100)
                        {
                            cost = 40.0;
                        }
                        else
                        {
                            cost = 40.0 + (newMileage-100)*0.15;
                        }
                        printf("Successfully transferred a car from the rented list to the available-for-rent list.\n");
                        printf("The cost of this transaction is $%.2f.\n\n",cost);
                        totalIncome += cost;
                        insertAvail(&avForRent,new);
                    }
                }
                break;
            /*Transfer a car from rented list to repair list*/
            case 3:
            
                if(length(rented)==0)
                {
					printf("Fatal error! No cars to be transferred!\n\n");
				}
				else
				{
                    new = addInput(avForRent,repair,rented);
                    newMileage = search(&rented,new);
                    if(newMileage<=0)
                    {
                        printf("Fatal error! Either you entered a wrong plate number or your new mileage is less than or equal to the old.\n\n");
                        freeNode(new);
                    }
                    else
                    {
                        if(newMileage <=100)
                        {
                            cost = 40.0;
                        }
                        else
                        {
                            cost = 40.0 + (newMileage-100)*0.15;
                        }
                    
                        printf("Successfully transferred a car from the rented list to the repair list.\n");
                        printf("The cost of this transaction is $%.2f.\n\n",cost);
                        totalIncome += cost;

                        addToFront(&repair,new);
                    
                    }
                }
                break;
            /*Transfer a car from repair list to avForRent list*/
            case 4:
                if(length(repair)==0)
                {
					printf("Fatal error! No cars in repair\n\n");
				}
				else
				{
                    returnIn = malloc(sizeof(char)*100);
                    printf("Please enter a valid license plate: ");
                    fgets(returnIn,99,stdin);
                    returnIn[strlen(returnIn)-1]='\0';
                    new = searchRepair(&repair, returnIn);
                    if(new == NULL)
                    {
                        printf("Fatal error! Your license plate is not in our records!\n\n");
                    }
                    else
                    {
                        printf("Eureka! We found the car, it has been transferred from repairs to available-for-rent!\n\n");
                        insertAvail(&avForRent,new);
                    }
			    }
                break;
            /*Transfer a car from avForRent list to rented list*/
            case 5:
                if(length(avForRent)==0)
                {
					printf("Fatal error! No cars available for rent!\n\n");
				}
				else
				{
                    returnIn = malloc(sizeof(char)*100);
                    do
                    {
                        printf("Please enter a valid return date of the type YYMMDD: ");
                        fgets(returnIn,99,stdin);
                        returnIn[strlen(returnIn)-1]='\0';
                    
                    }while(!isNum(returnIn)||strlen(returnIn)!=6||!stringToDate(returnIn));
                
                    new = popFront(&avForRent);
                    new->returnDate = returnIn;
                    printf("First car in available-for-rent has been rented.\n\n");
                    insertRent(&rented,new);
			    }
                break;
            /*Print all lists*/
            case 6:
                printLists(avForRent,repair,rented);
                break;
            /*if 7 print totalIncome, write lists to file, and delete lists*/
            case 7:
                printf("Total income for this session: $%.2f\n",totalIncome);
                writeToFile("./DISK_FILE.txt",avForRent,rented,repair);
                deleteAllLists(&avForRent,&rented,&repair);
                break;
        }
    }
    while(choice != 7);
    
    
    return 0;
}

/*See's if the date is valid*/
int stringToDate(char * date)
{
	int days[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};
    int n = strToNumber(date);
    int day = n %100;
    n/=100;
    int month = n%100;
    if(month>12)
    {
	    return 0;
	}
	if(day>days[month])
	{
	    return 0;
	}
	return 1;
}

/*Writes all the contents of three lists into a text file*/
void writeToFile(char * filePath, list * av, list * rented, list * repair)
{
    FILE * filePtr;
    int avLength;
    int rentedLength;
    int repairLength;
    int i;
    list * temp;
    
    filePtr = fopen(filePath,"w");
    avLength = length(av);
    rentedLength = length(rented);
    repairLength = length(repair);
    
    if(filePtr == NULL)
    {
        printf("error in opening file\n");
        exit(0);
    }
    
    fprintf(filePtr,"%d\n",avLength);
    temp = av;
    
    for(i=0;i<avLength;++i)
    {
        fprintf(filePtr,"%d\n",temp->mileage);
        fprintf(filePtr,"%s\n",temp->plateNum);
        temp = temp->next;
    }
    
    fprintf(filePtr,"%d\n",repairLength);
    temp = repair;
    
    for(i=0;i<repairLength;++i)
    {
        fprintf(filePtr,"%d\n",temp->mileage);
        fprintf(filePtr,"%s\n",temp->plateNum);
        temp = temp->next;
    }
    
    fprintf(filePtr,"%d\n",rentedLength);
    temp = rented;
    
    for(i=0;i<rentedLength;++i)
    {
        fprintf(filePtr,"%d\n",temp->mileage);
        fprintf(filePtr,"%s\n",temp->plateNum);
        fprintf(filePtr,"%s\n",temp->returnDate);
        temp = temp->next;
    }
    
    fclose(filePtr);
}

/*Gets text from a textFile, parses it, and loads all three lists with it*/
void getFromTextFile(char * filePath,list ** av, list ** rented, list ** repair)
{
    FILE * filePtr;
    char firstInputs[20];
    int index;
    char * plate;
    char * mileage;
    char * returnDate;
    int i;
    list * temp;
    

    returnDate = malloc(sizeof(char)*100);
    filePtr = fopen(filePath,"r");

    if(filePtr==NULL)
    {
        printf("error in opening file\n");
        exit(0);
    }
    
    fgets(firstInputs,19,filePtr);
    firstInputs[strlen(firstInputs)-1]='\0';

    i = strToNumber(firstInputs);
    for(index = 0;index<i;++index)
    {
        plate = malloc(sizeof(char)*100);
        mileage = malloc(sizeof(char)*100);
        fgets(mileage,99,filePtr);
        fgets(plate,99,filePtr);
        mileage[strlen(mileage)-1]='\0';
        plate[strlen(plate)-1]='\0';
        temp = newNode(strToNumber(mileage),plate);
        insertAvail(av,temp);
    }
    
    fgets(firstInputs,19,filePtr);
    firstInputs[strlen(firstInputs)-1]='\0';
    i=strToNumber(firstInputs);

    for(index=0;index<i;++index)
    {
        plate = malloc(sizeof(char)*100);
        mileage = malloc(sizeof(char)*100);
        fgets(mileage,99,filePtr);
        fgets(plate,99,filePtr);
        mileage[strlen(mileage)-1]='\0';
        plate[strlen(plate)-1]='\0';
        temp = newNode(strToNumber(mileage),plate);
        addToFront(repair,temp);
    }
    
    fgets(firstInputs,19,filePtr);
    firstInputs[strlen(firstInputs)-1]='\0';
    i=strToNumber(firstInputs);
    for(index=0;index<i;++index)
    {
        plate = malloc(sizeof(char)*100);
        mileage = malloc(sizeof(char)*100);
        returnDate = malloc(sizeof(char)*100);
        fgets(mileage,99,filePtr);
        fgets(plate,99,filePtr);
        fgets(returnDate,99,filePtr);
        mileage[strlen(mileage)-1]='\0';
        plate[strlen(plate)-1]='\0';
        returnDate[strlen(returnDate)-1]='\0';
        temp = newNode(strToNumber(mileage),plate);
        temp->returnDate = returnDate;
        insertRent(rented,temp);
    }
    fclose(filePtr);
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

/*Checks if a given string is a number*/
int isNum(char * num)
{
    int i;
    for(i=0;i<strlen(num);++i)
    {
        if(num[i]<'0'||num[i]>'9')
        {
            return 0;
        }
    }
    return 1;
}

/*Outputs the options and gets the input of which one you pick*/
int getInput()
{
    char * in;

    in = malloc(sizeof(char)*100);
    
    printf("1. add a new car to the available-for-rent list\n");
    printf("2. add a returned car to the available-for-rent list\n");
    printf("3. add a returned car to the repair list\n");
    printf("4. transfer a car from the repair list to the available-for-rent list\n");
    printf("5. rent the first available car\n");
    printf("6. print all the lists\n");
    printf("7. quit\n");
    
    do {
        printf("Please enter one of the numbers 1-7: ");
        fgets(in,99,stdin);
        in[strlen(in)-1]='\0';
    } while (!isGood(in));
    printf("\n");
    
    return in[0]-'0';
}

/*Returns whether the input is within 1-7*/
int isGood(char * in)
{
    return strlen(in)==1 && in[0]>='1'&&in[0]<='7';
}




/* LINKED LIST FUNCTION! */
/*Searches if a node is within the available-for-rent list*/
int searchIfInList(list * root,char * plateNum)
{
    list * temp;
    temp = root;
    
    while(temp != NULL)
    {
        if(strncmp(temp->plateNum,plateNum,99)==0)
        {
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}

/*Gets the length of a given list and returns it*/
int length(list * root)
{
    int counter = 0;
    list * temp = root;
    
    while(temp != NULL)
    {
        counter++;
        temp = temp->next;
    }
    return counter;
}

/*pops a value from the front of a list and returns it*/
list * popFront(list ** root)
{
    list * temp;
    temp = *root;
    
    *root = (*root)->next;
    temp->next = NULL;
    
    return temp;
}

/*Creates a new node*/
list * newNode(int mileage,char * plateNum)
{
    list * new;
    new = malloc(sizeof(list));
    
    new->plateNum = plateNum;
    new->mileage = mileage;
    new->returnDate =  NULL;
    new->next = NULL;
    
    return new;
}

/*Frees the contents of a node*/
void freeNode(list * toFree)
{
    free(toFree->plateNum);
    if(toFree->returnDate != NULL)
    {
        free(toFree->returnDate);
    }
    free(toFree);
}

/*Prints all contents of all lists*/
void printLists(list * avail, list * repair, list * rented)
{
    list * temp;
    printf("Available-for-rent:\n");
    temp = avail;
    
    while(temp != NULL)
    {
        printf("Mileage: %d   Plate number: %s\n",temp->mileage,temp->plateNum);
        temp = temp->next;
    }
    
    temp = repair;
    printf("Repair: \n");
    while(temp!=NULL)
    {
        printf("Mileage: %d   Plate number: %s\n",temp->mileage,temp->plateNum);
        temp = temp->next;
    }
    
    temp = rented;
    printf("Rented:\n");
    
    while(temp!=NULL)
    {
        printf("Return date: %s   Mileage: %d   Plate number: %s\n",temp->returnDate,temp->mileage,temp->plateNum);
        temp = temp->next;
    }
    printf("\n");
}

/*Inserts a node in the correct spot in the available-for-rent list*/
void insertAvail(list ** root,list * toAdd)
{
    list * temp;
    
    temp = *root;
    if(length((*root))==0)
    {
        *root = toAdd;
    }
    else
    {
        if(temp->mileage >= toAdd->mileage)
        {
            
            toAdd->next = temp;
            *root = toAdd;
            return;
        }
        
        while(temp->next != NULL){
            if(temp->next->mileage > toAdd->mileage)
            {
                list * veryTemp;
                veryTemp = temp->next;
                toAdd->next = veryTemp;
                temp->next = toAdd;
                return;
            }
            temp = temp->next;
        }
        if(temp->mileage <= toAdd->mileage)
        {
           
            temp->next = toAdd;
        }
    }
    
}

/*Inserts a node in the correct spot in the rented list*/
void insertRent(list ** root,list * toAdd)
{
    list * temp;
    
    temp = *root;
    if(length((*root))==0)
    {
        *root = toAdd;
    }
    else
    {
        if(strToNumber(temp->returnDate) > strToNumber(toAdd->returnDate))
        {
            toAdd->next = temp;
            *root = toAdd;
            return;
        }
        
        while(temp->next != NULL){
            if(strToNumber(temp->next->returnDate) > strToNumber(toAdd->returnDate))
            {
                list * veryTemp;
                veryTemp = temp->next;
                toAdd->next = veryTemp;
                temp->next = toAdd;
                return;
            }
            temp = temp->next;
        }
        
        temp->next = toAdd;
    }
}

/*Returns if good plate num*/
int isGoodPlate(char * plate)
{
    int i;
    if(strlen(plate)>7||strlen(plate)==0)
    {
	    return 0;
	}
    for(i=0;i<strlen(plate);++i)
    {
	    if((plate[i]>'9'||plate[i]<'0')&& (plate[i]>'Z' || plate[i]<'A'))
	    {
		    return 0;
		}
	           
    }
    return 1;
}

/*Gets input for options 1-3*/
list * addInput(list * available, list * repair, list * rented)
{
    int mileage;
    char * plate;
    char * mile;
    list * new;
    
    mile = malloc(sizeof(char)*100);
    plate = malloc(sizeof(char)*100);
    
    do
    {
        printf("Please enter valid a plate number (i.e. Capital letters, numbers, and length less than 8) you wish to add: ");
        fgets(plate,99,stdin);
        plate[strlen(plate)-1]='\0';
    }while(!isGoodPlate(plate));
    
    do
    {
        printf("Please enter a valid integer mileage: ");
        fgets(mile,99,stdin);
        mile[strlen(mile)-1]='\0';
    }while(!isNum(mile));
    
    mileage = strToNumber(mile);
    
    new = newNode(mileage, plate);
    
    printf("\n");
    free(mile);
    
    return new;
}

/*Searches for a node within the repair list*/
/*If it is there it returns the node and then deletes it within that list*/
list * searchRepair(list ** root, char * plate)
{
    list * temp = *root;
    int compareLimit = 99;
    
    if(strncmp(plate, temp->plateNum,compareLimit)==0)
    {
        (*root)=(*root)->next;
        temp->next = NULL;
        return temp;
    }
    while(temp->next != NULL)
    {
        if(strncmp(temp->next->plateNum,plate,compareLimit)==0)
        {
            list * veryTemp;
            veryTemp = temp->next;
            temp->next = temp->next->next;
            veryTemp->next = NULL;
            return veryTemp;
        }
    }
    return NULL;
}

/*Searches for a node within the rented list*/
/*If it is there, it deletes it and returns the change in mileage*/
int search(list ** root, list * node)
{
    list * temp = *root;
    int changeMile;
    int compareLimit = 99;
    if(strncmp(temp->plateNum,node->plateNum,compareLimit)==0)
    {
        changeMile = node->mileage - temp->mileage;
        *root = (*root)->next;
        return changeMile;
        
    }
    while(temp->next != NULL)
    {
        if(strncmp(temp->next->plateNum,node->plateNum,compareLimit)==0)
        {
            list * veryTemp;
            veryTemp = temp->next;
            temp->next = temp->next->next;
            changeMile = node->mileage-temp->mileage;
            freeNode(veryTemp);
            return changeMile;
        }
        temp = temp->next;
    }
    return -1;
}

/*Adds a new node to the front of a given list*/
void addToFront(list ** root, list * toAdd)
{
    toAdd->next = *root;
    *root = toAdd;
}

/*Frees all elements of every list*/
void deleteAllLists(list ** av, list ** rented,list ** repair)
{
    list * temp;
    
    while((*av) != NULL)
    {
        temp =(*av)->next;
        freeNode(*av);
        *av = temp;
    }
    
    while((*rented) != NULL)
    {
        temp =(*rented)->next;
        freeNode(*rented);
        *rented = temp;
    }
    
    while((*repair) != NULL)
    {
        temp =(*repair)->next;
        freeNode(*repair);
        *repair = temp;
    }
}
