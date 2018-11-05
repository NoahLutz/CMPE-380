/**************************************************************************
  Student frame work.	Add and remove as you see fit.
  
  07/14/2017	 R. Repka	 Initial release
 * ***********************************************************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "ClassErrors.h"
#include "rootfinding.h"
#include "Timers.h"

/************************************************************************
  Tests three types of root finding, secant, newton, and bisection,
  based on user input and prints out the timing results.
************************************************************************/
int main(int argc, char* argv[]) {
	/*------------------------------------------------------------------------
		UI variables with sentential values
	------------------------------------------------------------------------*/
	int verbose = 0;
	enum modes {UNDEF, BISECT, SECANT, NEWTON} mode = UNDEF;
	double tol = 0.0;
	double guess1 = INFINITY;	
	double guess2 = INFINITY;

	/*------------------------------------------------------------------------
	 These variables are used to control the getopt_long_only command line 
	 parsing utility.  
	------------------------------------------------------------------------*/
	int rc;
	/* getopt_long stores the option index here. */
	int option_index = 0;
  
	/* This contains the short command line parameters list */
	char *getoptOptions = "vbsnt:g:u:";	 /* add lots of stuff here */
  
	/* This contains the long command line parameter list, it should mostly 
	 match the short list																  */
	struct option long_options[] = {
		/* These options don’t set a flag.
		We distinguish them by their indices. */
		{"verbose",		no_argument,		0,	'v'},
		{"verb",		no_argument,		0,	'v'},
		{"bisection",	no_argument,		0,	'b'},
		{"secant",		no_argument,		0,	's'},
		{"newton",		no_argument,		0,	'n'},
		{"tolerance",	required_argument,	0,	't'},
		{"guessa",		required_argument,	0,	'g'},
		{"ga",			required_argument,	0,	'g'},
		{"bracket_a",	required_argument,	0,	'g'},
		{"ba",			required_argument,	0,	'g'},
		{"guessb",		required_argument,	0,	'u'},
		{"gb",			required_argument,	0,	'u'},
		{"bracket_b",	required_argument,	0,	'u'},
		{"bb"		,	required_argument,	0,	'u'},
		/* add lots of stuff here */
		{0, 0, 0, 0}
	};
  
	opterr = 1;			  /* Enable automatic error reporting */
	while ((rc = getopt_long_only(argc, argv, getoptOptions, long_options, 
																	 &option_index)) != -1) {
		
		printf("getopt_long_only() returned ='%c' index = '%d'\n",  rc, option_index);	
		/* Detect the end of the options. */
		switch (rc) {
			case 'v':						  /* Verbose */
				verbose = 1;
				printf("setting verbose to %d\n", verbose);
				break;

			/* Handle method */
			case 'b':
				if (mode == UNDEF) {
					mode = BISECT;
				}
				break;

			case 's':
				if (mode == UNDEF) {
					mode = SECANT;
				}
				break;

			case 'n':
				if (mode == UNDEF) {
					mode = NEWTON;
				}
				break;

			/* Handle value arguments */
			case 't':
				sscanf(optarg, "%lf", &tol);
				break;

			case 'g':
				sscanf(optarg, "%lf", &guess1);
				break;

			case 'u':
				sscanf(optarg, "%lf", &guess2);
				break;

			/* Handle errors */
			case '?':  /* Handled by the default error handler */
				break;

			default:
				printf ("Internal error: undefined option %0xX\n", rc);
				exit(PGM_INTERNAL_ERROR);
		} // End switch 
	} /* end while */

	/*------------------------------------------------------------------------
		Check for command line syntax errors
	------------------------------------------------------------------------*/
	if ((optind < argc) /* add lots of stuff here */  ){
		fprintf(stderr, "Tests root finding methods\n");
		fprintf(stderr, "usage: hw5 -b[isection] | -s[ecant] | -n[ewton]	-t[ol[erance} number\n");
		fprintf(stderr, "			 -g[uess1] number	<-g[u]ess2 number	<-verb[ose]> \n");
		fprintf(stderr, " e.g:	hw5 -bisection -tol 1E-6 -g1 -2 -g2 3 -verb\n"); 
		fflush(stderr);
		return(PGM_INTERNAL_ERROR);
	} /* End if error */

	 
	/* Performs the root finding with bisection */
	if(mode == BISECT) {
		double value = bisection(&func1, guess1, guess2, tol, verbose);
		printf("Root found: %f\n", value);
	}  // End if bisection
	
	/* Performs the root finding using the secant method */
	else if(mode == SECANT) {

	} // End secant
	
	/* Performs the root finding using newtons method */
	else /* must be newton */ {

	} // End newton 
	
		  
	 return PGM_SUCCESS;
}
