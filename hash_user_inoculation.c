/**
 * Instituto Superior Técnico
 * Projeto IAED - Vaccine Managment System
 * @file hash_user_inoculation.c
 * Afonso Manata ist 1113406
 */

#include "hash_user_inoculation.h"

// ------------------------------------------------------------------------- //
// ---------------------------- Hash Table User ---------------------------- //
// ------------------------------------------------------------------------- //


/** Function to initiate the Hash Table of inocs by user 
 * @param list		Pointer to the hash table 
 */  

void init_Hash_user(User_HashTable *hashTable) {
    // Setting all values to NULL in the hash Table
	for (int i = 0; i < TABLE_SIZE; i++) hashTable->table[i] = NULL;
}


/** Hash function	turns the key into an index 
 * @param name		Name of the user -> key to our Hash Table 
 */  

unsigned int user_Hash_func(const char *name)
{
	unsigned int sum = 0, factor = 31;

	while (*name)
	{
		sum = (sum * factor) + *name++; // 31 because is prime (less colisions)
	}

	return sum % TABLE_SIZE; // returns the index in the hash table
}


/** Aux function to insert an inoculation into the hash table
 * @param hashTable  Pointer to the hash table
 * @param user       Name of the user
 * @param batch      Batch ID of the inoculation
 * @param date       Date of the inoculation
 */

void insert_user_hash_aux(User_HashTable *hashTable, char *user, Date date, 
	char *batch, unsigned int index) {
    
	// Allocating memory to the new user hashNode
	User_Node *newUser = (User_Node *)malloc(sizeof(User_Node));

	// If the system dont have memory to allocate 
	if(!newUser) no_memory();

	// Allocating memory to the name of the user
    newUser->user = (char *)malloc(strlen(user) + 1);
	
	// If the system dont have memory to allocate 
	if(!newUser->user) no_memory();

    strcpy(newUser->user, user);
    newUser->inocs.head = newUser->inocs.tail = NULL;

    // Allocating memory for the inoculation node
    InoculationNode *newNode=(InoculationNode *)malloc(sizeof(InoculationNode));

	// If the system dont have memory to allocate 
	if(!newNode) no_memory();

	// Copying all the info to the the Hash Node
    strncpy(newNode->inoc.batch, batch, MAX_HEX_DIG_LOTE);
    newNode->inoc.batch[MAX_HEX_DIG_LOTE] = '\0';
    newNode->inoc.date = date;
    newNode->inoc.user = (char *)malloc(strlen(user) + 1);
 
	// If the system dont have memory to allocate 
	if(!newNode->inoc.user) no_memory();

	strcpy(newNode->inoc.user, user);
    newNode->next = NULL;

	// Inserting into the linked list for colisions
    newUser->inocs.head = newUser->inocs.tail = newNode;
    newUser->next = hashTable->table[index];  
    hashTable->table[index] = newUser;
}


/** Function to insert an inoculation into the hash table
 * @param hashTable  Pointer to the hash table
 * @param user       Name of the user
 * @param batch      Batch ID of the inoculation
 * @param date       Date of the inoculation
 */

void insert_user_hash(User_HashTable *hashTable, char *user, Date date, 
	char *batch) {
    
	unsigned int index = user_Hash_func(user);
    User_Node *current = hashTable->table[index];

    // Check if user already exists in the hash table
    while (current) {
        
		if (strcmp(current->user, user) == 0) {
           
			// User found, insert inoculation in their list
            InoculationNode *newNode = (InoculationNode *)
				malloc(sizeof(InoculationNode));	
				
			// If the system dont have memory to allocate 
			if(!newNode) no_memory();
            
            // Copying all the stuf to the node
			strncpy(newNode->inoc.batch, batch, MAX_HEX_DIG_LOTE);
            newNode->inoc.batch[MAX_HEX_DIG_LOTE] = '\0';
            newNode->inoc.date = date;
            newNode->inoc.user = (char *)malloc(strlen(user) + 1);
			
			// If the system dont have memory to allocate 
			if(!newNode->inoc.user) no_memory();
            
			strcpy(newNode->inoc.user, user);
            newNode->next = NULL;

            // Insert in the linked list
            if (!current->inocs.head) {
                current->inocs.head = current->inocs.tail = newNode;
            } else {
                current->inocs.tail->next = newNode;
                current->inocs.tail = newNode;
            }
            return;
        }
        current = current->next;
    }

    // If the number does'nt already exists we create 
    insert_user_hash_aux(hashTable, user, date, batch, index);
}


/** Function to delete an inoculation from the hash table with 3 informations
 * @param hashTable  Pointer to the hash table
 * @param user       Name of the user
 * @param batch      Batch ID of the inoculation
 * @param date       Date of the inoculation
 */

void delete_user_hash3(User_HashTable *hashTable, const char *user, Date date,
	char *batch) {
    
	unsigned int index = user_Hash_func(user);
    User_Node *current = hashTable->table[index];

    while (current) {
        
		// Find the user
		if (strcmp(current->user, user) == 0) {
            InoculationNode *prevInoc = NULL;
            InoculationNode *currentInoc = current->inocs.head;
            
			while (currentInoc) {
                
				// Check if the inoculation matches the provided batch and date
                if (strcmp(currentInoc->inoc.batch, batch) == 0 &&
                    currentInoc->inoc.date.day == date.day &&
                    currentInoc->inoc.date.month == date.month &&
                    currentInoc->inoc.date.year == date.year) {
                    
					// Inoculation found, remove it
                    if (prevInoc) prevInoc->next = currentInoc->next;  
                	else current->inocs.head = currentInoc->next;

                    // Update the tail pointer if is the last element
                    if (currentInoc->next == NULL)current->inocs.tail=prevInoc;

                    free(currentInoc->inoc.user);  // Free the user
                    free(currentInoc);  // Free the memory of the node
                    return;  
                }
                prevInoc = currentInoc;
                currentInoc = currentInoc->next;
            }
            return;
        }
        current = current->next;
    }
}


/** Function to delete all inoculations of a user for a specific date
 * @param hashTable  Pointer to the hash table
 * @param user       Name of the user
 * @param date       Date of the inoculation to be deleted
 */

void delete_user_hash2(User_HashTable *hashTable, const char *user, Date date){
    unsigned int index = user_Hash_func(user);
    User_Node *current = hashTable->table[index];

    while (current) {
        
		// Find the user
		if (strcmp(current->user, user) == 0) {
            InoculationNode *prevInoc = NULL;
            InoculationNode *currentInoc = current->inocs.head;
            
			while (currentInoc) {
                // Check if the inoculation matches the provided date
                
				if (currentInoc->inoc.date.day == date.day &&
                    currentInoc->inoc.date.month == date.month &&
                    currentInoc->inoc.date.year == date.year) {
                    
					if (prevInoc) prevInoc->next = currentInoc->next;  
                    else current->inocs.head = currentInoc->next;

                    // Update the tail pointer if is the last element
                    if (currentInoc->next == NULL)current->inocs.tail=prevInoc;

                    // Free the memory of the inoculation node
                    InoculationNode *temp = currentInoc;
                    currentInoc = currentInoc->next;
                    free(temp->inoc.user);  
                    free(temp);

                } else {
                    prevInoc = currentInoc;
                    currentInoc = currentInoc->next;
				}
            }
            return;  
        }
        current = current->next;
    }
}



/** Function to delete all inoculations of a user (all dates)
 * @param hashTable  Pointer to the hash table
 * @param user       Name of the user
 */

void delete_user_hash1(User_HashTable *hashTable, const char *user) {
    unsigned int index = user_Hash_func(user);
    User_Node *current = hashTable->table[index];

    // Enter the hash table to find the user
    while (current) {
        if (strcmp(current->user, user) == 0) {
            
			// User found, now delete all inoculations
            InoculationNode *currentInoc = current->inocs.head;

            // Delete all inoculations in the list for this user
            while (currentInoc) {
                InoculationNode *temp = currentInoc;
                currentInoc = currentInoc->next;
                free(temp->inoc.user);  // Free the user
                free(temp);  // Free memory of the inoculation
            }

            // Reset the head and tail pointers as the list is now empty
            current->inocs.head = current->inocs.tail = NULL;
            
            return;  
        }
        current = current->next;
    }
}


/** Function to find if a user exists and has at least one inoculation
 * @param hashTable  Pointer to the hash table
 * @param user       Name of the user
 * @return 1 if user exists and has at least one inoculation, 0 otherwise
 */

int find_user(User_HashTable *hashTable, const char *user) {
    unsigned int index = user_Hash_func(user);
    User_Node *current = hashTable->table[index];

    // Enter the hash table to find the user
    while (current) {
        
		if (strcmp(current->user, user) == 0) {
            
			// User found, check if they have any inoculations
            if (current->inocs.head != NULL) {
                return 1;  // User has at least one inoculation
            }
            return 0;  // User has no inoculations
        }
        current = current->next;
    }
    return 0;  
}


/** Function to check if a user was vaccinated with a specific batch on a date
 * @param hashTable  Pointer to the hash table
 * @param user       Name of the user
 * @param batch      Batch ID of the inoculation
 * @param date       Date of the inoculation
 * @return 1 if user was vaccinated with that batch on that date 0 otherwise
 */

int validate_inoc_hash(HashMap *batch, User_HashTable *hashTable,
	const char *user, const char *vac_name, Date date) {
    
	unsigned int index = user_Hash_func(user);
    User_Node *current = hashTable->table[index];

    // Enter the hash table to find the user
    
	while (current) {
        
		if (strcmp(current->user, user) == 0) {
            // User found, check their inoculations
            InoculationNode *currentInoc = current->inocs.head;
			
			while (currentInoc) {
        
				Vaccine vaccine = hash_find(batch, currentInoc->inoc.batch);

				// Check if the inoculation matches the batch and date
				if (vaccine.expiration.year != 0 && 
					strcmp(vaccine.name, vac_name) == 0 &&
                    currentInoc->inoc.date.day == date.day &&
                    currentInoc->inoc.date.month == date.month &&
                    currentInoc->inoc.date.year == date.year) {
                    return 0;  
                }
                currentInoc = currentInoc->next;
            }
            return 1;  
        }
        current = current->next;
    }
    return 1;  // User not found in the hash table
}


/** Function to free the memory allocated for the hash table and its lists
 * @param hashTable  Pointer to the hash table
 */

void free_user_hash(User_HashTable *hashTable) {
    
	for (int i = 0; i < TABLE_SIZE; i++) {
        User_Node *currentUser = hashTable->table[i];
        
        while (currentUser) {
            
			// Free all inoculations for the current user
            InoculationNode *currentInoc = currentUser->inocs.head;
            
			while (currentInoc) {
                InoculationNode *tempInoc = currentInoc;
                currentInoc = currentInoc->next;
                free(tempInoc->inoc.user);  // Free the user
                free(tempInoc);  // Free the inoculation node
            }

            // Free the user node and its dynamically allocated user name
            free(currentUser->user);  // Free the user name memory
            User_Node *tempUser = currentUser;
            currentUser = currentUser->next;
            free(tempUser);  // Free the temporary user node
        }
    }
}

