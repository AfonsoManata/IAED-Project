/**
 * Instituto Superior Técnico
 * Projeto IAED - Vaccine Managment System
 * @file main.c
 * Afonso Manata ist 1113406
 */


// -------------------------------- Includes ------------------------------- //

#include "constants.h"
#include "date.h"
#include "inoculation.h"
#include "vaccine.h"
#include "list_inoculations.h"
#include "avl_tree_vaccines.h"
#include "hash_user_inoculation.h"
#include "hash_batch_to_vaccine.h"


// ------------------------------ Struct Sys ------------------------------- //

typedef struct {
  	InoculationList *inoc; // Linked List with Inoculations
	User_HashTable *user_hash; // Hash table user to inoculations 
  	HashMap *ba_vac; // Hash table batch to vaccine
	struct Node* root;	// Pointer to the AVL tree with the vaccines
	Date date;	// Current date of the system 
  	int count;	// Num of vaccines in the system 
} Sys;


/*

	Command    | Action
    -----------|--------------------------------------------------------

    q          | Quit the program

    c          | Insert a new vaccine batch into the system

    l          | List all available vaccines

    a          | Apply a dose of vaccine to a user

    r          | Remove a vaccine from availability

    d          | Delete all application records of a vaccine

    u          | List all vaccine applications for a specific user

    t          | Advance the simulated system date


*/


// All the functions above change the info in the system 

// ------------------------------------------------------------------------- //
// ----------------------------- 't' command ------------------------------- //
// ------------------------------------------------------------------------- //


/** Handling the input with t command
  * @param *sys	The system that has the current date
  * @param *in	The input command from the user
  * @param *lang	The system language
  * @return	<day>-<month>-<year> of the new date or current date if no input
*/

void t_input(Sys *sys, char *in, char lang) {
  	
	Date date;

	// Just print the current system date if the input is only 't'  
	if (sscanf(in, "%*s %d-%d-%d", &date.day, &date.month, &date.year) < 1)
		printf("%02d-%02d-%02d\n",sys->date.day,sys->date.month,sys->date.year);
  
	// If the possible new date is more recent turn it into system date
	else {
		if (valid_date(sys->date, date)){
			printf("%02d-%02d-%02d\n", date.day, date.month, date.year);
			sys->date = date;		
		}
		else {
    		puts(lang == 'p' ? INVALIDDATE : INVALIDDATE_ING);
			return;    
		}
  	}
}


 
// ------------------------------------------------------------------------- //
// ----------------------------- Find Vaccine------------------------------- //
// ------------------------------------------------------------------------- //


/** Function to list the info of the vaccine used and use that vaccine
 * @param root	Node of the tree
 * @param name	Name of the vaccine
 * @param lang	language
 * @param found	pointer to verify if there is a vaccine with that name
 */ 

void find_vaccine(Sys *sys,struct Node* root,char* name,char lang, int* found,
	char *user){
    
	if (root != NULL) {
		
		if (*found == 0) find_vaccine(sys, root->left, name, lang, found, user); 
        
        // If the vaccine maches the name we print the names 
        if (*found==0 && strcmp(root->vaccine.name, name) == 0 && 
			valid_date(sys->date, root->vaccine.expiration) &&
			root->vaccine.num_doses_available > 0) {

			// Update the doses in the vaccine
			root->vaccine.num_doses_available--;
			root->vaccine.num_doses_used++;
			printf("%s\n", root->vaccine.batch);
        	
			*found = 1; // To tell the program we found at least one 
			
			// Changing all the structures that depend on the inoculation
			hash_use_dose(sys->ba_vac, root->vaccine.batch);
			insertInoculation(sys->inoc, root->vaccine.batch, sys->date, user);
			insert_user_hash(sys->user_hash,user,sys->date,root->vaccine.batch);
			return;
		}

        if (*found==0) find_vaccine(sys, root->right, name, lang, found, user); 
		
	}

	return;
}



// ------------------------------------------------------------------------- //
// ------------------------------ 'c' command ------------------------------ //
// ------------------------------------------------------------------------- //


/** Auxiliar to Handling the input with c command
  * @param *sys	The system that has all the info
  * @param *in	The input command from the user
  * @param lang	 The system language
  * @param *input_sec	Secondary variable to store the input
  * @param *batch	The batch we will verify
  * @param *name	The name of the vaccine 
  * @param *d1 		Expiration date of the vaccine
  * @param *n	Number of doses available 
  * @return	 1 if there is an error and 0 otherwise
*/ 

int c_input_verify1(Sys *sys, char *in, char lang, char *input_sec, 
	char *batch, char *name, Date *d1, int *n) {
	
	strcpy(input_sec, in); // Strtok modifies the input string

	// Verifies if the system had reached the maximum capacity of vaccines
	if (sys->count >= MAX_NUM_VAC){	
		puts(lang == 'p' ? TOOMANYVACCINES  : TOOMANYVACCINES_ING);
		return 1; 
	}
	
	char *word = strtok(input_sec, " ");
	word = strtok(NULL, " "); // Get batch code

	// Verifies if the size of the batch is invalid
	if (strlen(word) > MAX_HEX_DIG_LOTE){
		puts(lang == 'p' ? INVALIDBATCH : INVALIDBATCH_ING);
		return 1;
	}

	// Skip date and doses
	word = strtok(NULL, " ");
	word = strtok(NULL, " ");
	word = strtok(NULL, " ");

	// Verifies if the size of the vaccine name is invalid
	if (strlen(word) > MAX_BYT_VAC_NAME){
		puts(lang == 'p' ? INVALIDNAME : INVALIDNAME_ING);
		return 1;
	}

	// Get values
	sscanf(in, "%*s %s %d-%d-%d %d %s", batch, &d1->day, &d1->month,
	&d1->year, n, name);
	
	return 0;
}


/** Auxiliar to Handling the input with c command
  * @param *sys	The system that has all the info
  * @param *batch	The batch we will verify
  * @param *name	The name of the vaccine 
  * @param *d1 		Expiration date of the vaccine
  * @param *n	Number of doses available 
  * @param lang	 The system language
  * @return	 1 if there is an error and 0 otherwise
*/ 

int c_input_verify2(Sys *sys,char *batch,char *name,Date d1,int n,char lang) {

	// Verify If there are any batches like that 
	if (verify_duplicate_batch(sys->ba_vac, batch)){
		puts(lang == 'p' ? DUPLICATEBATCH : DUPLICATEBATCH_ING);
		return 1;
	}

	// Verify if this batch is valid
	if (!validate_batch(batch)){
		puts(lang == 'p' ? INVALIDBATCH : INVALIDBATCH_ING);
		return 1;
	}

	// Verify if the name is valid
	if (!validate_name(name)){
		puts(lang == 'p' ? INVALIDNAME : INVALIDNAME_ING);
		return 1;
	}	

	// Verify if the date is a valid date
	if (!valid_date(sys->date, d1)){
		puts(lang == 'p' ? INVALIDDATE : INVALIDDATE_ING);
		return 1;
	}

	// Verify if the number of doses is positve
	if (n < 1){	
		puts(lang == 'p' ? INVALIDQUANTITY  : INVALIDQUANTITY_ING);
		return 1;
	}

	return 0;
}


/**  Handling the input with c command
  * @param *sys	The system that has all the info
  * @param *in	The input command from the user
  * @param *lang	The system language
  * @return	<batch> of the new valid vacinne
*/ 

void c_input(Sys *sys, char *in, char lang) {
	Vaccine vac;
	Date d1;
	int n;
	char input_sec[MAX_CAR_PEREXEC], batch[21], name[51];

	// Verify all the conditions
	if (c_input_verify1(sys, in, lang, input_sec, batch, name, &d1, &n)) return;
	if (c_input_verify2(sys, batch, name, d1, n, lang)) return;

	// Copy the info
	strcpy(vac.batch, batch);
	vac.expiration = d1;
	vac.num_doses_available = n;
	vac.num_doses_used = 0;
	strcpy(vac.name, name);

	// Insert the vaccine
	sys->root = insert_vaccine(sys->root, vac);
	insert_Hash(sys->ba_vac, vac.batch, vac);
	printf("%s\n", batch);
	sys->count++;
}



// ------------------------------------------------------------------------- //
// ------------------------------ 'l' command ------------------------------ //
// ------------------------------------------------------------------------- //


/** Handling the input with l command
  * @param *sys	The system that has all the info
  * @param *in	The input command from the user
  * @param *lang	The system language
  * @return	Prints all the info about this vaccine like name, batch, date...
*/ 

void l_input(Sys *sys, char *in, char lang)
{
	char input_sec[MAX_CAR_PEREXEC];
	strcpy(input_sec, in); // Strtok modififes the input string

	// If the input is only 'l' we input all the vaccines
	if (in[0]=='l' && in[1]=='\n') 
	{
		list_vaccine(sys->root);
		return;
	}
	
	char *word = strtok(input_sec, " " );
	word = strtok(NULL, " \n" ); 
	
	// Finding the vaccine
	while (word != NULL) 
	{			
		int found = 0;
		list_vaccine_name(sys->root, word, lang, &found);
		
		// If the vaccine doesnt exist in our system
		if (!found){
			
			printf("%s: ", word);
			puts(lang == 'p' ? NOSUCHVACCINE  : NOSUCHVACCINE_ING);

		} 

		word = strtok(NULL, " \n" ); 
	}
}



// ------------------------------------------------------------------------- //
// ------------------------------ 'a' command ------------------------------ //
// ------------------------------------------------------------------------- //


/** Handling the input with a command
  * @param *sys	The system that has all the info
  * @param *in	The input command from the user
  * @param *lang	The system language
  * @return	Prints all the info about the vaccine that was used
*/ 

void a_input(Sys *sys, char *in, char lang){

	int found = 0;
	char vacName[MAX_BYT_VAC_NAME];
	unsigned long name_length;
  
	char *name = extractUserName(&name_length, &in);
	char *ptr = in;

	sscanf(ptr, "%s", vacName);

	// Verify if the inoculation is valid
	if (!validate_inoc_hash(sys->ba_vac,sys->user_hash,name,vacName,sys->date)){
	 	
		puts(lang == 'p' ? ALREADYVACCINATED : ALREADYVACCINATED_ING);
		free(name);
		return;
	}

	find_vaccine(sys, sys->root, vacName, lang, &found, name);
	
	// Verify if the vaccine if the vaccine is in the stock 
	if (!found) {
		puts(lang == 'p' ? NOSTOCK  : NOSTOCK_ING);
		free(name);
		return;
	}

	free(name);
}



// ------------------------------------------------------------------------- //
// ------------------------------ 'r' command ------------------------------ //
// ------------------------------------------------------------------------- //


/** Handling the input with r command
  * @param *sys	The system that has all the info
  * @param *in	The input command from the user
  * @param *lang	The system language
  * @return	Prints the number of doses already applied  
*/ 

void r_input(Sys *sys, char *in ,char lang)
{
	int found=0;
	char input_sec[MAX_CAR_PEREXEC], batch[21];
	
	strcpy(input_sec, in); // Strtok modififes the input string

	char *word = strtok(input_sec, " " );
	word = strtok(NULL, " \n" ); 

	// Invalid batch note: strlen counts the bytes so no worries about accents
	if ( strlen(word) > MAX_HEX_DIG_LOTE ){
	 	printf("%s: ", word);
	 	puts(lang == 'p' ? NOSUCHBATCH : NOSUCHBATCH_ING);
	}
	
   	sscanf(in,"%*s %s", batch);
	
	Vaccine a = hash_find(sys->ba_vac, batch);	

	// Verify if there was a vaccine with that batch
	if (a.expiration.year == 0){
		printf("%s: ", batch);
		puts(lang == 'p' ? NOSUCHBATCH : NOSUCHBATCH_ING);
		return;
	}
	
	sys->root = removeVaccine(sys->root, a, &found);

	// Verify if there is now a vaccine with that batch
	if (!found){
		printf("%s: ", batch);
		puts(lang == 'p' ? NOSUCHBATCH : NOSUCHBATCH_ING);
		return;
	} 

	printf("%d\n", a.num_doses_used);
	hash_delete(sys->ba_vac, batch);
	sys->count--;
}



// ------------------------------------------------------------------------- //
// ------------------------------ 'd' command ------------------------------ //
// ------------------------------------------------------------------------- //


/** Auxiliar for handling the input with d command
  * @param *sys	The system that has all the info
  * @param *name	The name of the user 
  * @param date		The date of the inoculation  
  * @param lang		The system language
  * @return	Prints all the info about this vaccine like name, batch, date...
*/ 

void d_input_aux_1(Sys *sys, char *name, Date date, char lang) {
    
	// Verrifies if the user is in the system
	if (!find_user(sys->user_hash, name)) {
        printf("%s: ", name);
        puts(lang == 'p' ? NOSUCHUSER : NOSUCHUSER_ING);
        return;
    }

	// Verifies if the date is valid 
    if (!delete_valid_date(sys->date, date)) {
        puts(lang == 'p' ? INVALIDDATE : INVALIDDATE_ING);
        return;
    }

	// Deletes those inoculations from the system 
    delete_user_hash2(sys->user_hash, name, date);
    deleteInoc2(sys->inoc, name, date);
}


/** Auxiliar for handling the input with d command
  * @param *sys	The system that has all the info
  * @param *name	The name of the user 
  * @param date		The date of the inoculation  
  * @param *batch	The batch of the vaccine used 
  * @param lang		The system language
  * @return	Prints all the info about this vaccine like name, batch, date...
*/ 

void d_input_aux_2(Sys *sys, char *name, Date date, char *batch, char lang) {
	
	// If the user is not in the system give an error
	if (!find_user(sys->user_hash, name)) {
        printf("%s: ", name);
        puts(lang == 'p' ? NOSUCHUSER : NOSUCHUSER_ING);
        return;
    }

	// If the date is not valid give an error
    if (!delete_valid_date(sys->date, date)) {
        puts(lang == 'p' ? INVALIDDATE : INVALIDDATE_ING);
        return;
    }

    Vaccine a = hash_find(sys->ba_vac, batch);

	// If the vaccine is not in the system give an error
    if (a.expiration.year == 0) {
        printf("%s: ", batch);
        puts(lang == 'p' ? NOSUCHBATCH : NOSUCHBATCH_ING);
        return;
    }

	// Deleting that inoculation from the system (hash table and linked list)
    delete_user_hash3(sys->user_hash, name, date, batch);
    deleteInoc3(sys->inoc, name, date, batch);
}


/** Handling the input with d command
  * @param *sys	The system that has all the info
  * @param *in	The input command from the user
  * @param *lang	The system language
  * @return	Prints all the info about this vaccine like name, batch, date...
*/ 

void d_input(Sys *sys, char *in, char lang) {
    Date date;
    char name[MAX_BYT_UTENTE_NAME];
    char batch[MAX_HEX_DIG_LOTE];

    // If they only give the name of the user 
	if (sscanf(in, "%*s %s %d-%d-%d %s", name, &date.day, &date.month,
			   &date.year, batch) == 1) {
        
		// Check if the user was in the system or not 
		if (!find_user(sys->user_hash, name)) {
            printf("%s: ", name);
            puts(lang == 'p' ? NOSUCHUSER : NOSUCHUSER_ING);
            return;
        }

		// Delete inoculations of that user in the hash table and linked list
        delete_user_hash1(sys->user_hash, name);
        deleteInoc1(sys->inoc, name);
    } 
	
	// If they give de date and the name of the user
	else if (sscanf(in, "%*s %s %d-%d-%d %s", name, &date.day, &date.month, 
		&date.year, batch) == 4) 
    	d_input_aux_1(sys, name, date, lang);
 
	
	// If the give the date, name of the user and the batch of the vaccine
	else d_input_aux_2(sys, name, date, batch, lang);
    
}



// ------------------------------------------------------------------------- //
// ------------------------------ 'u' command ------------------------------ //
// ------------------------------------------------------------------------- //


/** Handling the input with u command
  * @param *sys	The system that has all the info
  * @param *in	The input command from the user
  * @param *lang	The system language
  * @return	Prints all the info about this vaccine like name, batch, date...
*/ 

void u_input(Sys *sys, char *in, char lang) 
{
	unsigned long length_name;

	// If the input is only u we list all the inoulations by order 
	if (in[0]=='u' && in[1]=='\n') {
		printInoculation(sys->inoc);
		return;
	}

	else{	
		char *name = extractUserName(&length_name, &in);

		// If the user is not in the system we give an error 
		if (!find_user(sys->user_hash, name))
		{
			printf("%s: ", name);
			puts(lang == 'p' ? NOSUCHUSER : NOSUCHUSER_ING);
			return;    
		}

		// Printing the inoculation of that user
		printInoculationUser(sys->inoc, name);
		free(name);
	}
}



// ------------------------------------------------------------------------- //
// ----------------------------- main functions ---------------------------- //
// ------------------------------------------------------------------------- //


/** Function that frees all the info in the system 
  * @param *sys	The system that has all the info
*/ 

void main_free(Sys *sys) {
	
	// Free the AVL tree
    freeTree(sys->root);
	
	// Free the linked list of inoculations 
    freeInoculationList(sys->inoc);

	// Free the Hash table of batch to vaccine 
    freeBatchHash(sys->ba_vac);

	// Free the Hash table of user to inoculations
    free_user_hash(sys->user_hash);

	// free the structures and not what is inside them (already freed)
    free(sys->ba_vac);
    free(sys->inoc);
    free(sys->user_hash);
}


/** Auxiliar function to help the main to decide wich command to use   
  * @param *sys	 The system that has all the info
  * @param *buf	 Buffer with the input from the user 
  * @param lang	 The language of the system
*/ 

void main_input_aux(Sys *sys, char *buf, char lang) {

    switch (buf[0]) {
	
	// q command	
    case 'q':
        main_free(sys);
        exit(0); // If we just put return the program will not end 
    
	// c command 
	case 'c':
        c_input(sys, buf, lang);
        break;

	// l command
    case 'l':
        l_input(sys, buf, lang);
        break;

	// a command
    case 'a':
        a_input(sys, buf, lang);
        break;

	// r command
    case 'r':
        r_input(sys, buf, lang);
        break;

	// d command
    case 'd':
        d_input(sys, buf, lang);
        break;
	
	// u command
    case 'u':
        u_input(sys, buf, lang);
        break;

	// t command
    case 't':
        t_input(sys, buf, lang);
        break;
    }
}


/** Main function of the project   
  * @param argc	 The number of arguments written when running the project
  * @param **argv	Arguments written when running the project
*/ 

int main(int argc, char **argv) {
    char lang = 'i'; // Language is english as default 
    char buf[MAX_CAR_PEREXEC];
    Sys sys;
    
	// Initiating the AVL tree
	sys.root = NULL;

	// Starting the Hash Table batch to vaccine  
    sys.ba_vac = malloc(sizeof(HashMap));
	
	// If the system doesnt have memory to allocate 
	if (!sys.ba_vac) no_memory();

	init_Hash(sys.ba_vac, MAX_NUM_VAC);

	// Starting the Linked List of Inoculations  
	sys.inoc = malloc(sizeof(InoculationList));

	// If the system doesnt have memory to allocate 
	if (!sys.inoc) no_memory();

	initInoculationList(sys.inoc);
    
	// Initiating the Hash Table of User to inoculation
	sys.user_hash = malloc(sizeof(User_HashTable));

 	// If the system doesnt have memory to allocate 
	if (!sys.user_hash) no_memory();

	init_Hash_user(sys.user_hash);
    
	// Inicial date
	sys.count = 0;
    sys.date.day = 1;
    sys.date.month = 1;
    sys.date.year = 2025;

	// Deciding the language
    if (argc > 1 && strcmp(argv[1], "pt") == 0) lang = 'p';

    // Principal loop of the program
	while (fgets(buf, MAX_CAR_PEREXEC, stdin)) {
        main_input_aux(&sys, buf, lang);
    }

    // Free the system when the program is quitted with ctrl + c
	main_free(&sys);
    return 0;
}

