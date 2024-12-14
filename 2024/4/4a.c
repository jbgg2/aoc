#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct pos {
	unsigned int x;
	unsigned int y;
	int astq;

	int touched;
	int ndetected;

	struct pos *next;
} pos_t;

pos_t *get_pos(pos_t **pos, unsigned int xmax, unsigned int ymax, unsigned int x, unsigned int y);
pos_t **create_pos(pos_t *p, unsigned int xmax, unsigned int ymax);

int get_input(FILE *f, pos_t **pp, unsigned int *xmax_p, unsigned int *ymax_p);
void free_list(pos_t *p);
int insert_pos(pos_t **pp, unsigned int x, unsigned int y, int astq);
pos_t *new_pos(unsigned int x, unsigned int y, int astq, pos_t *next);
void print_pos(const pos_t *p);
void print_list(const pos_t *p);

int main(int argc, char *argv[]){

	FILE *f = NULL;

	pos_t *listpos = NULL;
	pos_t **pos = NULL;

	unsigned int xmax = 0;
	unsigned int ymax = 0;

	unsigned int i;
	unsigned int j;

	f = fopen("input", "r");
	if(f == NULL)
		goto cleanup;

	if(get_input(f, &listpos, &xmax, &ymax) == 0)
		goto cleanup;

	printf("xmax: %u; ymax: %u\n", xmax, ymax);

	pos = create_pos(listpos, xmax, ymax);

	/* assert: pos is well defined */
	for(i=0;i<xmax;i++){
		for(j=0;j<ymax;j++){
			pos_t *p;
			p = get_pos(pos, xmax, ymax, i, j);
			assert(p != NULL);
			assert(p->x == i && p->y == j);
		}
	}

cleanup:
	free(pos);
	free_list(listpos);
	if(f != NULL)
		fclose(f);
	return 0;
}

pos_t *get_pos(pos_t **pos, unsigned int xmax, unsigned int ymax, unsigned int x, unsigned int y){
	if(pos == NULL)
		return NULL;
	if(x>=xmax || y>=ymax)
		return NULL;
	return pos[y*xmax + x];
}

pos_t **create_pos(pos_t *p, unsigned int xmax, unsigned int ymax){
	pos_t **r = NULL;
	pos_t **it = NULL;
	if(xmax == 0 || ymax == 0)
		return r;

	r = malloc(xmax*ymax*sizeof(pos_t*));
	if(r == NULL)
		return r;
	for(it = r;p!=NULL;p=p->next,it++){
		*it = p;
	}
	return r;
}

int get_input(FILE *f, pos_t **pp, unsigned int *xmax_p, unsigned int *ymax_p){
	unsigned int x = 0;
	unsigned int y = 0;
	char c;
	unsigned int xmax = 0;
	unsigned int ymax = 0;

	if(f == NULL || pp == NULL)
		return 0;
	while(!feof(f)){
		c = fgetc(f);
		if(c == '.'){
			insert_pos(pp, x, y, 0);
			x++;
		}else if(c == '#'){
			insert_pos(pp, x, y, 1);
			x++;
		}else if(c == '\n'){
			if(xmax == 0)
				xmax = x;
			else
				assert(x == xmax);
			x = 0;
			y++;
		}
	}
	ymax = y;
	if(xmax_p)
		*xmax_p = xmax;
	if(ymax_p)
		*ymax_p = ymax;
	return 1;
}

pos_t *new_pos(unsigned int x, unsigned int y, int astq, pos_t *next){
	pos_t *p;
	p = malloc(sizeof(pos_t));
	if(p == NULL)
		return p;
	p->x = x;
	p->y = y;
	p->astq = astq;
	p->touched = 0;
	p->ndetected = 0;
	p->next = next;
	return p;
}

int insert_pos(pos_t **pp, unsigned int x, unsigned int y, int astq){
	if(pp == NULL)
		return 0;
	for(;*pp != NULL; pp = &(*pp)->next)
		;
	*pp = new_pos(x, y, astq, *pp);
}

void free_list(pos_t *p){
	if(p != NULL)
		free_list(p->next);
	free(p);
}

void print_pos(const pos_t *p){
	if(p != NULL)
		printf("(%u,%u) %d %d\n", p->x, p->y, p->astq, p->touched);
}

void print_list(const pos_t *p){
	for(;p!=NULL;p=p->next)
		print_pos(p);
}
