//rpp25. Custom malloc() using worst-fit allocation

#include "mymalloc.h"
#include <stdio.h>
#include <unistd.h>

struct node {
  int size;
  int free;	
  struct node *next;
  struct node *prev;
  void *address;
};

struct node *start = NULL;
struct node *end = NULL;

void *my_worstfit_malloc(int requestSize){
	start = sbrk(sizeof(struct node) + requestSize);
	struct node *currentNode = start;
	struct node *worstNode = NULL;

	while(currentNode != NULL) {
        if (currentNode->free == 1  && (currentNode->size >= requestSize) && (worstNode == NULL || currentNode->size > worstNode->size)) 
        {
		/*if current node is free and larger than the requested size,
		and either the worst node is not yet designated 
		OR  current node is larger than the worst node*/
		
            worstNode = currentNode; //designates the current node as the worst node
            if (worstNode->size == requestSize)
            { 
            	break; 
            }
        }
        currentNode = currentNode->next; //move to the next node

    }
    if (worstNode == NULL)
    {
    //creates new Node if there's no current worst Node
    	struct node *brk_pointer = (struct node *)sbrk(0);
    	sbrk(sizeof(struct node) + requestSize);
    	struct node *newNode = brk_pointer;
    	newNode->address = sizeof(struct node) + brk_pointer;
    	newNode->size = requestSize;
    	newNode->free = 0;
    	currentNode = newNode;
    	end = currentNode;
    	
    } else{
    //if the worst node's size is equal to the request size, the whole node become's allocated
    	if (worstNode->size == requestSize){
    		worstNode->free = 0;
    	}	
    	
    	/*if the worst node is larger than the request size, create a new node of a size equal to
    	what is leftover by the allocation of the worst node.*/
    	else if(worstNode->size > requestSize){
    		struct node *brk_pointer = (struct node *)sbrk(0);
    		struct node *newNode = brk_pointer + requestSize;
    		newNode->size = worstNode->size - sizeof(struct node) - requestSize;
    		newNode->free = 1;
    		worstNode->size = requestSize;
    		worstNode->free = 0;
    	}
    }

}

void my_free(void *ptr){
	struct node *currentNode = start;
	struct node *prevNode;
	struct node *nextNode;
	
	//traverse the list, stop when you find the node called
	while(currentNode != NULL) {
		if (currentNode->address == ptr)
			break;
		currentNode = currentNode->next;
	}
	
	if(currentNode != NULL){
		currentNode->free = 1; //free up current node
		prevNode = currentNode->prev;
		nextNode = currentNode->next;
		if (prevNode->free == 1){ //if previous node is free
			//coalesce the free space
			prevNode->size = sizeof(struct node) + prevNode->size + currentNode->size;
			if (nextNode->free == 0){ //if the next node is full
				prevNode->next = nextNode; //point to next node
			}
			if (nextNode->free == 1){ //if next node is free
				//coalesce the free space
				prevNode->size = sizeof(struct node) + prevNode->size + nextNode->size;
				prevNode->next = nextNode->next;
			}
			
			//setup the new coalesced previous node as the current node
			currentNode = prevNode; 
			prevNode = currentNode->prev;
			nextNode = currentNode->next;

		}else if (prevNode->free == 0 && nextNode->free == 1){ //if previous node is full, but the next one is free
			//coalesce free space and set up the current node		
			currentNode->size = sizeof(struct node) + currentNode->size + nextNode->size;
			currentNode->next = nextNode->next;
			nextNode = currentNode->next;
			prevNode = currentNode->prev;
		}
		
		//once the node freeing is done, frees up data space
		if(prevNode == NULL && nextNode == NULL){
			sbrk(0 - (sizeof(struct node) + currentNode->size));
			start = NULL;
			end = NULL;

		}else if(nextNode == NULL){
			end = currentNode->prev;
			sbrk(0 - (sizeof(struct node) + currentNode->size));
		}
	}

 
}