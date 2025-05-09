/**
 * Instituto Superior Técnico
 * Projeto IAED - Vaccine Managment System
 * @file list_inoculations.c 
 * Afonso Manata ist 1113406
 */

#include "list_inoculations.h"

// ------------------------------------------------------------------------- //
// ------------------------ Linked List Inoculations ----------------------- //
// ------------------------------------------------------------------------- //


/** Function to initiate the linked list of inoculations
 * @param list		Pointer to the linked list 
 */  

void initInoculationList(InoculationList* list) {
    // Starting the head and the tail as a NULL value
	list->head = NULL;
    list->tail = NULL;
}


/** Function to create a Node in the linke list 
 * @param batch		Batch of the vaccine used 
 * @param date		Date of the inoculation
 * @param user		User inoculatted 
 */  

InoculationNode* createNode(const char* batch, Date date, const char* user) {

	// Allocating memory to the inoculation node
    InoculationNode* newNode=(InoculationNode*)malloc(sizeof(InoculationNode));
    
	// If the system dont have memory to allocate 
	if(!newNode) no_memory();

	// Copy the info to the linked list node
    newNode->inoc.user = (char*)malloc(strlen(user) + 1);
 
	// If the system dont have memory to allocate 
	if(!newNode->inoc.user) no_memory();

	strcpy(newNode->inoc.user, user);

    strncpy(newNode->inoc.batch, batch, MAX_HEX_DIG_LOTE);
    newNode->inoc.batch[MAX_HEX_DIG_LOTE - 1] = '\0';  // String notation

    newNode->inoc.date = date;
    newNode->next = NULL;
    
    return newNode;
}


/** Function to insert an inoculation in our linked list 
 * @param list  A pointer to our linked list with Inoculations 
 * @param batch		Batch of the vaccine used 
 * @param date		Date of the inoculation
 * @param user		User inoculatted 
 */ 

void insertInoculation(InoculationList* list, const char* batch, 
	Date date, const char* user) {

    // Creating the New Node
	InoculationNode* newNode = createNode(batch, date, user);
    if (!newNode) return;

	// Putting the node in its place 
    if (list->tail == NULL) {
        list->head = newNode;
        list->tail = newNode;
    } 
	else {
        list->tail->next = newNode;
        list->tail = newNode;
    }
}


/** Function to delete an inoculation we receive 3 infos about the inoc 
 * @param list  A pointer to our linked list with Inoculations 
 * @param user		User inoculatted 
 * @param date		Date of the inoculation
 * @param batch		Batch of the vaccine used 
 */ 

void deleteInoc3(InoculationList *list,char *user,Date d1,char *batch) {
    InoculationNode* current = list->head;
    InoculationNode* prev = NULL;

    while (current != NULL) {
       
		// Verifies if is the inoc we are looking for 
		if (strcmp(current->inoc.batch, batch) == 0 && 
			strcmp(current->inoc.user, user) == 0 && 
			current->inoc.date.day == d1.day &&
			current->inoc.date.month == d1.month &&
			current->inoc.date.year == d1.year)
		{
            
			// If its the first element
			if (prev == NULL) 
			{
                list->head = current->next;
                if (list->head == NULL) list->tail = NULL;
            }
			
			else 
			{
                prev->next = current->next;
                if (prev->next == NULL) list->tail = prev;
            }
			
			// Free the memory 
            free(current->inoc.user);
			free(current);
            printf("1\n"); // We only remove 1 inoculation 
			return;
        }
        prev = current;
        current = current->next;
    }
}


/** Function to delete inoculations by the user and the date  
 * @param list  A pointer to our linked list with Inoculations 
 * @param user		User inoculatted 
 * @param date		Date of the inoculation 
 */ 

void deleteInoc2(InoculationList *list, char *user, Date d1) {
    InoculationNode *current = list->head;
    InoculationNode *prev = NULL;
	int count = 0;

    while (current != NULL) 
	{
        // Verifies if is the user and date that we want
		if (strcmp(current->inoc.user, user) == 0 && 
            current->inoc.date.day == d1.day &&
            current->inoc.date.month == d1.month &&
            current->inoc.date.year == d1.year) {
            
			InoculationNode *toDelete = current;
            current = current->next;
			count++;

            // If its the first element
			if (prev == NULL) {
                list->head = current;
                if (list->head == NULL) list->tail = NULL;
            } else {
                prev->next = current;
                if (current == NULL) list->tail = prev;
            }
        	
			// Free the memory
			free(toDelete->inoc.user);
			free(toDelete);

        } else {
            prev = current;
            current = current->next;
        }
    }

	printf("%d\n", count); // Num of eliminated doses
}


/** Function to delete inoculations by the user  
* @param list  A pointer to our linked list with Inoculations 
* @param user		User inoculatted 
*/ 

void deleteInoc1(InoculationList *list, char *user) {
    
	InoculationNode *current = list->head;
    InoculationNode *prev = NULL;
	int count = 0;

    while (current != NULL) 
	{
        // Find the user that we want
		if (strcmp(current->inoc.user, user) == 0)	{
            InoculationNode *toDelete = current;
            current = current->next;
			count++;
			
			// If is the first element
            if (prev == NULL){
                list->head = current;
                if (list->head == NULL) list->tail = NULL;
            } 
			else {
                prev->next = current;
                if (current == NULL) list->tail = prev;
            }
			
			// Free the memory
			free(toDelete->inoc.user);
            free(toDelete);
        
		} else {
            prev = current;
            current = current->next;
        }
    }

	printf("%d\n", count); // Num of eliminated doses
}


/** Function to print all Inoculations 
 * @param list  A pointer to our linked list with Inoculations 
 */ 

void printInoculation(InoculationList* list) {
    
	InoculationNode* current = list->head;

	// Print all the inoculations in the list
	while (current != NULL) {
        printf("%s %s %02d-%02d-%d\n", 
			current->inoc.user,
            current->inoc.batch,
            current->inoc.date.day,
            current->inoc.date.month,
            current->inoc.date.year);
        current = current->next;
    }
}


/** Function to print Inoculations by name of the user
 * @param list  A pointer to our linked list with Inoculations 
 * @param name  Name of the user
 */ 

void printInoculationUser(InoculationList *list, char *name){
	
	InoculationNode* current = list->head;
    
	while (current != NULL) {
        // If the user is what we want we print
		if (strcmp(current->inoc.user, name) == 0){
			printf("%s %s %02d-%02d-%d\n", 
				current->inoc.user,
				current->inoc.batch,
				current->inoc.date.day,
				current->inoc.date.month,
				current->inoc.date.year);
		}	
        current = current->next;
    }
}


/** Function to free the memory used in the linked list 
 * @param list  A pointer to our linked list with Inoculations 
 */ 

void freeInoculationList(InoculationList *list) {
    
	InoculationNode *current = list->head;
    InoculationNode *next;

	// Iterate to all the elements and free the memory
    while (current != NULL) {
        next = current->next;  
        free(current->inoc.user);
		free(current);         
        current = next;        
    }
	
	// Put the head and tail to NULL
    list->head = NULL;
    list->tail = NULL;
}

