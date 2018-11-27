#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <gsl/gsl_linalg.h>
#include "ClassErrors.h"
#include "Timers.h"

int main(int argc, char* argv[]) {
	int verbose = 0;
	char *input_file = NULL;
	FILE *file = NULL;
	char buffer[255];
	int nr,nc,s;

	int rc;
	int option_index = 0;
	char *getoptOptions = "vi:";

	DECLARE_TIMER(factor);
	DECLARE_TIMER(solve);

	struct option long_options[] = {
		{"verbose",	no_argument,		0,	'v'},
		{"verb",	no_argument,		0, 	'v'},
		{"input",	required_argument,	0,	'i'},
		{"in",		required_argument,	0,	'i'}
	};

	opterr = 1;
	while((rc = getopt_long_only(argc, argv, getoptOptions, long_options,
					&option_index)) != -1) {
		switch(rc) {
			case 'v':
				verbose = 1;
				break;
			case 'i':
				input_file = optarg;
				break;
			case '?':
				fprintf(stderr, "Encountered error parsing options.\n");
				exit(PGM_INTERNAL_ERROR);
				break;
			default:
				fprintf(stderr, "Internal error: undefined option %0xX\n", rc);
				exit(PGM_INTERNAL_ERROR);
				break;
		}
	}
	if(input_file==NULL) {
		fprintf(stderr, "No file specified.\n");
		exit(PGM_FILE_NOT_FOUND);
	}

	file = fopen(input_file, "r");
	if(file == NULL) {
		fprintf(stderr, "Could not open file for reading\n");
		exit(PGM_FILE_NOT_FOUND);
	}
	
	//Get first line (# rows and cols)
	if(fgets(buffer, 255, file) == NULL){
		fprintf(stderr, "Failed to read file\n");
		exit(PGM_INTERNAL_ERROR);
	}
	
	// Replace all tabs with space
	for(char* temp = buffer; *temp != '\0'; temp++) {
		if(*temp == '\t'){
			*temp=' ';
		}
	}

	// Parse rows and columns
	char* token = strtok(buffer, " ");
	if(token == NULL) {
		fprintf(stderr, "Error encountered while parsing file\n");
		exit(PGM_INTERNAL_ERROR);
	}
	nr = atoi(token);
	token = strtok(NULL, " ");
	if(token == NULL) {
		fprintf(stderr, "Error encountered while parsing file\n");
		exit(PGM_INTERNAL_ERROR);
	}
	nc = atoi(token);
	
	if(nr != nc-1) {
		fprintf(stderr, 
				"Matrix not square, cannot solve using Gaussian Elimination.\n");
		exit(PGM_INTERNAL_ERROR);
	}

	gsl_matrix *A = gsl_matrix_calloc(nr, nc-1);
	gsl_vector *b = gsl_vector_calloc(nr);
	gsl_vector *x = gsl_vector_calloc(nr);
	gsl_permutation *p = gsl_permutation_alloc(nr);
	
	double value = 0;
	double *a_data = malloc(nr*(nc-1) * sizeof(double));
	double *b_data = malloc(nr * sizeof(double));;

	int cnt = 0;
	int acnt = 0;
	int bcnt = 0;
	while(fscanf(file, "%lf", &value) != NULL && bcnt < nr) {
		if(((cnt+1) % nc) == 0 && cnt!=0) {
			b_data[bcnt++] = value;
		}
		else {
			a_data[acnt++] = value;
		}
		cnt++;
	}

	cnt = 0;
	bcnt = 0;
	for(int i = 0; i<nr; i++){
		for(int j = 0; j<nc-1; j++) {
			gsl_matrix_set(A,i,j,a_data[cnt++]);
		}

		gsl_vector_set(b,i,b_data[bcnt++]);
	}
	
	free(a_data);
	free(b_data);

	printf("********** %s **********\n", input_file);

	printf("A=\n");
	for(int i = 0; i<nr; i++) {
		for(int j = 0; j<nc-1; j++) {
			printf("%8.4f", gsl_matrix_get(A,i,j));
		}
		putchar('\n');
	}
	printf("\nb=\n");
	gsl_vector_fprintf(stdout, b, "\t%8.4f");

	START_TIMER(factor);
	gsl_linalg_LU_decomp(A, p, &s);
	STOP_TIMER(factor);
	
	if(verbose) {
		fprintf(stdout, "\nP = [");
		gsl_permutation_fprintf (stdout, p, " %u");
		fprintf(stdout, " ] \n");

		printf("LU matrix = \n");

		for(int i = 0; i<nr; i++) {
			for(int j = 0; j<nc-1; j++) {
				printf("%8.4f ", gsl_matrix_get(A,i,j));
			}
			putchar('\n');
		}
	}

	START_TIMER(solve);
	gsl_linalg_LU_solve(A, p, b, x);
	STOP_TIMER(solve);

	printf("\nx=\n");
	gsl_vector_fprintf(stdout, x, "\t%8.4f");

	gsl_matrix_free(A);
	gsl_vector_free(b);
	gsl_vector_free(x);
	gsl_permutation_free(p);

	PRINT_TIMER(factor);
	PRINT_TIMER(solve);
	return PGM_SUCCESS;
}
