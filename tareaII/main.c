#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>
#define B 1000

static FILE *data;
static FILE *readData;

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

typedef struct node* pnode;


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

unsigned int arrayLenght(struct node *array)
{
	int i = 0;
	for(i=0; array[i] > 0; i++){
	;;
	}
	return i-1;
}

unsigned int partition(struct node *a, int first, int last)
{
	unsigned int pivot  = a[first].key;
	unsigned int pleft  = first +1;
	unsigned int pright = last;

	int notDone = 1;

	while(notDone) {
		while( (pivot >= a[pleft].key) && (pleft <= pright) ) {
		    pleft++;
		}

		while((pivot <= a[pright].key) && (pright >= pleft)) {
		    pright--;
		}

		if(pright < pleft) {
			notDone = 0;
		}else{

			struct node temp = a[pleft];
			a[pleft] = a[pright];
			a[pright] = temp;
		}

	}
	struct node temp = a[first];
	a[first] = a[pright];
	a[pright] = temp;

	return pright;

}

void quickSort(struct node *a, int first, int last){
	if(first <= last) {
		unsigned int splitpoint = partition(a, first, last);
		quickSort(a, first, splitpoint -1);
		quickSort(a, splitpoint+1, last);
	}
}

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
		(*leaf)->left  = insrtNd(key, &((*leaf)->left), name, last_name, old);

	}
	else if(key > ((*leaf)->key)) {
		(*leaf)->right = insrtNd(key, &((*leaf)->right), name, last_name, old);

	}
	else {}
	return((*leaf));
}

pnode findNode(unsigned int key, pnode leafTree)
{
	if(leafTree ==NULL) {
		return 0;
	}

	if(leafTree->key == key){
		return leafTree;
	}
	else if(key > leafTree->key) {
		findNode(key, leafTree->right);
	}
	else if(key < leafTree->key) {
		findNode(key,leafTree->left);
	}
	else return NULL;
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

	if(rtrn > 0) {
		//fscanf end

		return (1);
	}else {
		return(0);
	}

}

double quadratic_sum(struct node *a, double prom)
{

	double cont = 0;
	int i = 0;
	for (i = 0; i < arrayLenght(a); i++){
		cont = cont + ((a[i].old -prom)*(a[i].old -prom));
	}
	return cont;
}

int sum_data(struct node *a)
{
	int i = 0;
	int cont = 0;
	for (i = 0; i < arrayLenght(a); i++){
		cont = cont + a[i].old;
	}
	return cont;
}

int main(int argc, char const *argv[])
{
	data = fopen("data.txt", "r");
	if(!data) {
			printf("ERROR: text file can't be opened\n");
			exit(1);
		}
	struct node info[10000];

	char *name      = (char*) malloc(sizeof(char)*1024);
	char *last_name = (char*) malloc(sizeof(char)*1024);
	int  *old       = (int*)  malloc(sizeof(int));

	readDatafl(data, name, last_name, old);

	unsigned int key = h(concat(last_name, name), (*old));
	pnode root = 0;
	root = insrtNd(key,&root,name, last_name, old);
	int id = 0;
	int isNotOK = 1;

//	add the rest of list to tree through loop
	while(fscanf(data, "%d %s %s %d", &id, last_name, name, old) != EOF){
			key = h(concat(last_name, name), (*old));
			insrtNd(key,&root,name, last_name, old);
	}

	fclose(data);
	readData = fopen("read-data.txt", "r");
	if (!readData){
			printf("ERROR: Text file can't be opened\n");
			exit(1);
		}

	isNotOK = 1;
	pnode leaf;
	int i = 0;
	while(isNotOK){
		isNotOK = readDatafl(readData, name, last_name, old);
		key = h(concat(last_name, name),(*old));
		leaf = findNode(key, root);
		if(leaf != NULL){
			info[i] = (*leaf);
			i++;
		}
	}

	quickSort(info, 0, arrayLenght(info)-1);
	double prom    = sum_data(info)/arrayLenght(info);
	double dsv_std = sqrt(quadratic_sum(info, prom)/arrayLenght(info));
	printf("%.2f %.2f\n", prom, dsv_std);
	for (i = 0; i < arrayLenght(info); i++) {
			printf("%s %s %u\n", info[i].last_name, info[i].name, info[i].key);
	}
	free(name);
	free(last_name);
	fclose(readData);


	return 0;
}
