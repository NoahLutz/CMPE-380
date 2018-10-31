#ifndef _LINKED_LISTS_H_
#define _LINKED_LISTS_H_
/******************************************************************************
* Linked Lists Module public interface - LinkedLists.n
* Revised: 2/27/2015
* 12/24/2017 - R. Repka     Removed AddToFrontOfLinkedList()
******************************************************************************/

/******************************************************************************
* Modify structure ElementStructs to suit your application
******************************************************************************/
typedef struct {
	int number;
	char buffer[256];
} ElementStructs;

/**************  Nothing else in the module needs to be modified *************/

/******************************************************************************
* struct for linked list nodes, contains user information and link pointers.
* The "ElementStructs" typemark must be defined based on specific needs of the
* application.
******************************************************************************/
typedef struct LinkedListNodes {
	/* The user information field, , pointer to the actual data */
	ElementStructs *ElementPtr;
	/* Link pointers to other nodes */
	struct LinkedListNodes *Next;
	struct LinkedListNodes *Previous;
} LinkedListNodes;

/*********************************************************************************
* struct for linked list "header", keeps a counter of the size of the linked list
**********************************************************************************/
typedef struct LinkedLists {
	/* Number of elements in the list */
	int NumElements;
	/* Pointer to the front of the list of elements, possibly NULL */
	struct LinkedListNodes *FrontPtr;
	/* Pointer to the end of the list of elements, possibly NULL */
	struct LinkedListNodes *BackPtr;
} LinkedLists;

/******************************************************************************
 Function prototypes 
******************************************************************************/
void InitLinkedList(LinkedLists *ListPtr);
void AddToBackOfLinkedList(LinkedLists *ListPtr, ElementStructs *DataPtr);
ElementStructs *RemoveFromFrontOfLinkedList(LinkedLists *ListPtr);
ElementStructs *RemoveFromBackOfLinkedList(LinkedLists *ListPtr);
void DestroyLinkedList(LinkedLists *ListPtr);
ElementStructs* SearchList(LinkedLists *ListPtr, char *String);

#endif /* _LINKED_LISTS_H_ */
