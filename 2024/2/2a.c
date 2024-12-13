#include <stdio.h>
#include <assert.h>

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

void get_input(FILE *f){
	int safe = 1;
	int incq = -1;
	int a = -1;
	int b = -1;
	int d;
	char c;
	int count = 0;
	while(2 == fscanf(f, "%d%c", &a, &c)){
		if(incq == -1){ /* estamos en la primera o segunda lectura del nivel */
			if(b == -1){ /* esta es la primera */
				b = a;
				/* solamente hay un nivel? */
				assert(c != '\n');
			}else{ /* es la segunda */
				if(a>b)
					incq = 1;
				else if(a<b)
					incq = 0;
				else
					safe = 0;
			}
		}
		/* comprobación */
		if(incq == 1){
			d = a-b;
			if(d<1 || d>3)
				safe = 0;
		}else if(incq == 0){
			d = b-a;
			if(d<1 || d>3)
				safe = 0;
		}
		b = a;
		if(c == '\n'){
			if(safe == 1)
				count++;
			safe = 1;
			a = b = -1;
			incq = -1;
		}
	}

	printf("%d\n", count);
}
