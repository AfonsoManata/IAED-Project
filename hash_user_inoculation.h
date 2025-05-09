/**
 * Instituto Superior Técnico
 * Projeto IAED - Vaccine Managment System
 * @file hash_user_inoculatio.h 
 * Afonso Manata ist 1113406
 */


#ifndef HASH_USER_INOCULATION_H
#define HASH_USER_INOCULATION_H

#include "inoculation.h" 
#include "list_inoculations.h"
#include "hash_batch_to_vaccine.h"
#include "date.h"        
#include "vaccine.h"
#include "constants.h"


/** Node in the hash table (each node represents a user) */
typedef struct User_Node {
    char *user;           // User's name
    InoculationList inocs; // Linked list of inoculations for each user
    struct User_Node *next; // Pointer to the next element (for collision)
} User_Node;


/** Hash table structure for inoculations by user */
typedef struct {
    User_Node *table[TABLE_SIZE]; // Hash table with inoculations by user
} User_HashTable;


/** Function to initialize the hash table for inoculations by user */
void init_Hash_user(User_HashTable *hashTable);


/** Hash function that turns the key (user's name) into an index */
unsigned int user_Hash_func(const char *name);


/** Auxiliary function to insert an inoculation into the hash table */
void insert_user_hash_aux(User_HashTable *hashTable, char *user, Date date,
                          char *batch, unsigned int index);


/** Function to insert an inoculation into the hash table */
void insert_user_hash(User_HashTable *hashTable, char *user, Date date,
                      char *batch);


/** Function to delete an inoc from the hash table using date and batch */
void delete_user_hash3(User_HashTable *hashTable, const char *user, Date date,
                       char *batch);


/** Function to delete all inoculations of a user for a specific date */
void delete_user_hash2(User_HashTable *hashTable, const char *user, Date date);


/** Function to delete all inoculations of a user */
void delete_user_hash1(User_HashTable *hashTable, const char *user);


/** Function to check if a user exists and has at least one inoculation */
int find_user(User_HashTable *hashTable, const char *user);


/** Function to validate if a user was vaccinated with batch and date */
int validate_inoc_hash(HashMap *batch, User_HashTable *hashTable,
                       const char *user, const char *vac_name, Date date);


/** Function to free the memory allocated for the hash table and its lists */
void free_user_hash(User_HashTable *hashTable);


#endif 
