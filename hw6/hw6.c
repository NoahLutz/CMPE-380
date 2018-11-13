#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <getopt.h>
#include "ClassErrors.h"
#include "poly.h"



int main (int argc, char* argv[]) {
	
	int verbose = 0;
	char* input_file = NULL;


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
				//sscanf(optarg, "%s", input_file);
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
		//TODO: parse file
	}
		
	polynomial p = {0, NULL};
	initPoly(&p, 4);
	p.polyCoef[0] = 1;
	p.polyCoef[1] = 1;
	p.polyCoef[2] = 0;
	p.polyCoef[3] = 1;

	
	printf("P(x) = ");
	printPoly(&p);
	
	double complex *roots_array = roots(&p, 1E-6, verbose);
	double complex *cursor = roots_array;
	
	printf("Roots: \n");
	while (!isnan(creal(*cursor))) {
		if(cabs(cimag(*cursor)) > ZERO) {
			printf("\t%f%+fi\n", creal(*cursor), cimag(*cursor));
		}
		else {
			printf("\t%f\n", creal(*cursor));
		}
		cursor++;
	}	
	
	//double complex *results = evalDerivs(&p, (double complex)1.0);
	//printComplex(results[0]);
	//printf("\n");
	//printComplex(results[1]);
	//printf("\n");
	//printComplex(results[2]);
	//printf("\n");

	return PGM_SUCCESS;
}
