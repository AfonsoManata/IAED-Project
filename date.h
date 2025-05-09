/**
 * Instituto Superior Técnico
 * Projeto IAED - Vaccine Managment System
 * @file date.h 
 * Afonso Manata ist 1113406
 */

#ifndef DATE_H
#define DATE_H

#include "constants.h"


/** Struct of the date*/ 

typedef struct {
  int day;	// day of the date
  int month;	// month of the date 
  int year;		// year of the date 
} Date;

// Functions for date manipulation

/** Checks if the given year is a leap year
 * @param year	The year to check
 * @return	1 if the year is a leap year, otherwise 0
 */
int is_leap_year(int year);

/** Function to calculate the maximum of two numbers
 * @param a 	An integer to compare
 * @param b 	An integer to compare
 * @return	The maximum value between a and b
 */
int max(int a, int b);

/** Exctrating the name of the user 
  * @param *length_name	The length of the name of the user
  * @param *in	The input command from the user
  * @return	 Returns the name of the user 
*/ 
char *extractUserName(unsigned long *length_name, char **in); 
   
/** Verify if this batch is valid
  * @param batch	The input batch from the user
  * @return	1 if its a valid batch for a vaccine otherwise 0
*/
int validate_batch(char *batch);

/** Verify it its a valid name 
  * @param *in	The input of the name of the vaccine from the user
  * @return	1 if its a valid name for a vacinne otherwise 0
*/
int validate_name(char *name);

/** Compares two dates and checks if date1 is later than date2
 * @param date1	The current system date
 * @param date2	Possible new system date
 * @return	A negative value if date2 is more recent, otherwise positive
 */
int compareDates(Date date1, Date date2);

/** Verifies if the new date is valid
 * @param d1	Current system date
 * @param d2	Possible new system date
 * @return	1 if the date is valid, otherwise 0
 */
int valid_date(Date d1, Date d2);

/** Verifies if the inoculation date is valid
 * @param d1	Current system date
 * @param d2	Inoculation date
 * @return	1 if the date is valid, otherwise 0
 */
int delete_valid_date(Date d1, Date d2);

/** Prints the no memory error message */
void no_memory();

#endif
