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
			break;
		}
	}
	if(ret)
		*p = a;
cleanup:
	return ret;
}

void get_input(FILE *f){
	unsigned int s = 0;
	unsigned int a = 0;
	unsigned int b = 0;
	long pos = 0;
	char c;
	while(!feof(f)){
		c = fgetc(f);
		pos = ftell(f);
		if(c == 'm'){
			if(fgetc(f) == 'u' && fgetc(f) == 'l' && fgetc(f) == '(' &&
				get_number(f, &a) && fgetc(f) == ',' && 
				get_number(f, &b) && fgetc(f) == ')'){
					s += a*b;
			}else
				fseek(f, pos, SEEK_SET);
		}
	}
	printf("%u\n", s);
}
