#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <getopt.h>
#include "ClassErrors.h"
#include "Timers.h"
#include "apmatrix.h"


int main(int argc, char* argv[]) {
	int verbose = 0;
	char *input_file = NULL;
	FILE *file = NULL;

	char buffer[255];
	char *token;
	int rows,cols;
	Matrix* A = NULL;
	iVector* p = NULL;
	rVector* b = NULL;
	rVector* x = NULL;

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
	token = strtok(buffer, " ");
	if(token == NULL) {
		fprintf(stderr, "Error encountered while parsing file\n");
		exit(PGM_INTERNAL_ERROR);
	}
	rows = atoi(token);
	token = strtok(NULL, " ");
	if(token == NULL) {
		fprintf(stderr, "Error encountered while parsing file\n");
		exit(PGM_INTERNAL_ERROR);
	}
	cols = atoi(token);
	
	if(rows != cols-1) {
		fprintf(stderr, 
				"Matrix not square, cannot solve using Gaussian Elimination.\n");
		exit(PGM_INTERNAL_ERROR);
	}

	//Allocate space for A matrix
	A = m_alloc(rows, cols-1);

	//Set up b vector
	b = rv_alloc(A->nr);

	double *a_data = malloc(rows*(cols-1) * sizeof(double));
	double *b_data = malloc(rows * sizeof(double));;
	
	double value = 0;
	int cnt = 0;
	int acnt = 0;
	int bcnt = 0;
	while(fscanf(file, "%lf", &value) != 0 && bcnt < rows) {
		if(((cnt+1) % cols) == 0 && cnt!=0) {
			b_data[bcnt++] = value;
		}
		else {
			a_data[acnt++] = value;
		}
		cnt++;
	}

	fclose(file);

	cnt = 0;
	bcnt = 0;
	for(int i = 0; i<rows; i++){
		for(int j = 0; j<cols-1; j++) {
			A->mat[i][j] = a_data[cnt++];
		}

		b->vec[i] = b_data[bcnt++];
	}

	free(a_data);
	free(b_data);

	printf("********** %s **********\n", input_file);

	printf("A=\n");
	m_print(A, "%8.4f");

	printf("b=\n");
	rv_print(b, "\t%8.4f\n");

	//Set up x vector
	x = rv_alloc(A->nr);
	
	//Set up p vector
	p = iv_alloc(A->nr);
	for(int i = 0; i<A->nr; i++){
		p->ivec[i] = i;
	}
	
	START_TIMER(factor);
	int status = PLU_factor(A, p);
	STOP_TIMER(factor);

	if(status == 3) {
		fprintf(stdout, "Failed to find solution for given matrix.\n");
		m_free(A);
		iv_free(p);
		rv_free(b);
		rv_free(x);
		exit(PGM_INTERNAL_ERROR);
	}

	if(verbose) {
		printf("P = [ ");
		iv_print(p, "%d ");
		printf("]\n");
		
		printf("LU matrix = \n");
		m_print(A, "%8.4f");
	}

	START_TIMER(solve);
	PLU_solve(A, p, b, x);
	STOP_TIMER(solve);

	printf("x = \n");
	rv_print(x, "\t%8.4f\n");

	m_free(A);
	rv_free(b);
	rv_free(x);
	iv_free(p);
	
	fflush(stdout);
	PRINT_TIMER(factor);
	PRINT_TIMER(solve);
	return PGM_SUCCESS;
}
