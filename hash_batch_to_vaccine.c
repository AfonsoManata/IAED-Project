/**
 * Instituto Superior Técnico
 * Projeto IAED - Vaccine Managment System
 * @file hash_batch_to_vaccine.c
 * Afonso Manata ist 1113406
 */

#include "hash_batch_to_vaccine.h"

// ------------------------------------------------------------------------- //
// ---------------------------- Hash Table Batch --------------------------- //
// ------------------------------------------------------------------------- //


/** Function to create a node in the hash table
 * @param batch	Key of the hash table batch of a vaccine
 * @param vac	Value of the hash table : vaccine
 * @return	The new node of the hash table
 */ 

hashNode* createHashNode(char *batch, Vaccine vac){

	// Allocating memory to an node of the hash table 
	struct hashNode *new = (struct hashNode*)malloc(sizeof(struct hashNode));
	
	// If the system dont have memory to allocate 
	if(!new) no_memory();

	new->batch = strdup(batch); // Allocating memory and copying the batch
	new->vac = vac; // Copying the vaccine 
	new->next = NULL; // Pointer to next starts null 

	return new;
}


/** Function to init the hash table
 * @param ba_vac  An pointer to our hash table 
 * @param size  Size of the hash table
 */ 

void init_Hash(HashMap *ba_vac, int size){	

	ba_vac->size = size; // Copying the size 
	
	// Allocating memory to the hash table
	ba_vac->arr = (hashNode**)malloc(sizeof(hashNode*) * size);
	
	// If the system dont have memory to allocate 
	if(!ba_vac->arr) no_memory();

	for (int i = 0; i < size; i++) {
        ba_vac->arr[i] = NULL; // Setting all to NULL  
    }
    
	ba_vac->num_el = 0; // In the beginning num of elements is 0
}


/** Hash Function
 * @param ba_vac  An pointer to our hash table 
 * @param batch  Our key : batch of the vaccine
 */ 

int hashFunc(HashMap *ba_vac, char *batch){
	
	int sum = 0, factor = 31; // Factor is a prime number to mitigate colisions
	int len = strlen(batch);
	
	// Calculates the hash of each key 
	for (int i=0; i < len; i++)
		sum = (sum * factor + batch[i]) % ba_vac->size;
	
	return sum;
}


/** Hash Function
 * @param ba_vac  An pointer to our hash table 
 * @param batch  Our key : batch of the vaccine
 */ 

void insert_Hash(HashMap *ba_vac, char *batch, Vaccine vac){
	
	int index = hashFunc(ba_vac, batch); // Hash of that batch 

	hashNode *new = createHashNode(batch,vac); // Mem for new member

	if (ba_vac->arr[index] == NULL) ba_vac->arr[index] = new;
	
	// In case of colison put in the linked list in that index 
	else {
		new->next = ba_vac->arr[index];
		ba_vac->arr[index] = new;
	}

	ba_vac->num_el++;
	return;
}


/** Function to search in the hash table
 * @param ba_vac  An pointer to our hash table 
 * @param batch  Our key : batch of the vaccine
 * @return vac  Vaccine that we wanted 
 */ 

Vaccine hash_find(HashMap *ba_vac, char *batch){
	
	int index = hashFunc(ba_vac, batch);
	Vaccine vac2;
	vac2.expiration.year = 0;

	if (ba_vac->arr[index] == NULL)	return vac2; // No elements here

	hashNode *current = ba_vac->arr[index]; // First element in that index

	while (current!=NULL) {
		// If the batch is what we want we return 
		if (strcmp(current->batch, batch) == 0) {
            return current->vac;
        }

		current = current->next;
	}
	
	return vac2;
}


/** Function to delete a element in the hash table
 * @param ba_vac  An pointer to our hash table 
 * @param batch  Our key : batch of the vaccine
 */ 

void hash_delete(HashMap *ba_vac, char *batch){
	int index = hashFunc(ba_vac, batch);
	hashNode *current = ba_vac->arr[index];
	hashNode *prev = NULL;
	
    while (current!=NULL) {

		if (strcmp(current->batch, batch) == 0){
			if (current->vac.num_doses_used > 0) {
                // If there are doses used we just set the available to 0
                current->vac.num_doses_available = 0;
                return;
            }

            // Otherwise we remove the element 
            if (prev) prev->next = current->next;
            else ba_vac->arr[index] = current->next;

            free(current->batch);
            free(current);
            ba_vac->num_el--;
            return;
        }
        // Going to the next element in that index of the hash table
		prev = current;
        current = current->next;
    }
}


/** Function to change the hash when a vaccine is used 
 * @param ba_vac  An pointer to our hash table 
 * @param batch  Our key : batch of the vaccine
 */ 

void hash_use_dose(HashMap *ba_vac, char *batch){
	int index = hashFunc(ba_vac, batch);
	hashNode *current = ba_vac->arr[index];

	while (current!=NULL) {
		// If that's the batch we are looking for 		
		if (strcmp(current->batch, batch) == 0) {
            current->vac.num_doses_used++;
            current->vac.num_doses_available--;
        }

		current = current->next;
	}
}


/** Verify If there is a duplicated batch 
  * @param *sys	The system that has all the info
  * @param batch	The input batch from the user
  * @return	1 if its not duplicated otherwise 0
*/

int verify_duplicate_batch(HashMap *ba_vac, char *batch) {

    int index = hashFunc(ba_vac, batch);
	
	if (ba_vac->arr[index] == NULL) {
        return 0; // No elements here
    }
	
	hashNode *current = ba_vac->arr[index];

	// Find the batch in the hash table
	while (current!=NULL) 
	{
		if (strcmp(current->batch, batch) == 0) {
            return 1;
        }
		current = current->next;
	}

	return 0;
}


/** Function to free the hash table
 * @param ba_vac  An pointer to our hash table 
 */ 

void freeBatchHash(HashMap *ba_vac){
	for(int i=0; i < ba_vac->size; i++){
		hashNode *current = ba_vac->arr[i];
		
		// Going to all elements and cleaning the memory
		while (current){
			hashNode *temp = current;
			current = current->next;
			free(temp->batch);
			free(temp);
		}
	}
	
	free(ba_vac->arr); // cleaning the array of the hash table
}

