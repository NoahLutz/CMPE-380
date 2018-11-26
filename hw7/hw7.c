#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <getopt.h>
#include "ClassErrors.h"
#include "apmatrix.h"


int main(int argc, char* argv[]) {
	int verbose = 0;
	FILE *file = NULL;

	char buffer[255];
	char *token;
	int rows,cols;
	Matrix* G = NULL;
	Matrix* A = NULL;
	iVector* p = NULL;
	rVector* b = NULL;

	int rc;
	int option_index = 0;
	char *getoptOptions = "vi:";

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
				file = fopen(optarg, "r");
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

	if(file == NULL) {
		fprintf(stderr, "Could not open file for reading\n");
		exit(PGM_FILE_NOT_FOUND);
	}

	//Get first line (# rows and cols)
	if(fgets(buffer, 255, file) == NULL){
		fprintf(stderr, "Failed to read file\n");
		exit(PGM_INTERNAL_ERROR);
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

	//Allocate space for G matrix
	G = m_alloc(rows, cols);

	//Allocate space for A matrix
	A = m_alloc(rows, cols-1);
	int cur_row = 0;
	int cur_col = 0;

	while(fgets(buffer, 255, file) != NULL) {
		token = strtok(buffer, " ");
		while(token!=NULL) {
			G->mat[cur_row][cur_col] = atof(token);
			cur_col++;
			token = strtok(NULL, " ");
		}
		cur_row++;
		cur_col = 0;
	}

	m_print(G, "%f\t");
	
	//Copy over vector into A (minus b vector)
	for(int i = 0; i<rows; i++) {
		for(int j = 0; j<cols-1; j++) {
			A->mat[i][j] = G->mat[i][j];
		}
	}

	//Set up b vector
	b = rv_alloc(A->nr);
	for(int i = 0; i<G->nr; i++) {
		b->vec[i] = G->mat[i][G->nc-1];
	}
	printf("b:\n");
	rv_print(b, "%f\n");

	m_print(A, "%f\t");
	
	//Set up p vector
	p = iv_alloc(A->nr);
	for(int i = 0; i<A->nr; i++){
		p->ivec[i] = i;
	}

	PLU_factor(A, p);

	m_print(A, "%f\t");

	PLU_solve(A, p, b, NULL);

	m_print(A, "%f\t");

	return PGM_SUCCESS;
}
