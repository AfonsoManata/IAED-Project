/**
 * Instituto Superior Técnico
 * Projeto IAED - Vaccine Managment System
 * @file avl_tree_vaccines.c
 * Afonso Manata ist 1113406
 */

#include "avl_tree_vaccines.h"


// ------------------------------------------------------------------------- //
// --------------------------- AVL Tree Vaccines --------------------------- //
// ------------------------------------------------------------------------- //


/** Function to calculate the height of a node
 * @param node	Node of the tree
 * @return	The height of the node
 */ 

int getHeight(struct Node* node) {
    if (node == NULL) return 0;
    return node->height; // Size of the branch
}


/** Function to calculate the balance factor of a node
 * @param node	Node of the tree
 * @return	The balance factor of a node 
 */ 

int getBalanceFactor(struct Node* node) {
    if (node == NULL) return 0; // This will alow  us to make the tree balanced
    return getHeight(node->left) - getHeight(node->right);
}


/** To choose the older vaccine and verify the batch in case of draw
 * @param v1 	A vaccine 
 * @param v2 	A vaccine 
 * @return	Returns a negative number if d2 is older and vice-versa 
 */ 

int compareVaccines(Vaccine v1, Vaccine v2) {
    
	// First by expiration date
    int dateComparison = compareDates(v1.expiration, v2.expiration);
    if (dateComparison != 0) return dateComparison;

    // If dates are equal, compare batches
    return strcmp(v1.batch, v2.batch);
}


/** Function to free all the memory used in the tree
 * @param root	Node of the tree
 */

void freeTree(struct Node* node) {
    if (node == NULL) return;

	// Recursion to free all the nodes
    freeTree(node->left);  
    freeTree(node->right); 

    free(node);
}


/** Function to rotate a node to the right
 * @param y	Node of the tree
 * @return	The node after rotation
 */ 

struct Node* rightRotate(struct Node* y) {

	// Declarations 
	struct Node* x = y->left;
    struct Node* T2 = x->right;

    // Rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}


/** Function to rotate a node to the left 
 * @param y	Node of the tree
 * @return	The node after rotation
 */ 

struct Node* leftRotate(struct Node* x) {

	// Declarations
	struct Node* y = x->right;
    struct Node* T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}


/** Function to insert a vaccine 
 * @param node	Node of the tree
 * @param vaccine	A vaccine
 * @return	The node after the vaccine being inputed
 */ 

struct Node* insert_vaccine(struct Node* node, Vaccine vaccine) {
    
	// If the tree is empty we create a Node to the vaccine
    if (node == NULL) {
        
		struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        
		// In case there is no memory in the system 
		if (!newNode) no_memory();

		newNode->vaccine = vaccine;
        newNode->left = NULL;
        newNode->right = NULL;
        newNode->height = 1; 
		return newNode;
    }
    int cmp = compareVaccines(vaccine, node->vaccine);

	// Recursion to insert the vaccine
    if (cmp < 0) node->left = insert_vaccine(node->left, vaccine);
    else if (cmp > 0)node->right = insert_vaccine(node->right, vaccine);
    else return node;  // To prevent duplicated vaccines

    // Update node height and balance
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
	int balance = getBalanceFactor(node);

	// This tree is balanced so this are the rotations to put everything fine 
	// Left rotation case 
    if (balance > 1 && compareVaccines(vaccine, node->left->vaccine) < 0)
        return rightRotate(node);

    // Right rotation case
    if (balance < -1 && compareVaccines(vaccine, node->right->vaccine) > 0)
        return leftRotate(node);

    // Left-right rotation case
    if (balance > 1 && compareVaccines(vaccine, node->left->vaccine) > 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right-left rotation case
    if (balance < -1 && compareVaccines(vaccine, node->right->vaccine) < 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
	
    return node;
}


/** Function to find the smaller value (more in the left) 
 * @param node	Node of the tree
 * @return	The node after the vaccine being inputed
 */ 

struct Node* minValueNode(struct Node* node) {
    
	struct Node* current = node;
    
    // Recursion to find the min value (Node in the low left corner)
	while (current->left != NULL)
        current = current->left;
    
    return current;
}


/** Auxiliar Function to remove a vaccine by it's batch
 * @param node	Node of the tree
 * @return	The node after the vaccine being inputed
 */ 

struct Node* removeVaccine_aux(struct Node* root) {

	// If the tree is empty, returns NULL
    if (root == NULL)
        return root;

    // Updating the node height
    root->height = 1 + max(getHeight(root->left), getHeight(root->right));
    int balance = getBalanceFactor(root);

    // This tree is balanced so these are the rotations to put everything fine

    if (balance > 1 && getBalanceFactor(root->left) >= 0)
        return rightRotate(root);

    // Left Right Case
    if (balance > 1 && getBalanceFactor(root->left) < 0)
    {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Right Right Case
    if (balance < -1 && getBalanceFactor(root->right) <= 0)
        return leftRotate(root);

    // Right Left Case
    if (balance < -1 && getBalanceFactor(root->right) > 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}


/** Function to remove a vaccine by it's batch
 * @param node	Node of the tree
 * @param vaccine	A vaccine
 * @param found	 To verify if a vaccine with that batch exists
 * @return	The node after the vaccine being inputed
 */ 

struct Node* removeVaccine(struct Node* root, Vaccine vaccine, int* found) {
    
	if (root == NULL) return root;
    
	// Recursion to find the vaccine we want to remove
    if (compareVaccines(vaccine, root->vaccine) < 0)
		root->left = removeVaccine(root->left, vaccine, found);
    else if (compareVaccines(vaccine, root->vaccine) > 0)
		root->right = removeVaccine(root->right, vaccine, found);
    else {
        *found = 1; // To know that we found the vaccine

		// If the vaccine had already ben used just put availability to 0
        if(root->vaccine.num_doses_used > 0)root->vaccine.num_doses_available=0;

		// If the node don't have 2 childrens
        else if (root->left == NULL || root->right == NULL) {
            struct Node* temp;

            if (root->left != NULL) temp = root->left;
            else if (root->right != NULL) temp = root->right;
            else temp = NULL;

            // If he doesnt have childrens
            if (temp == NULL) root = NULL;
			
			else {   // If the node as one children
                root->vaccine = temp->vaccine;
                root->left = temp->left;
                root->right = temp->right;
                root->height = temp->height;
            }
            temp = NULL;

        }else {
            
			// Two Childrens tecnhique -> switch to the min value
            struct Node* temp = minValueNode(root->right);
            
			//	Switch and remove teh sucessor
            root->vaccine = temp->vaccine;
            root->right = removeVaccine(root->right, temp->vaccine, found);
        }
    }
    return removeVaccine_aux(root);
}


/** Function to list the vaccines 
 * @param root	Node of the tree
 */ 

void list_vaccine(struct Node* root) {
	
	// List all the vaccines
	if (root != NULL) {
    
		list_vaccine(root->left);

        // Flag to know if the vaccine have been bad removed
		if (root->vaccine.expiration.year != 0){
			
			printf("%s %s %02d-%02d-%02d %d %d\n",
					root->vaccine.name,
					root->vaccine.batch,
					root->vaccine.expiration.day,
					root->vaccine.expiration.month,
					root->vaccine.expiration.year,
					root->vaccine.num_doses_available,
					root->vaccine.num_doses_used);
		}
        list_vaccine(root->right);
    }
}


/** Function to list the vaccines by its name  
 * @param root	Node of the tree
 * @param name	Name of the vaccine
 * @param lang	language
 * @param found	pointer to verify if there is a vaccine with that name
 */ 

void list_vaccine_name(struct Node* root, char* name, char lang, int* found) {
    if (root != NULL) 
	{
		list_vaccine_name(root->left, name, lang, found); 
        
        // If the vaccine maches the name we print the names 
        if (strcmp(root->vaccine.name, name) == 0) {
        	printf("%s %s %02d-%02d-%02d %d %d\n",
				root->vaccine.name,
				root->vaccine.batch,
				root->vaccine.expiration.day,
				root->vaccine.expiration.month,
				root->vaccine.expiration.year,
				root->vaccine.num_doses_available,
				root->vaccine.num_doses_used);
        	*found = 1; // To tell the program we found at least one  
		}

        list_vaccine_name(root->right, name, lang, found); 
		
	}
}


