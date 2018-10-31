#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ClassErrors.h"
#include "LinkedLists.h"


#define BUFFER_SIZE 256

int main(int argc, char* argv[]) {
	FILE *DataFile;
	char buffer[BUFFER_SIZE];
	int word = 0;

	LinkedLists list;
	
	// Check for arguments
	if(3 == argc) {
		printf("Opening %s for reading...\n", argv[1]);

		DataFile = fopen(argv[1], "r");

		if (DataFile != NULL) {
			InitLinkedList(&list);
			
			// Read data from text file
			while (fgets(buffer, BUFFER_SIZE, DataFile) != NULL) {
				if(strstr(buffer, "\n") == NULL && !feof(DataFile)) {
					fprintf(stderr, "String too long, ignoring...\n");
					// Advance fp
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

			// Close data file
			fclose(DataFile);

			ElementStructs *value = SearchList(&list, argv[2]);

			if(value != NULL){
				fprintf(stdout, "Found value: %s @ position %d in linked list\n", 
						argv[2], value->number);
			}
			else {
				fprintf(stdout, "Did not find value %s in linked list\n", argv[2]);
			}
		}
		else {
			fprintf(stderr, "Failed to open %s for reading\n", argv[1]);
			exit(PGM_FILE_NOT_FOUND);
		}
	}
	else {
		printf("Usage: %s <input_file> <value>\n", argv[0]);
		exit(PGM_SYNTAX_ERROR);
	}
	return PGM_SUCCESS;
}
