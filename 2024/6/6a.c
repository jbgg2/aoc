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

void pos_a_fill(pos_t *p, int *max, char *pa);
void pos_a_print(char *pa, int *max);

int inside(int *max, int *p);
void step(char *pa, int *max, int *p, int *di, int *pn);
void walk(char *pa, int *max, int *p, int di);

void pos_a_count(char *pa, int *max, unsigned int *s_p);

int main(int argc, char *argv[]){

	FILE *f = NULL;
	pos_t *pos = NULL;
	int max[2] = {0, 0};
	int start[2] = {0, 0};

	char *pa = NULL;
	unsigned int s = 0;

	f = fopen("input", "r");
	if(f==NULL)
		goto cleanup;
	pos_get(f, &pos, max, start);
	if(pos == NULL)
		goto cleanup;

	pa = calloc(max[0]*max[1], sizeof(char));
	if(pa == NULL)
		goto cleanup;
	pos_a_fill(pos, max, pa);

	walk(pa, max, start, 0);

	pos_a_count(pa, max, &s);

	printf("%u\n", s);

cleanup:
	free(pa);
	pos_free(pos);
	if(f)
		fclose(f);
	return 0;
}

void pos_a_count(char *pa, int *max, unsigned int *s_p){
	int i;
	int j;
	unsigned int s = 0;
	for(j=0;j<max[1];j++){
		for(i=0;i<max[0];i++){
			if(*pa > 0)
				s++;
			pa++;
		}
	}
	if(s_p)
		*s_p = s;
}

int inside(int *max, int *p){
	if(p[0]>=0 && p[0]<max[0] && p[1]>=0 && p[1]<max[1])
		return 1;
	return 0;
}

void step(char *pa, int *max, int *p, int *di, int *pn){
	int dirs[4][2] = { {0, -1}, {1, 0}, {0, 1}, {-1, 0} };
	int i;
	int pnext[2];
	for(i=0;i<4;i++){
		pnext[0] = p[0] + dirs[*di][0];
		pnext[1] = p[1] + dirs[*di][1];
		if(!inside(max, pnext) || pa[pnext[1] * max[0] + pnext[0]] >= 0){
			pn[0] = pnext[0];
			pn[1] = pnext[1];
			return;
		}
		*di = (*di + 1) % 4;
	}
	assert(0);
}

void walk(char *pa, int *max, int *start, int di){
	int p[2];
	int pnext[2];
	char c;

	p[0] = start[0];
	p[1] = start[1];
	while(inside(max, p)){
		// set to one
		assert(pa[p[1] * max[0] + p[0]] >= 0);
		pa[p[1] * max[0] + p[0]] = 1;
		// move
		step(pa, max, p, &di, pnext);
		p[0] = pnext[0];
		p[1] = pnext[1];
	}
}

void pos_a_fill(pos_t *p, int *max, char *pa){
	if(p == NULL || max == NULL || pa == NULL)
		return;
	for(;p!=NULL;p=p->next,pa++){
		if(p->c == '.')
			*pa = 0;
		else if(p->c == '^')
			*pa = 1;
		else if(p->c == '#')
			*pa = -1;
	}
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

void pos_a_print(char *pa, int *max){
	int i;
	int j;
	char c;
	for(j=0;j<max[1];j++){
		for(i=0;i<max[0];i++){
			c = pa[j*max[0] + i];
			if(c > 0)
				c = 'X';
			else if(c == 0)
				c = '.';
			else if(c < 0)
				c = '#';
			printf("%c", c);
		}
		printf("\n");
	}
}
