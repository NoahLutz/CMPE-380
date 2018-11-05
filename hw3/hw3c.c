/**************************************************************************
 ** Linear Curve Fitting 
 **   This program fits a line to the data points in the text file provided
 **   on the command line (one data point per line)
 **
 ** Revised: Juan C. Cockburn - 9/11/2014
 **          R. Repka         -12/12/2015 - added error checking
  **         R. Repka         -02/20/2016 - Added include file hint
 **************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* add other include files as necessary */
#include "Timers.h"  

#define NUM_ITERATIONS	100
/* structure to hold header of dynamic array */
typedef struct {
   double *Data_X;      /* Pointer to X data dynamic array */
   double *Data_Y;      /* Pointer to X data dynamic array */
   int Size;            /* Current Size of dynamic arrays */
   int NextElement;     /* Index of element next to last used entry in the arrays */
  } LinearFit;
 
/* Function prototypes */ 
void CalculateCoefficients(LinearFit *DataSet, double *A, double *B);
void AddPoint(LinearFit *DataSet, double X, double Y);
  
  
/**************************************************************************
* Main program to fit a line to a data set using "batch" least squares
**************************************************************************/
int main(int argc, char *argv[]){
	/* Declare a LinearFit data structure */
	LinearFit DataSet; 
  
	double A, B; /* Variables for coefficients of least-square line       */
	double X, Y; /* Temporary variables to hold data point read from file */ 
	int Done;    /* "Boolean" variable to indicate all data has been read */
	FILE *InputFile = NULL; /* Input file pointer for data file                  */
  
	/* Check that a command line argument was provided */
	if (1 != argc){
	
		DECLARE_TIMER(data);
		DECLARE_TIMER(calculation);

		DECLARE_REPEAT_VAR(var);
		BEGIN_REPEAT_TIMING(NUM_ITERATIONS ,var)

		START_TIMER(data);

		/* Open input file for reading -- it should be a valid filename */    
		InputFile = fopen(argv[1], "r");
		if (NULL == InputFile){
			fprintf(stderr, "Error: Input file '%s' not found\n", argv[1]);
			return(-1);
    	}
 
		/* Start with minimally sized arrays */
    	DataSet.Size = 1;
 
    	/* Allocate the arrays */
    	DataSet.Data_X = (double *)malloc(sizeof(double) * DataSet.Size);
    	DataSet.Data_Y = (double *)malloc(sizeof(double) * DataSet.Size);
    
    	if ((NULL == DataSet.Data_X) || (NULL == DataSet.Data_Y)) {
			fprintf(stderr, "Error: Could not allocate memory at line %d\n", __LINE__);
        	exit(-99);
       	}
     
    	/* Initialize the index where the next data point will go */
    	DataSet.NextElement = 0;
 
    	/* Read all of the data from the file */
    	do {
       		/* Read X,Y data point and if read did not go beyond end-of-file, 
          	add it to the data set */
       		if (fscanf(InputFile, "%lf %lf", &X, &Y) != EOF){
          		/* Append the new data point to the array */
          		AddPoint(&DataSet, X, Y);
          		Done = 0;
         	} /* if() */
       		else{
    			/* Set flag indicating that all the data has been read */
          		Done = 1;
        	} /* if...else() */
    	} while (!Done);
 		
		STOP_TIMER(data);
		
		START_TIMER(calculation);

    	/* Compute the coefficients of the least squares line */
    	CalculateCoefficients(&DataSet, &A, &B);

    	STOP_TIMER(calculation);

    	/* Return dynamic memory for data to the heap */
    	free(DataSet.Data_X);
    	free(DataSet.Data_Y);
 
    	/* Disconnect the input file from the stream */
    	fclose(InputFile);

		END_REPEAT_TIMING
		PRINT_RTIMER(data, NUM_ITERATIONS);
		PRINT_RTIMER(calculation, NUM_ITERATIONS);

    	/* Print out the line that fits the data set. */
    	printf("The best least square line is: Y = %g * X + %g\n", A, B);
 
    } /* if() */
  	else {
    	/* Display program usage information */
      	printf("Fits a line to data points\n");
      	printf("(C Version) Usage: %s Filename\n", argv[0]);
	} /* if...else() */
 
   	return 0;
} /* main() */

/**************************************************************************
* AddPoint() - Accepts a single point and appends it to the array expanding
*              the size of the arrays if necessary.
**************************************************************************/
void AddPoint(LinearFit *DataSet, double X, double Y)
  {
  int tmp_old_size; /* temp variable to store size when full */
   /* Store the data point (X,Y) into the arrays */
   DataSet->Data_X[DataSet->NextElement] = X;
   DataSet->Data_Y[DataSet->NextElement] = Y;
 
   /* Increment index for the next point and see if that point will be */
   /* beyond the size of the arrays */
   if (++DataSet->NextElement >= DataSet->Size)
     {
      /* Increase the size of the arrays by 1 */
      tmp_old_size = DataSet->Size;
      DataSet->Size += 1;
 
      /* Declare AND allocate new (larger) arrays for the additional data */
      double *NewData_X = (double *)malloc(sizeof(double) * DataSet->Size);
      double *NewData_Y = (double *)malloc(sizeof(double) * DataSet->Size);
      
      /* Check for any errors */
      if ((NULL == NewData_X) || (NULL == NewData_Y)) 
         {
          fprintf(stderr, "Error: Could not allocate memory at line %d\n", __LINE__);
          exit(-99);
         }
      
      /* Copy the existing data points to the new arrays */
      int lcv;
      for (lcv = 0; lcv < tmp_old_size ; lcv++)
        {
         NewData_X[lcv] = DataSet->Data_X[lcv];
         NewData_Y[lcv] = DataSet->Data_Y[lcv];
        } /* for() */
 
      /* De-allocate (return to heap) the old (smaller) arrays */
      free(DataSet->Data_X);
      free(DataSet->Data_Y);
 
      /* Point to the new arrays to be used from now on */
      DataSet->Data_X = NewData_X;
      DataSet->Data_Y = NewData_Y;
     } /* if() */
  } /* AddPoint() */
  

/**************************************************************************
*  CalculateConstant() - Calculate coefficients A and B best linear fit 
*                        equation: Y = A * X + B
**************************************************************************/
void CalculateCoefficients(LinearFit *DataSet, double *A, double *B)
  {
   /* Declare and initialize sum variables */
   double S_XX = 0.0;
   double S_XY = 0.0;
   double S_X  = 0.0;
   double S_Y  = 0.0;
   int lcv;
 
   /* Compute the sums */
   for (lcv=0; lcv < DataSet->NextElement; lcv++)
    {
      S_XX += DataSet->Data_X[lcv] * DataSet->Data_X[lcv];
      S_XY += DataSet->Data_X[lcv] * DataSet->Data_Y[lcv];
      S_X  += DataSet->Data_X[lcv];
      S_Y  += DataSet->Data_Y[lcv];
    } /* for() */
 
   /* Compute the parameters of the line Y = A*X + B */
   (*A) = (((DataSet->NextElement * S_XY) - (S_X * S_Y)) / ((DataSet->NextElement * S_XX) - (S_X * S_X)));
   (*B) = (((S_XX * S_Y) - (S_XY * S_X)) / ((DataSet->NextElement * S_XX) - (S_X * S_X)));
  } /* CalculateCoefficients() */
