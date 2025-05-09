/**
 * Instituto Superior Técnico
 * Projeto IAED - Vaccine Managment System
 * @file avl_tree_vaccines.h
 * Afonso Manata ist 1113406
 */


#ifndef AVL_TREE_VACCINES_H
#define AVL_TREE_VACCINES_H

#include "vaccine.h" 
#include "date.h"    
#include "constants.h"


/** Structure that defines a node of my AVL tree filled with vaccines */
struct Node {
    Vaccine vaccine;     // Vaccine data
    struct Node* left;   // Left subtree
    struct Node* right;  // Right subtree
    int height;          // Node height
};


/** Function to calculate the height of a node
 * @param node  Node of the tree
 * @return      The height of the node
 */
int getHeight(struct Node* node);


/** Function to calculate the balance factor of a node
 * @param node  Node of the tree
 * @return      The balance factor of a node
 */
int getBalanceFactor(struct Node* node);


/** To choose the older vaccine and verify the batch in case of a tie
 * @param v1    A vaccine
 * @param v2    A vaccine
 * @return      Returns a negative number if v2 is older, and vice-versa
 */
int compareVaccines(Vaccine v1, Vaccine v2);


/** Function to free all the memory used in the tree
 * @param node  Node of the tree
 */
void freeTree(struct Node* node);


/** Function to rotate a node to the right
 * @param y    Node of the tree
 * @return     The node after rotation
 */
struct Node* rightRotate(struct Node* y);


/** Function to rotate a node to the left
 * @param x    Node of the tree
 * @return     The node after rotation
 */
struct Node* leftRotate(struct Node* x);


/** Function to insert a vaccine
 * @param node    Node of the tree
 * @param vaccine A vaccine
 * @return        The node after the vaccine has been inserted
 */
struct Node* insert_vaccine(struct Node* node, Vaccine vaccine);


/** Function to find the smallest value (most to the left)
 * @param node  Node of the tree
 * @return      The node with the smallest value
 */
struct Node* minValueNode(struct Node* node);


/** Auxiliary function to remove a vaccine by its batch
 * @param node  Node of the tree
 * @return      The node after the vaccine has been removed
 */
struct Node* removeVaccine_aux(struct Node* root);


/** Function to remove a vaccine by its batch
 * @param node    Node of the tree
 * @param vaccine A vaccine
 * @param found   Pointer to verify if a vaccine with that batch exists
 * @return        The node after the vaccine has been removed
 */
struct Node* removeVaccine(struct Node* root, Vaccine vaccine, int* found);


/** Function to list the vaccines in the tree
 * @param root  Node of the tree
 */
void list_vaccine(struct Node* root);


/** Function to list vaccines by their name
 * @param root   Node of the tree
 * @param name   Name of the vaccine
 * @param lang   Language
 * @param found  Pointer to verify if there is a vaccine with that name
 */
void list_vaccine_name(struct Node* root, char* name, char lang, int* found);


#endif 
