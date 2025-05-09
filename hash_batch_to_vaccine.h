/**
 * Instituto Superior Técnico
 * Projeto IAED - Vaccine Managment System
 * @file hash_batch_to_vaccine.h 
 * Afonso Manata ist 1113406
 */

#ifndef HASH_BATCH_TO_VACCINE_H
#define HASH_BATCH_TO_VACCINE_H

#include "constants.h" // Include the constants
#include "vaccine.h"  // Include for Vaccine structure


// Structure of a node in the hash table
typedef struct hashNode {
    char *batch;    // Batch of the vaccine
    Vaccine vac;    // Vaccine object
    struct hashNode *next; // Pointer to the next node in case of collision
} hashNode;


// Structure for the hash table itself
typedef struct {
    int size;       // Size of the hash table
    hashNode **arr; // Array of hash nodes
    int num_el;     // Number of elements in the hash table
} HashMap;


/** Verify If there is a duplicated batch 
  * @param *sys	The system that has all the info
  * @param batch	The input batch from the user
  * @return	1 if its not duplicated otherwise 0
*/
int verify_duplicate_batch(HashMap *ba_vac, char *batch);


/** Function to create a node in the hash table
 * @param batch  Key of the hash table batch of a vaccine
 * @param vac    Value of the hash table : vaccine
 * @return       The new node of the hash table
 */
hashNode* createHashNode(char *batch, Vaccine vac);


/** Function to initialize the hash table
 * @param ba_vac  A pointer to our hash table
 * @param size    Size of the hash table
 */
void init_Hash(HashMap *ba_vac, int size);


/** Hash Function
 * @param ba_vac  A pointer to our hash table
 * @param batch   Our key : batch of the vaccine
 * @return        Index of the hash table
 */
int hashFunc(HashMap *ba_vac, char *batch);


/** Function to insert a vaccine into the hash table
 * @param ba_vac  A pointer to our hash table
 * @param batch   Key of the hash table batch of a vaccine
 * @param vac     Value of the hash table : vaccine
 */
void insert_Hash(HashMap *ba_vac, char *batch, Vaccine vac);


/** Function to search for a vaccine in the hash table
 * @param ba_vac  A pointer to our hash table
 * @param batch   Key of the hash table batch of a vaccine
 * @return        Vaccine corresponding to the batch, or a default vaccine 
 */
Vaccine hash_find(HashMap *ba_vac, char *batch);


/** Function to delete a vaccine element from the hash table
 * @param ba_vac  A pointer to our hash table
 * @param batch   Key of the hash table batch of a vaccine
 */
void hash_delete(HashMap *ba_vac, char *batch);


/** Function to update the hash table when a vaccine dose is used
 * @param ba_vac  A pointer to our hash table
 * @param batch   Key of the hash table batch of a vaccine
 */
void hash_use_dose(HashMap *ba_vac, char *batch);


/** Function to free the memory used by the hash table
 * @param ba_vac  A pointer to our hash table
 */
void freeBatchHash(HashMap *ba_vac);


#endif 
