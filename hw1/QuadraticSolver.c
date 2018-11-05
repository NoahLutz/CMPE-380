/********************************************************************
 * Charles Noah Lutz
 * cnl9674@rit.edu
 *
 * Homework 0, Program to find the roots of the quadratic equation
 *   p(x) = 0, where p(x) = a*x^2 + b*x + c
 *   The program reads the coefficients from the command line as
 *   a b c, in that order, then it echoes the polynomial equation,
 *   and prints its roots.
 * 
 * Last Update: Juan C. Cockburn 1/27/2015 (juan.cockburn@rit.edu)
 * 05/05/2017   R. Repka    Introducted errors to fix
 ********************************************************************/

/* Include Standard Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Define CONSTANTS and PARAMETERIZED MACROS */
#define COEFF_A (2)
#define COEFF_B (1)
#define COEFF_C (0)
#define DISCRIMINANT(a,b,c) ((b)*(b)-4.0*(a)*(c))

/* Program Begins, note arguments of main */
int main (int argc, char *argv[]) {
  /* Declare and initialize local variables */
   char Version[] = "v2.0e-jc";     /* String with Version of the program   */
   int ReturnCode = 0;          /* Return code for OS                   */
   int Index;                       /* Loop index variable                  */
   float Polynomial[COEFF_A+1];     /* Array for quadratic coefficients     */
   float Discriminant;              /* Discriminant of quadratic equation   */
   float Root1, Root2;              /* Real roots                           */
   float Real, Imaginary;           /* Complex conjugate roots              */

  /* Print title and version of program */
   printf ("Quadratic Solver, version %s\n", Version);

  
   if (argc >= 4) { /* Check for enough command line arguments */
      for (Index = 2; Index >= 0; Index--) {        /* parse coefficients */
         Polynomial[Index] = atof (argv[3-Index]);  /* convert to float */ 
      } /* for Index */

      /* Show equation to be solved (for verification) */
      printf ("Equation to be solved:  %gx^2 + %gx + %g = 0\n",
              Polynomial[COEFF_A],Polynomial[COEFF_B],Polynomial[COEFF_C]);

      /* Calculate discriminant - note use of macro */
      Discriminant = DISCRIMINANT(Polynomial[COEFF_A],
                                  Polynomial[COEFF_B],Polynomial[COEFF_C]);

      /* Check if roots are real or complex */
      if (Discriminant < 0.0) { /* Complex conjugate roots */
         Real = -Polynomial[COEFF_B] / (2.0 * Polynomial[COEFF_A]);
         Imaginary = sqrt(-Discriminant)/ (2.0 * Polynomial[COEFF_A]);
         
         /* Print complex roots */
         printf ("The quadratic has complex roots:  %f +/- j%f\n",
                 Real, Imaginary);
       } /* if Discriminant */
       
       else { /* Real roots */
          Root1 = sqrt(Discriminant);
          Root2 = (-Polynomial[COEFF_B] + Root1)/(2.0 * Polynomial[COEFF_A]); 
          Root1 = (-Polynomial[COEFF_B] - Root1)/(2.0 * Polynomial[COEFF_A]);
          /* Print real roots */
          printf ("The quadratic has real roots:  %f and %f\n",
                  Root1, Root2);
       } /* else Discriminant */
    } /* if argc */
    
    else {
       /* Not enough arguments, print usage message to stderr */
       fprintf (stderr,"Equation: a x^2 + b x + c = 0 \n");
       fprintf (stderr,"Usage: %s a b c\n",argv[0]);
       ReturnCode; /* Invocation error: return -1 to the OS */
   }  /* else argc */
   
   return (ReturnCode);
} /* main () */
