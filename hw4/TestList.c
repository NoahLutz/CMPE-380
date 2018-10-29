/******************************************************************************
 An student framework implementation to test the LinkedLists module.  
 Reads in a large list of words and puts them into the data structure, 
 then prints out the first and last six elements in the data structure as 
 well as the total number of words.
 Note: This is only a framework, it does not include ALL the functions or 
       code you will need.
******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ClassErrors.h"
#include "LinkedLists.h"

/* Local functions */
/******************************************************************************
 Program to test the Data structure by reading the Data file provided on
 the command line into the structure, printing out the first and last five
 elements and total number of elements.

 Where: int argc     - the number of parameters on the command line
        char *argv[] - Pointer to the command line parameters
 Returns: 0 - success, 
          2 - can't open input file
          1 - Invalid number of parameters
******************************************************************************/
int main(int argc, char* argv[])
  {
  /* declare local variables */
 
   /* One command line argument is required: the file name     */
   if (2 == argc) /* note that argc 2 means one argument given */
     {
      /* Print out an opening message with the file name 
         then try to open the Data file for input (read mode)   */
 

      /* Verify that file was opened correctly */
      if (NULL != DataFile)
        {
         /* Initialize the linked list */

         
         /* Read all Data from text file */

         
         /* Close the Data file */
  

         /* Print the first 6 entries */
         fprintf(stdout, "First 6 words in linked list:\n");
          
         /* Print the last 6 entries */
         fprintf(stdout, "Last 6 words in linked list:\n");
         
         /* Print total number of words read */
         fprintf(stdout, "Number of words: %d \n", TestLinkedList.NumElements);
         
         /* Remove from front of list, print a message */
             fprintf(stdout, "Remove from front of list, new front is: %s \n", );
             fprintf(stdout, "Removed value is: %s \n", );

         
         /* Remove from back of list, print a message */
             fprintf(stdout, "Remove from back of list, new back is: %s \n", );
             fprintf(stdout, "Removed value is: %s \n", );
           
            
         /* De-allocate the linked list */
         fprintf(stdout, "Destroying the linked list\n"); 

        } /* if() */
      else
      { /* Error message */
   
      } /* if...else() */
     } /* if() */
   else
     { /* Usage message */
  
     } /* if...else() */

   return ErrorCode;
  } /* main() */

