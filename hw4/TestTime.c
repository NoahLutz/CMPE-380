#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ClassErrors.h"
#include "Timers.h"
#include "DynamicArrays.h"
#include "LinkedLists.h"

#define NUM_ITERATIONS 10
#define BUFFER_SIZE 256

int main(int argc, char* argv[]) {
	// Local variables
	FILE *file;
	char buffer[BUFFER_SIZE];
	LinkedLists list;
	DArray darray;
	DECLARE_TIMER(LinkedInputTimer);
	DECLARE_TIMER(LinkedSearchTimer);
	DECLARE_TIMER(DArrayInputTimer);
	DECLARE_TIMER(DArraySearchTimer);

	if (argc == 2) {
		printf("Opening %s for reading...\n", argv[1]);
		
		//Time linked list implementations
		DECLARE_REPEAT_VAR(linkedInVar);
		BEGIN_REPEAT_TIMING(NUM_ITERATIONS, linkedInVar)
		int word = 0;
		START_TIMER(LinkedInputTimer);
		file = fopen(argv[1], "r");

		//Check for success
		if (file != NULL) {
			// Initialize the lists
			InitLinkedList(&list);

         	/* Read all Data from text file */
			while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
				if(strstr(buffer, "\n") == NULL && !feof(file) ) {
					fprintf(stderr, "String too long, ignoring...\n");
					//Advance fp
					while (fgetc(file)!='\n');
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

		}
		else {
			fprintf(stderr, "Failed to open %s for reading\n", argv[1]);
			exit(PGM_FILE_NOT_FOUND);
		}

		fclose(file);
		STOP_TIMER(LinkedInputTimer);
		END_REPEAT_TIMING

		PRINT_RTIMER(LinkedInputTimer, NUM_ITERATIONS);
		
		//Time DArray list implementation
		DECLARE_REPEAT_VAR(darrayInVar);
		BEGIN_REPEAT_TIMING(NUM_ITERATIONS, darrayInVar)

		START_TIMER(DArrayInputTimer);
		int word = 0;
		file = fopen(argv[1], "r");

		//Check for success
		if (file != NULL) {
			// Initialize the lists
			CreateDArray(&darray, 0); // Want to have initial size of zero for fair comparison

         	/* Read all Data from text file */
			while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
				if(strstr(buffer, "\n") == NULL && !feof(file) ) {
					fprintf(stderr, "String too long, ignoring...\n");
					//Advance fp
					while (fgetc(file)!='\n');
					continue;
				}
				Data data;

				data.Num = word++;
				// Copy word and remove trailing newline char
				buffer[strcspn(buffer, "\r\n")] = 0;
				strcpy(data.String, buffer);

				PushToDArray(&darray, &data);
			}

		}
		else {
			fprintf(stderr, "Failed to open %s for reading\n", argv[1]);
			exit(PGM_FILE_NOT_FOUND);
		}

		fclose(file);
		STOP_TIMER(DArrayInputTimer);
		END_REPEAT_TIMING

		PRINT_RTIMER(DArrayInputTimer, NUM_ITERATIONS);

		// Search through linked list
		DECLARE_REPEAT_VAR(linkedSearchVar);
		BEGIN_REPEAT_TIMING(NUM_ITERATIONS, linkedSearchVar)
		START_TIMER(LinkedSearchTimer);
		SearchList(&list, "space");
		STOP_TIMER(LinkedSearchTimer);
		END_REPEAT_TIMING

		DECLARE_REPEAT_VAR(darraySearchVar);
		BEGIN_REPEAT_TIMING(NUM_ITERATIONS, darraySearchVar)
		START_TIMER(DArraySearchTimer);
		SearchDArray(&darray, "space");
		STOP_TIMER(DArraySearchTimer);
		END_REPEAT_TIMING
		
		PRINT_RTIMER(LinkedSearchTimer, NUM_ITERATIONS);
		PRINT_RTIMER(DArraySearchTimer, NUM_ITERATIONS);

		DestroyLinkedList(&list);
		DestroyDArray(&darray);
	}
	else {
		//TODO: print usage
			fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
			exit(PGM_SYNTAX_ERROR);
	}
	return PGM_SUCCESS;
}
