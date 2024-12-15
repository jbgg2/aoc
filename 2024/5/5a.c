#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct page {
	int p;
	struct page *next;
} page_t;

typedef struct rule {
	int p;
	page_t *plist;
	struct rule *next;
} rule_t;

typedef struct update {
	page_t *plist;
	int len;
	int endq;
	struct update *next;
} update_t;

void page_list_insert(page_t **pp, int p, int ordq);

void get_updates(FILE *f, update_t **pp);
void update_list_print(update_t *p);
void update_list_free(update_t *p);

int rules_check(char *rules, int pmax, int a, int b);

void fill_rules(rule_t *prev, rule_t *post, char *rules, int pmax);
void rules_print(char *rules, int pmax);

void get_rules(FILE *f, rule_t **pp_prev, rule_t **pp_post, int *pmax_p);

void rule_list_print(rule_t *p);
void rule_list_free(rule_t *p);

void page_list_print(page_t *p);
void page_list_free(page_t *p);

int main(int argc, char *argv[]){

	FILE *f = NULL;

	rule_t *rule_list_prev = NULL;
	rule_t *rule_list_post = NULL;

	int pmax = 0;
	char *rules = NULL;

	update_t *update_list = NULL;

	f = fopen("input", "r");
	if(f == NULL)
		goto cleanup;

	get_rules(f, &rule_list_prev, &rule_list_post, &pmax);
	if(rule_list_prev == NULL || rule_list_post == NULL)
		goto cleanup;

	rules = calloc((pmax+1)*(pmax+1), sizeof(char));
	if(rules == NULL)
		goto cleanup;

	fill_rules(rule_list_prev, rule_list_post, rules, pmax);

	//rules_print(rules, pmax);

	get_updates(f, &update_list);

	//update_list_print(update_list);

cleanup:
	update_list_free(update_list);
	free(rules);
	rule_list_free(rule_list_prev);
	rule_list_free(rule_list_post);
	if(f != NULL)
		fclose(f);
	return 0;
}

update_t *update_new(update_t *next){
	update_t *p = NULL;
	p = malloc(sizeof(update_t));
	if(p == NULL)
		return p;
	p->plist = NULL;
	p->endq = 0;
	p->len = 0;
	p->next = next;
	return p;
}

void update_list_insert(update_t **pp, int n){
	for(;*pp!=NULL && (*pp)->endq == 1;pp = &(*pp)->next)
		;
	if(*pp == NULL)
		*pp = update_new(*pp);
	(*pp)->len++;
	page_list_insert(&(*pp)->plist, n, 0);
}

void update_list_set_last_endq(update_t **pp){
	for(;*pp!=NULL && (*pp)->endq == 1;pp = &(*pp)->next)
		;
	assert(*pp != NULL);
	(*pp)->endq = 1;
	assert((*pp)->next == NULL);
}

void get_updates(FILE *f, update_t **pp){
	int n;
	char c;
	while(!feof(f)){
		c = 0;
		while(c != '\n' && 1 == fscanf(f, "%d", &n)){
			update_list_insert(pp, n);
			c = fgetc(f);
		}
		if(c != 0){
			update_list_set_last_endq(pp);
		}
	}
}

/* return 1 si page a is previous to b */
int rules_check(char *rules, int pmax, int a, int b){
	if(a < 0 || a > pmax || b < 0 || b > pmax){
		return 0;
	}
	return rules[b * (pmax+1) + a];
}

/* page a is previous to b */
void set_rule(char *rules, int pmax, int a, int b){
	if(a < 0 || a > pmax || b < 0 || b > pmax){
		assert(0);
		return;
	}
	rules[b * (pmax+1) + a] = 1;
}

void fill_rules(rule_t *prev, rule_t *post, char *rules, int pmax){
	page_t *p = NULL;
	for(;prev!=NULL;prev=prev->next){
		for(p=prev->plist;p!=NULL;p=p->next)
			set_rule(rules, pmax, p->p, prev->p);
	}
}

page_t *page_new(int p, page_t *next){
	page_t *t = NULL;
	t = malloc(sizeof(page_t));
	if(t == NULL)
		return t;
	t->p = p;
	t->next = next;
	return t;
}

void page_list_insert(page_t **pp, int p, int ordq){
	for(;(*pp)!=NULL && (!ordq || (*pp)->p < p); pp = &(*pp)->next)
		;
	*pp = page_new(p, *pp);
}

rule_t *rule_new(int a, int b, rule_t *next){
	rule_t *p = NULL;
	if(next != NULL && next->p == b){
		page_list_insert(&next->plist, a, 1);
		return next;
	}
	p = malloc(sizeof(rule_t));
	if(p == NULL)
		return p;
	p->p = b;
	p->plist = NULL;
	page_list_insert(&p->plist, a, 1);
	p->next = next;
	return p;
}

void rule_list_insert(rule_t **pp, int a, int b){
	for(;(*pp)!=NULL && (*pp)->p < b; pp = &(*pp)->next)
		;
	*pp = rule_new(a, b, *pp);
}

void get_rules(FILE *f, rule_t **pp_prev, rule_t **pp_post, int *pmax_p){
	int a;
	int b;
	int pmax = 0;
	long pos = 0;
	while(2 == fscanf(f, "%d|%d", &a, &b)){
		if(a>pmax)
			pmax = a;
		if(b>pmax)
			pmax = b;
		rule_list_insert(pp_prev, a, b);
		rule_list_insert(pp_post, b, a);
		pos = ftell(f);
	}
	fseek(f, pos, SEEK_SET);
	if(pmax_p)
		*pmax_p = pmax;
}

void page_print(page_t *p){
	printf("p%d\n", p->p);
}

void page_list_print(page_t *p){
	for(;p!=NULL;p=p->next)
		page_print(p);
}

void rule_print(rule_t *p){
	if(p == NULL)
		return;
	printf("r%d:\n", p->p);
	page_list_print(p->plist);
}

void rule_list_print(rule_t *p){
	for(;p!=NULL;p=p->next)
		rule_print(p);
}

void rule_list_free(rule_t *p){
	if(p != NULL){
		page_list_free(p->plist);
		rule_list_free(p->next);
	}
	free(p);
}

void page_list_free(page_t *p){
	if(p != NULL)
		page_list_free(p->next);
	free(p);
}

void rules_print(char *rules, int pmax){
	int i;
	int j;
	for(i=0;i<=pmax;i++){
		for(j=0;j<=pmax;j++){
			if(rules[j*(pmax+1) + i])
				printf("%d|%d\n", i, j);
		}
	}
}

void update_list_print(update_t *p){
	page_t *page_list = NULL;
	for(;p!=NULL;p=p->next){
		for(page_list=p->plist;page_list!=NULL;page_list=page_list->next){
			printf("%d,", page_list->p);
		}
		printf("\n");
	}
}

void update_list_free(update_t *p){
	if(p != NULL){
		update_list_free(p->next);
		page_list_free(p->plist);
	}
	free(p);
}
