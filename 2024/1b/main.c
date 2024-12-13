#include <stdio.h>
#include <stdlib.h>

typedef struct list {
	int n;
	int cleft;
	int cright;
	struct list *next;
} list_t;

void insert_number(list_t **p, int n, int cleft, int cright);
void get_lists(FILE *f, list_t **p);
void print_list(list_t *p);
void free_lists(list_t *p);

int main(int argc, char *argv[]){

	FILE *f = NULL;

	list_t *list = NULL;

	f = fopen("../1a/input", "r");
	if(f == NULL)
		goto cleanup;

	get_lists(f, &list);

	unsigned long d = 0;
	list_t *p = NULL;
	for(p=list;p!=NULL;p=p->next){
		d += p->cleft * p->n * p->cright;
	}

	printf("%lu\n", d);

cleanup:
	free_lists(list);
	if(f != NULL)
		fclose(f);
	return 0;
}

list_t *new_number(int n, list_t *next, int cleft, int cright){
	list_t *p = NULL;
	if(next != NULL && next->n == n){
		next->cleft += cleft;
		next->cright += cright;
		return next;
	}
	p = malloc(sizeof(list_t));
	if(p==NULL)
		return NULL;
	p->n = n;
	p->cleft = cleft;
	p->cright = cright;
	p->next = next;
	return p;
}

void insert_number(list_t **p, int n, int cleft, int cright){
	if(p==NULL)
		return;
	if(*p == NULL){
		*p = new_number(n, NULL, cleft, cright);
		return;
	}
	while(*p != NULL && (*p)->n < n){
		p = &(*p)->next;
	}
	*p = new_number(n, *p, cleft, cright);
}

void get_lists(FILE *f, list_t **p){
	int l;
	int r;
	while(fscanf(f, "%d %d\n", &l, &r)==2){
		insert_number(p, l, 1, 0);
		insert_number(p, r, 0, 1);
	}
}

void print_list(list_t *p){
	for(;p!=NULL;p = p->next)
		printf("%d (%d, %d)\n", p->n, p->cleft, p->cright);
}

void free_lists(list_t *p){
	list_t *next = NULL;
	while(p!=NULL){
		next = p->next;
		free(p);
		p = next;
	}
}
