/**
 * Instituto Superior Técnico
 * Projeto IAED - Vaccine Management System
 * @file list_inoculations.h
 * Afonso Manata ist 1113406
 */

#ifndef LIST_INOCULATIONS_H
#define LIST_INOCULATIONS_H

#include "constants.h"
#include "date.h"
#include "inoculation.h"


/** Node of the singly linked list with inoculations */
typedef struct InoculationNode {
    Inoculation inoc;               // Inoculation
    struct InoculationNode* next;   // Pointer to the next inoculation
} InoculationNode;


/** Linked List (inoculations) Structure */
typedef struct {
    InoculationNode* head;          // Head of the linked list of inoculations
    InoculationNode* tail;          // Tail of the linked list of inoculations
} InoculationList;


/**
 * Function to initiate the linked list of inoculations
 * @param list    Pointer to the linked list 
 */
void initInoculationList(InoculationList* list);


/**
 * Function to create a Node in the linked list
 * @param batch  Batch of the vaccine used
 * @param date   Date of the inoculation
 * @param user   User inoculated
 * @return       Newly created node of the linked list
 */
InoculationNode* createNode(const char* batch, Date date, const char* user);


/**
 * Function to insert an inoculation in the linked list
 * @param list   Pointer to the linked list of inoculations
 * @param batch  Batch of the vaccine used
 * @param date   Date of the inoculation
 * @param user   User inoculated
 */
void insertInoculation(InoculationList* list, const char* batch, Date date, 
		const char* user);


/**
 * Function to delete an inoculation by user, date, and batch
 * @param list   Pointer to the linked list of inoculations
 * @param user   User inoculated
 * @param date   Date of the inoculation
 * @param batch  Batch of the vaccine used
 */
void deleteInoc3(InoculationList* list, char* user, Date date, char* batch);


/**
 * Function to delete inoculations by user and date
 * @param list   Pointer to the linked list of inoculations
 * @param user   User inoculated
 * @param date   Date of the inoculation
 */
void deleteInoc2(InoculationList* list, char* user, Date date);


/**
 * Function to delete inoculations by user
 * @param list   Pointer to the linked list of inoculations
 * @param user   User inoculated
 */
void deleteInoc1(InoculationList* list, char* user);


/**
 * Function to print all inoculations in the linked list
 * @param list   Pointer to the linked list of inoculations
 */
void printInoculation(InoculationList* list);


/**
 * Function to print inoculations by the name of the user
 * @param list   Pointer to the linked list of inoculations
 * @param name   Name of the user
 */
void printInoculationUser(InoculationList* list, char* name);


/**
 * Function to free the memory used in the linked list
 * @param list   Pointer to the linked list of inoculations
 */
void freeInoculationList(InoculationList* list);


#endif 
