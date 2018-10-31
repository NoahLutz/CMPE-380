/******************************************************************************
 An student framework implementation of doubly linked lists that holds 
 elements containing a 256 character string and a sequence number.
 12/24/2017 - R. Repka     Removed AddToFrontOfLinkedList()
 03/12/2018 - R. Repka     Added pseudo code 
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ClassErrors.h"
#include "LinkedLists.h"


/******************************************************************************
  Pseudo code: 
  
  add 1st node
    LinkedList front = points to node 1,  LinkedList back = points to node 1
      Node 1 next     = NULL  (at the end)
      Node 1 previous = NULL (at the start)
          
  add 2nd node
    LinkedList front = points to node 1,  LinkedList back = points to node 2
      Node 1 next points to node 2             Node 2 next   = NULL  (at the end)
      Node 1 previous = NULL (at the start)    Node 2 previous points to node 1

  add 3rd node
  LinkedList front = points to node 1,  LinkedList back = points to node 3
      Node 1 next points to node 2           Node 2 next points to node 3      Node 3 next   = NULL  (at the end)
      Node 1 previous = NULL (at the start)  Node 2 previous points to node 1  Node 3 previous points to node 2
      
      
   Remove from front of 3 node list 
    save the pointer to the data and return it at the end
    LinkedList front = points to node 2,  LinkedList back = points to node 3
      Node 2 next points to node 3           Node 3 next   = NULL  (at the end)
      Node 2 previous = NULL (at the start)  Node 3 previous points to node 2
      free Node but NOT the data
******************************************************************************/

/******************************************************************************
 Initialize the linked list data structure.  Points to nothing, no entries.

 Where: LinkedLists *ListPtr - Pointer to the linked list to create
 Returns: nothing
 Errors: none
******************************************************************************/
void InitLinkedList(LinkedLists *ListPtr) {
	// Set initial list to empty
	ListPtr->NumElements=0;
	ListPtr->FrontPtr = NULL;
	ListPtr->BackPtr = NULL;
}


/******************************************************************************
 Adds a node to the back of the list.

 Where: LinkedLists *ListPtr    - Pointer to the linked list to add to
        ElementStructs *DataPtr - Pointer to the data to add to the list
 Returns: nothing
 Errors: Prints to stderr and exits
******************************************************************************/
void AddToBackOfLinkedList(LinkedLists *ListPtr, ElementStructs *DataPtr) {
	LinkedListNodes *newNode = malloc(sizeof(LinkedListNodes));
	MALLOC_DEBUG(newNode);

	if (newNode == NULL) {
		fprintf(stderr, "Failed to allocate memory. File: %s Line %d\n", __FILE__, __LINE__);
		exit(MALLOC_ERROR);
	}

	newNode->ElementPtr = DataPtr;
	
	//Check size of list
	if(ListPtr->NumElements == 0){
		// If this is the first element, insert as both front and back, set
		// next and prev to null
		newNode->Next = NULL;
		newNode->Previous = NULL;
		ListPtr->FrontPtr = newNode;
		ListPtr->BackPtr = newNode;
	}
	else {
		// If not the first element, set previous to current backptr, and next to
		// NULL, update current backptr's next

		newNode->Next = NULL;
		ListPtr->BackPtr->Next = newNode;
		newNode->Previous = ListPtr->BackPtr;

		//Finally, update backptr
		ListPtr->BackPtr = newNode;
	}
	// increment list size
	ListPtr->NumElements++;
}

/******************************************************************************
 Removes a node from the front of the list and returns a pointer to the node
 data removed. On empty lists should return a NULL pointer.
 Note: This will destroy the node but not the associated ElementStruct data element.
  
 Where: LinkedLists *ListPtr    - Pointer to the linked list to remove from
 Returns: Pointer to the node removed or NULL for none
 Errors: none
******************************************************************************/
ElementStructs *RemoveFromFrontOfLinkedList(LinkedLists *ListPtr) {
	//Check if list is empty
	if(ListPtr->NumElements == 0) {
		//return null if empty
		return NULL;
	}
	else {
		//Save front ptr
		LinkedListNodes *frontNode = ListPtr->FrontPtr;
		ElementStructs *data = frontNode->ElementPtr;

		//Remove front ptr
		if(ListPtr->FrontPtr == ListPtr->BackPtr) {
			FREE_DEBUG(frontNode);
			free(frontNode);

			ListPtr->FrontPtr = NULL;
			ListPtr->BackPtr = NULL;
		}
		else {
			ListPtr->FrontPtr = frontNode->Next;
			if(ListPtr->FrontPtr != NULL) {
				ListPtr->FrontPtr->Previous = NULL;
			}
		
			//free node (not data)
			FREE_DEBUG(frontNode);
			free(frontNode);
		}
		//Update list size
		ListPtr->NumElements--;
		
		return data;
	}
}

/******************************************************************************
 Removes a node from the back of the list and returns a pointer to the node
 data removed. On empty lists should return a NULL pointer.
 Note: This will destroy the node but not the associated ElementStruct data element.
  
 Where: LinkedLists *ListPtr    - Pointer to the linked list to remove from
 Returns: Pointer to the node removed or NULL for none
 Errors: none
******************************************************************************/
ElementStructs *RemoveFromBackOfLinkedList(LinkedLists *ListPtr) {
	//Check if list is empty
	if(ListPtr->NumElements == 0) {
		return NULL;
	}
	else {
		//Save backptr
		LinkedListNodes *backNode = ListPtr->BackPtr;
		ElementStructs *data = backNode->ElementPtr;

		//Remove back ptr
		if(ListPtr->FrontPtr == ListPtr->BackPtr) {
			FREE_DEBUG(backNode);
			free(backNode);

			ListPtr->FrontPtr = NULL;
			ListPtr->BackPtr = NULL;
		}
		else {
			ListPtr->BackPtr = backNode->Previous;
			if(ListPtr->BackPtr != NULL) {
				ListPtr->BackPtr->Next = NULL;
			}

			//Free node (not data)
			FREE_DEBUG(backNode);
			free(backNode);
		}

		//Update list size
		ListPtr->NumElements--;

		return data;
	}
}


/******************************************************************************
 De-allocates the linked list and resets the struct fields (in the header) 
 to indicate that the list is empty.

 Where: LinkedLists *ListPtr    - Pointer to the linked list to destroy
 Returns: nothing
 Errors: none
******************************************************************************/
void DestroyLinkedList(LinkedLists *ListPtr) {
	// Check if list has nodes 
	if(ListPtr->NumElements > 0) {
		LinkedListNodes *cursor = ListPtr->FrontPtr;
		//Loop through list
		while(cursor != NULL) {
			//Save next node
			LinkedListNodes *next = cursor->Next;
			
			//Free the data
			FREE_DEBUG(cursor->ElementPtr);
			free(cursor->ElementPtr);

			//Free the node
			FREE_DEBUG(cursor);
			free(cursor);
			
			//Set next cursor
			cursor = next;
		}
		
		// Reset List stats and ptrs
		ListPtr->NumElements = 0;
		ListPtr->FrontPtr = NULL;
		ListPtr->BackPtr = NULL;
	}
}


/******************************************************************************
 Searches the linked list for a provided word. If found, returns the pointer
 to the element struct. If not found, returns a NULL pointer
 
 Where: LinkedLists *ListPtr - Pointer to the linked list to search
        char *String         - Pointer to the string to search
 Returns: Pointer to the element if found, NULL otherwise
 Errors: none
 * ***************************************************************************/
ElementStructs *SearchList(LinkedLists *ListPtr, char *String) {
	if(ListPtr->NumElements > 0) {
		LinkedListNodes *cursor = ListPtr->FrontPtr;
		while (cursor != NULL) {
			//Check for string
			if(strcmp(cursor->ElementPtr->buffer, String) == 0){
				return cursor->ElementPtr;
			}
			//Advance cursor
			cursor = cursor->Next;
		}
	}
	return NULL;
}




