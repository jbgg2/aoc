#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct pos {
	char c;
	int x;
	int y;
	struct pos *next;
} pos_t;

void pos_get(FILE *f, pos_t **pp, int *max, int *start);
pos_t *pos_new(int x, int y, char c, pos_t *next);
void pos_insert(pos_t **pp, int x, int y, char c);
void pos_free(pos_t *p);
void pos_print(pos_t *p);

int main(int argc, char *argv[]){

	FILE *f = NULL;
	pos_t *pos = NULL;
	int max[2] = {0, 0};
	int start[2] = {0, 0};

	f = fopen("input", "r");
	if(f==NULL)
		goto cleanup;
	pos_get(f, &pos, max, start);
	if(pos == NULL)
		goto cleanup;

cleanup:
	pos_free(pos);
	if(f)
		fclose(f);
	return 0;
}

void pos_get(FILE *f, pos_t **pp, int *max, int *start){
	int x = 0;
	int y = 0;
	int xmax = 0;
	int ymax = 0;
	int xstart = 0;
	int ystart = 0;
	char c;
	if(f == NULL || pp == NULL)
		return;
	while(!feof(f)){
		c = fgetc(f);
		if(c == '.'){
			pos_insert(pp, x, y, c);
			x++;
		}else if(c == '^'){
			pos_insert(pp, x, y, c);
			xstart = x;
			ystart = y;
			x++;
		}else if(c == '#'){
			pos_insert(pp, x, y, c);
			x++;
		}else if(c == '\n'){
			if(xmax==0)
				xmax = x;
			assert(xmax == x);
			x = 0;
			y++;
		}else
			assert(c!='.' && c!='#' && c!='^' && c!='\n');
	}
	ymax = y;
	if(max){
		max[0] = xmax;
		max[1] = ymax;
	}
	if(start){
		start[0] = xstart;
		start[1] = ystart;
	}
}

pos_t *pos_new(int x, int y, char c, pos_t *next){
	pos_t *p = NULL;
	p = malloc(sizeof(pos_t));
	if(p == NULL)
		return p;
	p->x = x;
	p->y = y;
	p->c = c;
	p->next = next;
	return p;
}

void pos_insert(pos_t **pp, int x, int y, char c){
	for(;*pp != NULL; pp = &(*pp)->next)
		;
	*pp = pos_new(x, y, c, *pp);
}

void pos_free(pos_t *p){
	if(p!=NULL)
		pos_free(p->next);
	free(p);
}

void pos_print(pos_t *p){
	for(;p!=NULL;p=p->next){
		printf("(%d,%d) %c\n", p->x, p->y, p->c);
	}
}
