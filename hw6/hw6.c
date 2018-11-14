#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <getopt.h>
#include "ClassErrors.h"
#include "poly.h"

polynomial* readPolynomial(FILE* file);

int main (int argc, char* argv[]) {
	
	int verbose = 0;
	char* input_file = NULL;
	FILE *file = NULL;

	int rc;
	int option_index = 0;
	char *getoptOptions = "vi:";

	struct option long_options[] = {
		{"verbose",	no_argument,		0,	'v'},
		{"verb",	no_argument,		0,	'v'},
		{"input",	required_argument,	0,	'i'},
		{"in",		required_argument,	0,	'i'}
	};
	
	opterr = 1;
	while ((rc = getopt_long_only(argc, argv, getoptOptions, long_options,
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
		}
	}

	if(input_file != NULL) {
		printf("File is: %s\n", input_file);
		file = fopen(input_file, "r");

		if(file != NULL){
			char buffer[255];
			//Loop though all lines
			while(fgets(buffer, 255, file) != NULL) {
				int nterms = 0;
				double complex coef_buffer[255];
				polynomial poly;

				//tokenize the string to get all coeficents
				char *token = strtok(buffer, " ");
				
				//Parse all coeficients
				while(token != NULL) {
					coef_buffer[nterms] = atof(token);
					nterms++;
					token = strtok(NULL, " ");
				}

				//Initialize the polynomial 
				initPoly(&poly, nterms);
				
				//Set the coeficients
				for(int i = nterms-1; i>= 0; i--) {
					poly.polyCoef[nterms-1-i] = coef_buffer[i];
				}
				
				printf("P(x) = ");
				printPoly(&poly);

				double complex *roots_array = roots(&poly, ZERO, verbose);
				double complex *cursor = roots_array;
				
				printf("Roots:\n");
				while(!isnan(creal(*cursor))) {
					if(cabs(cimag(*cursor)) > ZERO) {
						printf("\t%f%+fi\n", creal(*cursor), cimag(*cursor));
					}
					else {
						printf("\t%f\n", creal(*cursor));
					}
					cursor++;
				}

				free(roots_array);
				freePoly(&poly);
			}
			fclose(file);
		}
		else {
			fprintf(stderr, "Could not open file %s for reading.\n", input_file);
			exit(PGM_FILE_NOT_FOUND);
		}

	}
	else {
		fprintf(stderr, "No input file specified.\n");
		exit(PGM_FILE_NOT_FOUND);
	}

	return PGM_SUCCESS;
}

