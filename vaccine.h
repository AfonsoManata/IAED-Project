/**
 * Instituto Superior Técnico
 * Projeto IAED - Vaccine Managment System
 * @file vaccine.h 
 * Afonso Manata ist 1113406
 */

#ifndef VACCINE_H
#define VACCINE_H


#include "constants.h"
#include "date.h"  


/** Structure to represent a vaccine */
typedef struct {
    char batch[MAX_HEX_DIG_LOTE + 1];  // Batch of the vaccine (hexadecimal)
    Date expiration;                   // Expiration date of the vaccine
    int num_doses_available;           // Number of doses available 
    int num_doses_used;                // Number of doses used  
    char name[MAX_BYT_VAC_NAME + 1];   // Name of the vaccine
} Vaccine;


#endif 
