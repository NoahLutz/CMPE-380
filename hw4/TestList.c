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

#define BUFFER_SIZE 256
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
int main(int argc, char* argv[]) {
  	/* declare local variables */
	FILE* DataFile;
	LinkedLists list;
	LinkedListNodes* cursor;
	char buffer[BUFFER_SIZE];
	int word = 0;

	/* One command line argument is required: the file name     */
	if (2 == argc) {/* note that argc 2 means one argument given */
      /* Print out an opening message with the file name 
         then try to open the Data file for input (read mode)   */
 		printf("Opening %s for reading...\n", argv[1]);

		DataFile = fopen(argv[1], "r");
		
    	/* Verify that file was opened correctly */
    	if (NULL != DataFile) {


        	/* Initialize the linked list */
         	InitLinkedList(&list);

		
         	/* Read all Data from text file */
			while (fgets(buffer, BUFFER_SIZE, DataFile) != NULL) {
				if(strstr(buffer, "\n") == NULL && !feof(DataFile) ) {
					fprintf(stderr, "String too long, ignoring...\n");
					//Advance fp
					while (fgetc(DataFile)!='\n');
					continue;
				}

				ElementStructs* data = malloc(sizeof(ElementStructs));
				MALLOC_DEBUG(data);
				if(data==NULL) {
					fprintf(stderr, "Failed to allocate memory\n");
					exit(MALLOC_ERROR);
				}

				data->number = word++;
				// Copy word and remove trailing newline char
				buffer[strcspn(buffer, "\r\n")] = 0;
				strcpy(data->buffer, buffer);

				AddToBackOfLinkedList(&list, data);
			}

         	/* Close the Data file */
  			fclose(DataFile);
         	
			/* Print the first 6 entries */
         	fprintf(stdout, "First 6 words in linked list:\n");
          	
			cursor = list.FrontPtr;
			while (cursor != NULL && cursor->ElementPtr->number < 6) {
				printf("%s\n", cursor->ElementPtr->buffer);
				cursor = cursor->Next;
			}

			printf("\n");

         	/* Print the last 6 entries */
         	fprintf(stdout, "Last 6 words in linked list:\n");

			cursor = list.BackPtr;
			while (cursor != NULL && cursor->ElementPtr->number > list.NumElements - 6) {
				printf("%s\n", cursor->ElementPtr->buffer);
				cursor = cursor->Previous;
			}
         
			printf("\n");

         	/* Print total number of words read */
         	fprintf(stdout, "Number of words: %d \n", list.NumElements);
         
         	/* Remove from front of list, print a message */
			ElementStructs *front = RemoveFromFrontOfLinkedList(&list);
			if(list.FrontPtr != NULL) {
            	fprintf(stdout, "Remove from front of list, new front is: %s\n", 
						list.FrontPtr->ElementPtr->buffer);
			}
			else {
            	fprintf(stdout, "Remove from front of list, new front is: %s \n", 
						"NULL");
			}

			if(front != NULL) {
            	fprintf(stdout, "Removed value is: %s \n", front->buffer);
				// Free data
				FREE_DEBUG(front);
				free(front);
			}
			else {
            	fprintf(stdout, "Removed value is: %s \n", "NULL");
			}
         
         	/* Remove from back of list, print a message */
			ElementStructs *back = RemoveFromBackOfLinkedList(&list);
			if(list.BackPtr != NULL) {
            	fprintf(stdout, "Remove from back of list, new back is: %s \n", 
						list.BackPtr->ElementPtr->buffer);
			}
			else {
            	fprintf(stdout, "Remove from back of list, new back is: %s \n", 
						"NULL");
			}

			if(back != NULL) {
            	fprintf(stdout, "Removed value is: %s \n", back->buffer);
				// Free data
				FREE_DEBUG(back);
				free(back);
			}
			else {
            	fprintf(stdout, "Removed value is: %s \n", "NULL");
			}
           
            
         	/* De-allocate the linked list */
         	fprintf(stdout, "Destroying the linked list\n"); 
			DestroyLinkedList(&list);

    	} /* if() */
      	else { /* Error message */
   			fprintf(stderr, "Failed to open %s for reading.\n", argv[1]);
			exit(PGM_FILE_NOT_FOUND);
      	} /* if...else() */
	} /* if() */
   	else { /* Usage message */
		fprintf(stderr, "Usage: ./TestList <input.txt>\n");
		exit(PGM_SYNTAX_ERROR);
    } /* if...else() */
	return PGM_SUCCESS;
} /* main() */

