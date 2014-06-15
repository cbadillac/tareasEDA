#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#define B 1000

static FILE *data;

struct node
{
	// data
	char* name;
	char* last_name;
	int   old;
	// leaf properties
	unsigned int key;
	struct node *left;
	struct node *right;
};


unsigned int h(char *name, int age)
{
  int ret = 0;
  int i = 0;
  for (i=0; name[i] != '\0'; i++) {
     ret = 131 * ret + name[i];
  }
  ret = 131 * ret + age;
  return (ret % B);
}

typedef struct node* pnode;

pnode insrtNd (unsigned int key, pnode *leaf, char* name, char* last_name, int* old)
{
	if((*leaf) == 0) {

		pnode pi = (pnode) malloc(sizeof(struct node));
		if (pi == NULL){
			printf("ERROR: Can't be created node \n");
			exit(1);
		}
		// leaf properties
		pi -> left      = NULL;
		pi -> right     = NULL;
		pi -> key       = key;
		// data
		pi -> name      = strdup(name);
		pi -> last_name = strdup(last_name);
		pi -> old       = (*old);

		(*leaf) = pi;

	}
	else if(key < (*leaf)->key) {
		printf("Izquierda\n");
		(*leaf)->left  = insrtNd(key, &((*leaf)->left), name, last_name, old);

	}
	else if(key > ((*leaf)->key)) {
		printf("Derecha\n");
		(*leaf)->right = insrtNd(key, &((*leaf)->right), name, last_name, old);

	}
	else ;
	return((*leaf));
}

char* concat(char *s1, char *s2)
{
    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
    char *result =(char*) malloc(len1+len2+1);//+1 for the zero-terminator
    if(result == NULL) {
    	printf("ERROR: Malloc in concat is not work");
    	exit(1);
    }
    memcpy(result, s1, len1);
    memcpy(result+len1, s2, len2+1);//+1 to copy the null-terminator
    return result;
}

int readDatafl(FILE *file, char* name, char* last_name, int* old)
{
	int id;
	int rtrn = fscanf(file, "%d %s %s %d",
		       &id, last_name, name, old);
	if(rtrn <= 0) {
		//fscanf end
		return (0);
	}
	return(1);
}

int main(int argc, char const *argv[])
{
	data = fopen("data.txt", "r");

	char *name      = (char*) malloc(sizeof(char)*1024);
	char *last_name = (char*) malloc(sizeof(char)*1024);
	int  *old       = (int*)  malloc(sizeof(int));

	if(!data) {
		printf("ERROR: text file can't be opened\n");
		exit(1);
	}


	readDatafl(data, name, last_name, old);

	unsigned int key = h(concat(last_name, name), (*old));
	pnode root = 0;
	root = insrtNd(key,&root,name, last_name, old);

//	add the rest of list to tree through loop
	while(readDatafl(data, name, last_name, old)){

		readDatafl(data, name, last_name, old);
		key = h(concat(last_name, name), (*old));
		insrtNd(key,&root,name, last_name, old);
	}


	free(name);
	free(last_name);
	fclose(data);

	return 0;
}
