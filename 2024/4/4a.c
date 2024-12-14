#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

const char word[] = "XMAS";
const unsigned int wordlen = 4;

typedef struct let {
	char c;
	int x;
	int y;

	int n;

	struct let *next;
} let_t;

unsigned int get_sum(let_t **a, int xmax, int ymax);
void get_n(let_t **a, int xmax, int ymax);

void get_input(FILE *f, let_t **list, int *xmax_p, int *ymax_p);

let_t **create_array(let_t *list, int xmax, int ymax);
void print_array(let_t **a, int xmax, int ymax);

void print_let(let_t *list);
void print_list(let_t *list);
void insert_let(let_t **list_p, int x, int y, char c);
void free_list(let_t *list);

int main(int argc, char *argv[]){

	FILE *f = NULL;
	let_t *list = NULL;
	int xmax = 0;
	int ymax = 0;
	let_t **a = NULL;

	unsigned int s = 0;

	f = fopen("input", "r");
	if(f == NULL)
		goto cleanup;

	get_input(f, &list, &xmax, &ymax);
	if(list == NULL)
		goto cleanup;

	a = create_array(list, xmax, ymax);
	if(a == NULL)
		goto cleanup;

	get_n(a, xmax, ymax);

	s = get_sum(a, xmax, ymax);
	printf("%u\n", s);

cleanup:
	free(a);
	free_list(list);
	if(f)
		fclose(f);
	return 0;
}

unsigned int get_sum(let_t **a, int xmax, int ymax){
	int i;
	int j;
	unsigned int s = 0;
	for(i=0;i<xmax;i++){
		for(j=0;j<ymax;j++){
			if((*a)->n > 0)
				print_let(*a);
			s+=(*a)->n;
			a++;
		}
	}
	return s;
}

let_t *get_let(let_t **a, int xmax, int ymax, int x, int y){
	if(x<0 || x>=xmax || y<0 || y>=ymax)
		return NULL;
	return a[y*ymax + x];
}

/*
index 0: X
index 1: M
index 2: A
index 3: S
*/
int get_n_r(let_t **a, int xmax, int ymax, let_t *l, int index){
	int r;
	int x;
	int y;

	if(l == NULL)
		return 0;

	x = l->x;
	y = l->y;

	r = 0;
	if((index == 0 && l->c == 'X') || (index == 1 && l->c == 'M') || (index == 2 && l->c == 'A')){
		r += get_n_r(a, xmax, ymax, get_let(a, xmax, ymax, x-1, y-1), index+1);
		r += get_n_r(a, xmax, ymax, get_let(a, xmax, ymax, x, y-1), index+1);
		r += get_n_r(a, xmax, ymax, get_let(a, xmax, ymax, x+1, y-1), index+1);
		r += get_n_r(a, xmax, ymax, get_let(a, xmax, ymax, x+1, y), index+1);
		r += get_n_r(a, xmax, ymax, get_let(a, xmax, ymax, x+1, y+1), index+1);
		r += get_n_r(a, xmax, ymax, get_let(a, xmax, ymax, x, y+1), index+1);
		r += get_n_r(a, xmax, ymax, get_let(a, xmax, ymax, x-1, y+1), index+1);
		r += get_n_r(a, xmax, ymax, get_let(a, xmax, ymax, x-1, y), index+1);
	}else if(index == 3 && l->c == 'S')
		return 1;
	return r;
}

void get_n(let_t **a, int xmax, int ymax){
	let_t **pp;
	int i;
	int j;
	pp = a;
	for(i=0;i<xmax;i++){
		for(j=0;j<ymax;j++){
			(*pp)->n = get_n_r(a, xmax, ymax, *pp, 0);
			pp++;
		}
	}
}

void get_input(FILE *f, let_t **list_p, int *xmax_p, int *ymax_p){
	char c;
	int x = 0;
	int y = 0;
	int xmax = 0;
	int ymax = 0;
	if(f == NULL || list_p == NULL)
		return;
	while(!feof(f)){
		c = fgetc(f);
		if(isalpha(c)){
			insert_let(list_p, x, y, c);
			x++;
		}else if(c == '\n'){
			if(xmax == 0)
				xmax = x;
			else
				assert(xmax == x);
			x = 0;
			y++;
		}else
			assert(1);
	}
	ymax = y;
	if(xmax_p)
		*xmax_p = xmax;
	if(ymax_p)
		*ymax_p = ymax;
}

let_t *create_let(int x, int y, char c, let_t *next){
	let_t *p = NULL;
	p = malloc(sizeof(let_t));
	if(p == NULL)
		return p;
	p->x = x;
	p->y = y;
	p->c = c;
	p->n = 0;
	p->next = next;
	return p;
}

void insert_let(let_t **list_p, int x, int y, char c){
	for(;(*list_p)!=NULL;list_p = &(*list_p)->next)
		;
	*list_p = create_let(x, y, c, *list_p);
}

void free_list(let_t *list){
	if(list != NULL)
		free_list(list->next);
	free(list);
}

void print_let(let_t *l){
	printf("(%d,%d) %c; %d\n", l->x, l->y, l->c, l->n);
}

void print_list(let_t *list){
	for(;list!=NULL;list=list->next)
		print_let(list);
}

let_t **create_array(let_t *list, int xmax, int ymax){
	let_t **a = NULL;
	let_t **pp = NULL;
	a = malloc(xmax * ymax * sizeof(let_t*));
	if(a == NULL)
		return a;
	for(pp=a;list!=NULL;list=list->next,pp++)
		*pp = list;
	return a;
}

void print_array(let_t **a, int xmax, int ymax){
	int i;
	int j;
	for(i=0;i<xmax;i++){
		for(j=0;j<ymax;j++){
			print_let(*a);
			a++;
		}
	}
}
