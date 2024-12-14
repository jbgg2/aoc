#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void get_input(FILE *f);

int main(int argc, char *argv[]){

	FILE *f = NULL;
	f = fopen("input", "r");

	get_input(f);

cleanup:
	if(f != NULL)
		fclose(f);
	return 0;
}

int get_number(FILE *f, unsigned int *p){
	int ret = 0;
	unsigned int a = 0;
	long pos = 0;
	int i;
	char c;
	if(p == NULL)
		goto cleanup;
	for(i=0;i<3;i++){
		pos = ftell(f);
		c = fgetc(f);
		if(isdigit(c)){
			ret = 1;
			a *= 10;
			a += c-'0';
		}else{
			fseek(f, pos, SEEK_SET);
			goto cleanup;
		}
	}
cleanup:
	if(ret)
		*p = a;
	return ret;
}

int get_mul(FILE *f, unsigned int *ap, unsigned int *bp){
	long pos = 0;
	unsigned int a;
	unsigned int b;

	if(ap == NULL || bp == NULL)
		return 0;

	pos = ftell(f);
	if(fgetc(f) == 'm' && fgetc(f) == 'u' && fgetc(f) == 'l' && fgetc(f) == '(' &&
		get_number(f, &a) && fgetc(f) == ',' && 
		get_number(f, &b) && fgetc(f) == ')'){
			*ap = a;
			*bp = b;
			return 1;
	}else
		fseek(f, pos, SEEK_SET);
	return 0;
}

int get_dont(FILE *f){
	long pos = 0;
	pos = ftell(f);
	if(fgetc(f) == 'd' && fgetc(f) == 'o' &&
		fgetc(f) == 'n' && fgetc(f) == '\'' &&
		fgetc(f) == 't' && fgetc(f) == '(' &&
		fgetc(f) == ')'){
			return 1;
	}else
		fseek(f, pos, SEEK_SET);
	return 0;
}

int get_do(FILE *f){
	long pos = 0;
	pos = ftell(f);
	if(fgetc(f) == 'd' && fgetc(f) == 'o' &&
		fgetc(f) == '(' && fgetc(f) == ')'){
			return 1;
	}else
		fseek(f, pos, SEEK_SET);
	return 0;
}

void get_input(FILE *f){
	unsigned int s = 0;
	unsigned int a = 0;
	unsigned int b = 0;
	long pos = 0;
	char c;
	int enableq = 1;
	while(!feof(f)){
		if(enableq){
			if(get_dont(f))
				enableq = 0;
			else if(get_mul(f, &a, &b))
				s += a*b;
			else
				fgetc(f);
		}else{
			if(get_do(f))
				enableq = 1;
			else
				fgetc(f);
		}
	}
	printf("%u\n", s);
}
