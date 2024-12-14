#include <stdio.h>
#include <stdlib.h>

typedef struct page {
	int p;
	struct page *next;
} page_t;

typedef struct rule {
	int p;
	page_t *plist;
	struct rule *next;
} rule_t;

void get_rules(FILE *f, rule_t **pp_prev, rule_t **pp_post);

void rule_list_print(rule_t *p);
void rule_list_free(rule_t *p);

void page_list_print(page_t *p);
void page_list_free(page_t *p);

int main(int argc, char *argv[]){

	FILE *f = NULL;

	rule_t *rule_list_prev = NULL;
	rule_t *rule_list_post = NULL;

	f = fopen("input", "r");
	if(f == NULL)
		goto cleanup;

	get_rules(f, &rule_list_prev, &rule_list_post);
	if(rule_list_prev == NULL || rule_list_post == NULL)
		goto cleanup;

	/*
	printf("prev:\n");
	rule_list_print(rule_list_prev);
	printf("post:\n");
	rule_list_print(rule_list_post);
	*/

cleanup:
	rule_list_free(rule_list_prev);
	rule_list_free(rule_list_post);
	if(f != NULL)
		fclose(f);
	return 0;
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

void page_list_insert(page_t **pp, int p){
	for(;(*pp)!=NULL && (*pp)->p < p; pp = &(*pp)->next)
		;
	*pp = page_new(p, *pp);
}

rule_t *rule_new(int a, int b, rule_t *next){
	rule_t *p = NULL;
	if(next != NULL && next->p == b){
		page_list_insert(&next->plist, a);
		return next;
	}
	p = malloc(sizeof(rule_t));
	if(p == NULL)
		return p;
	p->p = b;
	p->plist = NULL;
	page_list_insert(&p->plist, a);
	p->next = next;
	return p;
}

void rule_list_insert(rule_t **pp, int a, int b){
	for(;(*pp)!=NULL && (*pp)->p < b; pp = &(*pp)->next)
		;
	*pp = rule_new(a, b, *pp);
}

void get_rules(FILE *f, rule_t **pp_prev, rule_t **pp_post){
	int a;
	int b;
	while(2 == fscanf(f, "%d|%d", &a, &b)){
		rule_list_insert(pp_prev, a, b);
		rule_list_insert(pp_post, b, a);
	}
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
