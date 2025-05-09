/**
 * Instituto Superior Técnico
 * Projeto IAED - Vaccine Managment System
 * @file inoculation.h 
 * Afonso Manata ist 1113406
 */


#ifndef INOCULATION_H
#define INOCULATION_H

#include "constants.h"
#include "date.h"  

/** Structure to represent an inoculation */
typedef struct {
    char batch[MAX_HEX_DIG_LOTE + 1]; // Batch of the vaccine used(hexadecimal)
    Date date;                 // Date of the inoculation
    char *user;                // Name of the user who received the inoculation
} Inoculation;

#endif  
