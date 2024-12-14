#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct list {
	int n;
	struct list *next;
} list_t;

void get_input(FILE *f);

int main(int argc, char *argv[]){

	FILE *f = NULL;
	f = fopen("input", "r");
	if(f == NULL)
		goto cleanup;

	get_input(f);

cleanup:
	if(f)
		fclose(f);
	return 0;
}

void free_list(list_t *l){
	if(l != NULL)
		free_list(l->next);
	free(l);
}

list_t *new_list(int n, list_t *l){
	list_t *p = NULL;
	if(l == NULL){
		p = malloc(sizeof(list_t));
		p->n = n;
		p->next = l;
		return p;
	}else{
		assert(l->n == -1);
		l->n = n;
		if(l->next != NULL)
			l->next->n = -1;
		return l;
	}
	return NULL;
}

void insert_number(list_t **p, int n){
	for(;*p != NULL && (*p)->n != -1;p = &(*p)->next)
		;
	*p = new_list(n, *p);
}

void print_list(const list_t *l){
	for(;l!=NULL && l->n != -1;l = l->next)
		printf("%d\n", l->n);
}

int linecheck(list_t *l, list_t *nol){
	int incr = 1;
	int decr = 1;
	int previncr;
	int prevdecr;
	int d;

	if(l == NULL || l->n == -1)
		return 0;

	previncr = -1;
	prevdecr = -1;

	for(;l!=NULL && l->n != -1;l=l->next){
		if(l == nol)
			continue;
		if(previncr == -1){
			previncr = l->n;
		}else if(incr == 1){
			d = l->n - previncr;
			if(d < 1 || d > 3){
				incr = 0;
			}
			previncr = l->n;
		}
		if(prevdecr == -1){
			prevdecr = l->n;
		}else if(decr == 1){
			d = prevdecr - l->n;
			if(d < 1 || d > 3){
				decr = 0;
			}
			prevdecr = l->n;
		}
		if(incr != 1 && decr != 1)
			return 0;
	}

	assert(incr == 1 || decr == 1);
	return 1;
}

void get_input(FILE *f){
	list_t *l = NULL;
	int a;
	char c;
	int count = 0;
	int check = 0;
	list_t *nol = NULL;
	while(2 == fscanf(f, "%d%c", &a, &c)){
		insert_number(&l, a);
		if(c == '\n'){
			check = linecheck(l, NULL);
			if(check == 0){
				for(nol = l; nol!=NULL && nol->n != -1 && check == 0; nol=nol->next){
					check = linecheck(l, nol);
				}
			}
			count += check;
			if(l!=NULL)
				l->n = -1;
		}
	}
	free_list(l);
	printf("%d\n", count);
}
