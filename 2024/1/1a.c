#include <stdio.h>
#include <stdlib.h>

typedef struct list {
	int n;
	struct list *next;
} list_t;

void insert_number(list_t **p, int n);
void get_lists(FILE *f, list_t **l_p, list_t **l_r);
void print_list(list_t *p);
void free_lists(list_t *p);

int main(int argc, char *argv[]){

	FILE *f = NULL;

	list_t *left = NULL;
	list_t *right = NULL;

	f = fopen("input", "r");
	if(f == NULL)
		goto cleanup;

	get_lists(f, &left, &right);

	list_t *l = NULL;
	list_t *r = NULL;

	unsigned int d = 0;

	for(l=left,r=right; l != NULL && r != NULL; l=l->next,r=r->next){
		d += l->n > r->n ? l->n - r->n : r->n - l->n;
	}

	printf("%lu\n", d);

cleanup:
	free_lists(left);
	free_lists(right);
	if(f != NULL)
		fclose(f);
	return 0;
}

list_t *new_number(int n, list_t *next){
	list_t *p = NULL;
	p = malloc(sizeof(list_t));
	if(p==NULL)
		return NULL;
	p->n = n;
	p->next = next;
	return p;
}

void insert_number(list_t **p, int n){
	if(p==NULL)
		return;
	if(*p == NULL){
		*p = new_number(n, NULL);
		return;
	}
	while(*p != NULL && (*p)->n <= n){
		p = &(*p)->next;
	}
	*p = new_number(n, *p);
}

void get_lists(FILE *f, list_t **l_p, list_t **r_p){
	int l;
	int r;
	while(fscanf(f, "%d %d\n", &l, &r)==2){
		insert_number(l_p, l);
		insert_number(r_p, r);
	}
}

void print_list(list_t *p){
	for(;p!=NULL;p = p->next)
		printf("%d\n", p->n);
}

void free_lists(list_t *p){
	list_t *next = NULL;
	while(p!=NULL){
		next = p->next;
		free(p);
		p = next;
	}
}
