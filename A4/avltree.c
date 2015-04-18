#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct avl{
    char * data;
    int freq;
    int height;
    struct avl * left;
    struct avl * right;
}Node;

Node *createNode(char * data);
int getIn();
int isGood(char * in);
Node * getFromTextFile(char * filePath,Node * root);
void search(Node * root, char * data);
Node * insert(Node * root, char * data, int isInit);
Node * delete(Node * root, char * data);
int height(Node * node);
int bf(Node * toGet);
Node * leftRotation(Node * root);
Node * rightRotation(Node * root);
void inorder(Node * root);
int max(int x, int y);
void getSize(Node * root, int * c);
void getFullHeight(Node * root,int height,int * c);
void deleteTree(Node * root);
int isNum(char * n);
int strToNum(char * n);
void printAllBiggerEqual(Node * root, int freq);

int main()
{
    int input;
    char * key;
    char * key1;
    Node * avlTree;
    int h;
    int s;
    
    key1 = malloc(sizeof(char)*100);
    key = malloc(sizeof(char)*100);
    h=0;
    s=0;


    avlTree = NULL;
    input = 0;
    /*Command loop*/
    while(input != 7)
    {
        input = getIn();
        switch(input)
        {
			/*Initialization*/
            case 1:
                printf("filename: ");
                fgets(key,99,stdin);
                key[strlen(key)-1]='\0';
                avlTree=getFromTextFile(key, avlTree);
                break;
            /*Find*/
            case 2:
                printf("find key: ");
                fgets(key1,99,stdin);
                key1[strlen(key1)-1]='\0';
                search(avlTree,key1);
                break;
            /*Insert*/
            case 3:
                printf("insert key: ");
                fgets(key,99,stdin);
                key[strlen(key)-1]='\0';
                avlTree = insert(avlTree,key,0);
                /*inorder(avlTree);*/
                break;
            /*Remove*/    
            case 4:
                printf("remove key: ");
                fgets(key,99,stdin);
                /*printf("2\n");*/
                key[strlen(key)-1]='\0';
                /*printf("3\n");*/
                avlTree = delete(avlTree,key);
                /*inorder(avlTree);*/
                break;
            /*Check Height and Size*/    
            case 5:
                h=0;
                s=0;
                getFullHeight(avlTree,1,&h);
                getSize(avlTree,&s);
                printf("height: %d, size: %d\n",h,s);
                break;
            /*Find All (above a given frequency)*/
            case 6:
                do
                {
					printf("frequency: ");
					fgets(key,99,stdin);
					key[strlen(key)-1]='\0';
				}
				while(!isNum(key));	
				printAllBiggerEqual(avlTree,strToNum(key));
                break;
        }
        key1[0]='\0';
        key[0]='\0';
    }
    printf("See you later!\n");
    free(key1);
    free(key);
    deleteTree(avlTree);
    return 0;
}

/*Option 6: traverses tree and prints all nodes with frequencies higher than or equal to freq*/
void printAllBiggerEqual(Node * root, int freq)
{
	if(root == NULL)
	{
	    return;
	}
	if(root->freq >= freq)
	{
		printf("key: %s, frequency: %d\n",root->data,root->freq);
	}
	printAllBiggerEqual(root->left,freq);
	printAllBiggerEqual(root->right,freq);
}

/*make shift string to integer*/
int strToNum(char * n)
{
	int r =1;
	int x = 0;
	int i;
	for(i=0;i<strlen(n);++i)
	{
		x+=(n[i]-'0')*r;
		r*=10;
	}
	return x;
}

/*Deletes the whole tree*/
void deleteTree(Node * root)
{
	if(root==NULL)
	{
		return;
	}
	deleteTree(root->right);
	deleteTree(root->left);
	free(root->data);
	free(root);
	
}

/*Returns whether a string is a number or not*/
int isNum(char * n)
{
	int i;
	for(i=0;i<strlen(n);++i)
	{
		if(n[i]<'0'||n[i]>'9')
		{
			return 0;
		}
	}
	return 1;
}

/*Gets the total height of the tree*/
void getFullHeight(Node * root, int height,int * c)
{
	if(root==NULL)
	{
		return;
	}
	(*c) = max((*c),height);
	getFullHeight(root->right,height+1,c);
	getFullHeight(root->left,height+1,c);
}

/*Gets the TOTAL size of the tree*/
void getSize(Node * root, int * c)
{
	if(root==NULL)
	{
		return;
	}
	(*c)++;
	getSize(root->left,c);
	getSize(root->right,c);
}

/*Deletes node from binary search tree*/
Node * delete(Node * root, char * data)
{
	int b;
	/*printf("0\n");*/
	if(root == NULL)
	{
		printf("no_such_key\n");
		return NULL;
	}


    if(strcmp(data,root->data)>0)
    {
        root->right = delete(root->right,data);
    }
    else if(strcmp(data,root->data)<0)
    {
        root->left = delete(root->left,data);
    }
    else
    {
		if(root->freq>1)
		{
            (root)->freq--;
            printf("key: %s, frequency: %d\n",(root)->data,(root)->freq);
		}
		else
		{
			Node * x = NULL;
			if(root->left == NULL || root->right == NULL)
			{
				if(root->left)
				{
					x = root->left;
				}
				else
				{
					x = root->right;
				}
				if(x != NULL)
				{
					*root = *x;
				}
				else
				{
					x = root;
					root = NULL;
				}
				free(x->data);
				free(x);
			}
			else
			{
				x = root->right;
				while(x->left != NULL)
				{
					x = x->left;
				}
				root->data = x->data;
				root->right = delete(root->right,x->data);
			}
			printf("key: %s, frequency: 0\n",data);
		}
    }
    /*printf("1\n");*/
    
    if(root == NULL)
    {
		return NULL;
	}
	root->height = max(height(root->left),height(root->right))+1;
	b = bf(root);
	
	    /*Right left*/
    if(b==-2&&bf(root->right)>0)
    {

        root->right = rightRotation(root->right);

        return leftRotation(root);
    }
    /*Right Right*/
    if(b==-2 &&bf(root->right)<=0)
    {

        return leftRotation(root);
        
    }
    /*Left Left*/
    if(b == 2 && bf(root->left)>=0)
    {
        return rightRotation(root);
    }
    /*Left Right*/
    if(b==2 && bf(root->left)<0)
    {
	
        root->left = leftRotation(root->left);
        return rightRotation(root);
    }
    
	return root;
}

/*Function to get max of two integers*/
int max(int x, int y)
{
    return y>x?y:x;
}

/*In order traversal for debugging*/
void inorder(Node * root)
{
    if(root == NULL)return;
    
    inorder(root->left);
    printf("In order: %s, count: %d, height: %d, bf: %d\n",root->data,root->freq,root->height,bf(root));
    
    inorder(root->right);
}

/*Rotates root left for avl tree*/
Node * leftRotation(Node * root)
{
    Node * temp;
    temp = root->right;
    root->right = temp->left;
    temp->left = root;
    root->height = max(height(root->left),height(root->right))+1;
    temp->height = max(height(temp->right),height(temp->left))+1;
    return temp;
}

/*Rotates root right for avl tree*/
Node * rightRotation(Node * root)
{
    Node * temp;
    temp = root->left;
    root->left = temp->right;
    temp->right = root;
    root->height = max(height(root->left),height(root->right))+1;
    temp->height = max(height(temp->left),height(temp->right))+1;
    return temp;
}

/*Gets balance factor*/
int bf(Node * toGet)
{
    if(toGet == NULL)
    {
        return 0;
    }
    return height(toGet->left)-height(toGet->right);
}

/*Gets height of a node*/
int height(Node * node)
{
    if(node == NULL)
    {
        return 0;
    }
    return node->height;
}

/*Option 1: Initializes tree with all words from filepath*/
Node * getFromTextFile(char * filePath, Node * root)
{
    FILE * filePtr;
    char * line;
    
    line = malloc(sizeof(char)*50);
    filePtr = fopen(filePath,"r");
    if(filePtr == NULL)
    {
        printf("error in opening file\n");
        return NULL;
    }
    while(EOF!=fscanf(filePtr,"%s",line))
    {
        root = insert(root,line,1);
        line[0]='\0';
    }
    
    fclose(filePtr);
    free(line);
    return root;
}

/*Searches for a node in a binary search tree*/
void search(Node * root, char * data)
{
    if((root)==NULL)
    {
        printf("no_such_key\n");
        return;
    }
    if(strcmp(data,(root)->data)==0)
    {
        printf("key: %s, frequency: %d\n",(root)->data,(root)->freq);
        return;
    }
    else if(strcmp(data,(root)->data)>0)
    {
        search(root->right,data);
    }
    else
    {
        search(root->left,data);
    }
}

/*Inserts a node into a binary search tree*/
Node * insert(Node * root, char * data, int isInit)
{
    /*balance*/
    int b;
    if(root==NULL)
    {
        return createNode(data);
    }
    
    if(strcmp(data,(root)->data)==0)
    {
        (root)->freq++;
        if(!isInit)
        {
            printf("key: %s, frequency: %d\n",(root)->data,(root)->freq);
        }
       
    }
    else if(strcmp(data,root->data)>0)
    {
        root->right = insert(root->right,data,isInit);
    }
    else
    {
        root->left = insert(root->left,data,isInit);
    }
    
    root->height = max(height(root->right),height(root->left)) + 1;
    b = bf(root);

    /*Right left*/
    if(b==-2 && strcmp(data,root->right->data)<0)
    {
        root->right = rightRotation(root->right);
        return leftRotation(root);
    }
    /*Right Right*/
    if(b==-2 && strcmp(data,root->right->data)>0)
    {
        return leftRotation(root);
        
    }
    /*Left Left*/
    if(b == 2 && strcmp(data,root->left->data)<0)
    {
        return rightRotation(root);
    }
    /*Left Right*/
    if(b==2 && strcmp(data,root->left->data)>0)
    {
        root->left = leftRotation(root->left);
        return rightRotation(root);
    }
    return root;
}

/*Checks if valid input for command loop*/
int isGood(char * in)
{
    return strlen(in)==1 && in[0]>='1'&&in[0]<='7';
}

/*Gets the initial input for 1-7*/
int getIn()
{
    char * in;
    in = malloc(sizeof(char)*100);
    printf("1. Initialization\n");
    printf("2. Find\n");
    printf("3. Insert\n");
    printf("4. Remove\n");
    printf("5. Check Height and Size\n");
    printf("6. Find All (above a given frequency)\n");
    printf("7. Exit\n");
    do {
        printf("avl/> ");
        fgets(in,99,stdin);
        in[strlen(in)-1]='\0';
    } while (!isGood(in));
    return in[0]-'0';
}

/*Creates a new node*/
Node *createNode(char * data)
{
    Node * new;
    new = (Node*)malloc(sizeof(Node));
    new->freq = 1;
    new->height = 1;
    
    new->data = malloc(sizeof(char)*100);
    new->data = strcpy(new->data,data);
    new->left = NULL;
    new->right = NULL;
    return new;
}
