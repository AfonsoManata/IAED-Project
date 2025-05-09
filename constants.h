/**
 * Instituto Superior Técnico
 * Projeto IAED - Vaccine Managment System
 * @file constants.h 
 * Afonso Manata ist 1113406
 */


#ifndef CONSTANTS_H 
#define CONSTANTS_H


// ------------------------------- Libraries ------------------------------- //

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



// ------------------------------ Constants -------------------------------- //

#define MAX_NUM_VAC 1000              // Max number of vaccines
#define MAX_BYT_UTENTE_NAME 200       // Max number of bytes in the user's name
#define MAX_BYT_VAC_NAME 50           // Max number of bytes in the vaccine name
#define MAX_HEX_DIG_LOTE 20           // Max number of hex digits in the batch 
#define MAX_CAR_PEREXEC 65535         // Max number of characters per execution
#define TABLE_SIZE 100                // Size of the hash table


// ------------------------------- Errors PT ------------------------------- //

#define TOOMANYVACCINES "demasiadas vacinas"     // System full of vaccines
#define DUPLICATEBATCH "número de lote duplicado"// Batch number already exists
#define INVALIDBATCH "lote inválido"             // Invalid batch format
#define INVALIDNAME "nome inválido"              // Invalid name
#define INVALIDDATE "data inválida"              // Invalid date
#define INVALIDQUANTITY "quantidade inválida"    // Invalid quantity (<= 0)
#define NOSUCHVACCINE "vacina inexistente"       // Vaccine not found
#define NOSTOCK "esgotado"                       // No stock available
#define ALREADYVACCINATED "já vacinado"          // User already vaccinated
#define NOSUCHBATCH "lote inexistente"           // Batch not found
#define NOSUCHUSER "utente inexistente"          // User not found


// ------------------------------- Errors ING ------------------------------ //

#define NOMEMORY "no memory"						// No memory available
#define TOOMANYVACCINES_ING "too many vaccines"		// System full of vaccines 
#define DUPLICATEBATCH_ING "duplicate batch number"	// Number already exists 
#define INVALIDBATCH_ING "invalid batch"            // Invalid batch format 
#define INVALIDNAME_ING "invalid name"              // Invalid name 
#define INVALIDDATE_ING "invalid date"              // Invalid date 
#define INVALIDQUANTITY_ING "invalid quantity"      // Invalid quantity (<= 0)
#define NOSUCHVACCINE_ING "no such vaccine"         // Vaccine not found 
#define NOSTOCK_ING "no stock"						// No stock available 
#define ALREADYVACCINATED_ING "already vaccinated"	// User already vaccinated 
#define NOSUCHBATCH_ING "no such batch"         	// Batch not found 
#define NOSUCHUSER_ING "no such user"				// User not found 



#endif 

