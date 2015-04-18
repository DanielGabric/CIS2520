#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX(a,b) (a>b)?a:b

/*Binary tree struct*/
typedef struct Node
{
    struct Node * left;
    struct Node * right;
    char  *value;
    double variable;
}Node;

/*Function prototypes*/
int isOper(char value);
Node * createNode(char * value);
void createTree(Node ** root, int vars[10]);
void postOrder(Node * root);
void preOrder(Node * root);
void inOrder(Node * root);
void printTree(Node * root, int currLevel);
double eval(Node * root);
int isNum(char * val);
int isVar(char * val);
void deleteTree(Node ** root);
int isGoodIn(char input[]);
void findAndReplace(Node * root,char * var, double val);

int main(int argc, char ** argv)
{
    int vars[10];
    char *input;
    char *op5;
    Node * root;
    char in = '0';
    char *text;
    char * text1;
    double var;
	
    memset(&vars[0],0,10);
    input = malloc(sizeof(char)*15);
    op5 = malloc(sizeof(char)*50);
    root = createNode(argv[1]);
    createTree(&root,vars);
    
    /*command loop*/
    while(in != '7')
    {
	printf("1. Display\n");
	printf("2. Preorder\n");
	printf("3. Inorder\n");
	printf("4. Postorder\n");
	printf("5. Update\n");
	printf("6. Calculate\n");
	printf("7. Exit\n");
	/*defensive programming*/
	do
	{
	    printf("Please choose one of these enter corresponding number(1-7): ");
	    fgets(input,10,stdin);
	    input[strlen(input)-1]='\0';
	}
	while(!isGoodIn(input));
	in = input[0];
		
	/*Choices based on input*/
        switch(in)
        {
	    case '1':
		   /*Display*/
	        printTree(root,0);
		break;
	    case '2':
		    /*preorder*/
	        preOrder(root);
		printf("\n");
	        break;
            case '3':
			/*inorder*/
	        inOrder(root);
		printf("\n");
		break;
	    case '4':
		 /*postorder*/
		postOrder(root);
		printf("\n");
		break;
	    case '5':
	        /*Update*/
	        printf("Enter which variable you want to change and the value you wish to replace it with.\n");
		printf("Use the form \"variable_name, new_value\": ");
		fgets(op5,49,stdin);
		op5[strlen(op5)-1]='\0';
		text = strtok(op5,", ");
		if(isVar(text))
		{
			if(vars[text[1]-'0']!=1)
			{
			    printf("Variable not found!\n");
					}
					else
					{
						text1 = strtok(NULL,", ");
						if(!isNum(text1))
						{
							printf("Not valid number!\n");
						}
						else
						{
							sscanf(text1,"%lf",&var);
							findAndReplace(root,text,var);
						}
					}
				}
				else
				{
					printf("You did not enter a valid variable!\n");
				}  
				break;
			case '6':
			    /*Calculate */
			    printf("The expression evaluates to: %.2lf\n",eval(root));
				break;
		    case '7':
		        /*Exit*/
		        printf("Bye bye, see you another time!\n");
		        continue;
		        break;
		}
		/*Continue to next input*/
		printf("Press enter to continue!\n");
		fgets(op5,4,stdin);
	}

    /*Deletes the malloced memory*/
    free(input);
    free(op5);
    deleteTree(&root);
    return 0;
}

/*Finds a variable in the tree, and replaces the current value to a new one*/
void findAndReplace(Node * root,char * var, double val)
{
	if(root==NULL)
	{
		return;
	}
	if(strcmp(root->value,var)==0)
	{
		root->variable = val;
		return;
	}
	findAndReplace(root->left,var,val);
	findAndReplace(root->right,var,val);
}

/*Checks if input number is within specs*/
int isGoodIn(char input[])
{
	if(strlen(input)!= 1 || (input[0]<'1'||input[0]>'7'))
	{
		return 0;
	}
	return 1;
}

/*Deletes tree*/
void deleteTree(Node ** root)
{
    if((*root)==NULL)return;
    deleteTree(&((*root)->left));
    deleteTree(&((*root)->right));
    free((*root));
}

/*Checks if the current string is a variable*/
int isVar(char *val)
{
    if(val[0]=='x'&&(val[1]>='0'&&val[1]<='9')&&strlen(val)==2)
    {
    	return 1;
    }
    return 0;
}

/*Evaluates the expression and returns value*/
double eval(Node * root)
{
	if(!isOper(root->value[0]))
	{
		return root->variable;
	}
	else
	{
		if(root->value[0]=='+')
		{
			return eval(root->left)+eval(root->right);
		}
		else if(root->value[0]=='-')
		{
			return eval(root->left)-eval(root->right);
		}
		else if(root->value[0]=='/')
		{
			return eval(root->left)/eval(root->right);
		}
		else
		{
			return eval(root->left)*eval(root->right);
		}
	}
}

/*Checks that the string value is a valid real number*/
int isNum(char * val)
{
    int i;
    int isDecimal;

    isDecimal = 0;
    for(i=0;i<strlen(val);++i)
    {
		
	    if(((val[i]<'0'||val[i]>'9')&&val[i]!='.')||(val[i]=='.'&&isDecimal))
	    {
	        return 0;
	    }
	    if(val[i]=='.')
	    {
	    	isDecimal = 1;
	    }
    }
    return 1;
}

/*Use in order traversal, rotate the tree by 90 degrees*/
/*Makes printing tree 10x easier*/
void printTree( Node *root, int currLevel )
{
  int i;
  if ( root == NULL ) {
    return;
  }
  else
  {
    printTree( root->right, currLevel + 1 );

    for ( i = 0; i < currLevel; i++ )
    {
        putchar ('\t');
	}
	if(!isVar(root->value))
    {
		printf ("%s\n", root->value);
	}
	else
	{
		printf("%s=%.2lf\n",root->value,root->variable);
	}
    printTree(root->left,currLevel+1);
  }
}

/*In order traversal*/
void inOrder(Node * root)
{
	if(root==NULL)
	{
		return;
	}
	if(isOper(root->value[0]))
	{
		printf("(");
	}
	
	inOrder(root->left);
	printf("%s",root->value);
	inOrder(root->right);
	
    if(isOper(root->value[0]))
	{
		printf(")");
	}
}

/*Post order traversal*/
void postOrder(Node * root)
{
	if(root==NULL)
	{
		return;
	}
	postOrder(root->left);
	postOrder(root->right);
	printf("%s",root->value);
}

/*Pre order traversal*/
void preOrder(Node * root)
{
	if(root==NULL)
	{
		return;
	}
    printf("%s",root->value);
	preOrder(root->left);
	preOrder(root->right);
}

/*Creates tree recursively*/
/*Error checks string, and exits program if string is invalid*/
void createTree(Node ** root, int vars[10])
{
    int n = 0;
    char * value= malloc(sizeof(char)*(strlen((*root)->value)+2));
    int i;
  
    value = strcpy(value,(*root)->value);
    
    /*Error checking*/
    if(((*root)->value[0]=='(' && (*root)->value[strlen((*root)->value)-1]!=')')||
       ((*root)->value[0]!='(' && (*root)->value[strlen((*root)->value)-1]==')')||
       ((*root)->value[0]!='(' && (*root)->value[strlen((*root)->value)-1]!=')'&&!isVar((*root)->value)&&!isNum((*root)->value)))
    {
		printf("Unbalanced expression, exiting!\n");
		exit(0);
	}

    if((*root)->value[0]!='(' && (*root)->value[strlen((*root)->value)-1]!=')')
    {
		if(isVar((*root)->value))
		{
			int val = (*root)->value[1]-'0';
			/*Error checking*/
			if(vars[val]>=1)
			{
				printf("Duplicate of same variable! Exiting!\n");
				exit(0);
			}
			else
			{
				vars[val]++;
			}
		}
		return;
	}

    if((*root)->value[0]=='('&&(*root)->value[strlen((*root)->value)-1]==')')
    {

		for(i=0;i<strlen(value)-1;++i)
		{
			value[i]=value[i+1];
		}

		value[strlen(value)-2]='\0';

	}

    int c=0;
	int j=0;
	int k=0;
	for(i=0;i<strlen(value);++i)
	{
		if(value[i]=='(')
		{
			n++;
		}
        else if(value[i]==')')
		{
			n--;
		}
        
		if(n==0&&isOper(value[i]))
		{
			char* val1 = malloc(sizeof(char)*(i+1));
			char *val2 = malloc(sizeof(char)*(strlen(value)-i+3));
			(*root)->value[0]= value[i];
			(*root)->value[1]='\0';
			for(j=0;j<i;++j)
			{
				val1[j]=value[j];
			}
			for(j=i+1,k=0;j<strlen(value);++j,++k)
			{
				val2[k]=value[j];
			}

			(*root)->right = createNode(val2);
			(*root)->left = createNode(val1);
			createTree(&((*root)->left),vars);
			createTree(&((*root)->right),vars);
			c=1;
			break;
		}
	}
	/*Error checking*/
	if(!c)
	{
		printf("Invalid expression, exiting!\n");
		exit(0);
	}

    free(value);
}

/*Checks if character is an operator*/
int isOper(char value)
{
	return value == '+'||value=='/'||value=='-'||value=='*';
}

/*Creates a new node and returns it*/
Node * createNode(char * value)
{
    Node * newNode = malloc(sizeof(Node));
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->variable = 0;
    newNode->value = value;
    if(isNum(value))
    {
		sscanf(value,"%lf",&(newNode->variable));
	}
    return newNode;	
}
