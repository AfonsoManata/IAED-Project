/**
 * Instituto Superior Técnico
 * Projeto IAED - Vaccine Managment System
 * @file date.c 
 * Afonso Manata ist 1113406
 */

#include "date.h"

// This file has functions to manipulate dates and other auxiliars // 


/** Prints the no_memory error
 */

void no_memory(){
	puts(NOMEMORY);
	exit(0);
}


/** Verify if the year is a leap year
 * @param year	Year that we are verifying
 * @return	1 if the date is a leap year otherwise 0
 */

int is_leap_year(int year) { 	// Condition to be a leap year
  return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}


/** Function to calculate the max of two numbers
 * @param a 	An integer that will be compared 
 * @param b 	An integer that will be compared 
 * @return	The max of those numbers 
 */ 

int max(int a, int b) {
    return (a > b) ? a : b; // In line condition to verify who is the max num
}


/** Exctrating the name of the user 
  * @param *length_name	The length of the name of the user
  * @param *in	The input command from the user
  * @return	 Returns the name of the user 
*/ 

char *extractUserName(unsigned long *length_name, char **in) {
    
	// Move pointer to the beginning of the name (after first space)
    char *pos = strchr(*in, ' ')+1;
    char *username;
    *in = pos;

    // Name is enclosed in quotation marks
    if (*pos == '"') {
        
		// Skip the opening quote
        pos++;
		char *quote_end = strchr(pos, '"');

		// Calculate the size of the name and allocate memory to it 
        *length_name = quote_end - pos;
        username = malloc(*length_name + 1);

		// If the system doesnt have memory to allocate 
		if (!username) no_memory();

        // Copy and add the final string caracter
		strncpy(username, pos, *length_name);
        username[*length_name] = '\0';

        *in = quote_end + 1;  // Move past the closing quote
    } 
    
	// Name is a single word (no quotes)
    else {
        
		// Go to the positio of the name  
		char *space_end = strchr(pos, ' ');
        if (!space_end) space_end = pos + strlen(pos)-1;

		// Calculate the length of the name
        *length_name = space_end - pos;
        username = malloc(*length_name + 1);

		// If the system doesnt have memory to allocate 
		if (!username) no_memory();

        // Copy and add the final string caracter 
		strncpy(username, pos, *length_name);
        username[*length_name] = '\0';

        *in = space_end;
    }
    return username;
}


/** Verify if this batch is valid
  * @param batch	The input batch from the user
  * @return	1 if its a valid batch for a vaccine otherwise 0
*/

int validate_batch(char *batch){
	
	int length = strlen(batch);

	// Verify the size 
    if (length == 0 || length > 20) {
        return 0;
    }

	// Verify if the batch verifies the rules
    for (int i = 0; i < length; i++) {
        if (!(batch[i]>='0'&&batch[i]<='9')&&!(batch[i]>='A'&&batch[i]<='F')) 
            return 0;
    }

    return 1;
}


/** Verify it its a valid name 
  * @param *in	The input of the name of the vaccine from the user
  * @return	1 if its a valid name for a vacinne otherwise 0
*/

int validate_name(char *name)
{
	int size = strlen(name); 

	// Verify if the name follows the rules
	for ( int i = 0; i < size; i++)
	{
		if (name[i] == ' ' || name[i] == '\t' || name[i] == '\n') return 0;
	}
	
	return 1;
}


/** Verify if the new date is more recent than the other date
 * @param d1	Current date of the system
 * @param d2	Possible new system date
 * @return	negative if the date2 is more recent and vice versa
 */

int compareDates(Date date1, Date date2) {
    
	if (date1.year != date2.year) return date1.year - date2.year;
    if (date1.month != date2.month) return date1.month - date2.month;
    return date1.day - date2.day; // negative if the dat2 is more recent
}


/** Verify if the new date is valid
 * @param d1	Current system date
 * @param d2	Possible new system date
 * @return	1 if the date is valid otherwise 0
 */

int valid_date(Date d1, Date d2) {
	
	if ( d2.month < 1 || d2.month > 12 || d2.year < 1) return 0;

	int days_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Adjust for leap years
	if (is_leap_year(d2.year)) days_month[1] = 29;

	int bigger = compareDates(d1, d2);

	if (bigger <= 0) bigger = 1; // if the new date is more recent
	else bigger=0;
	
	return (d2.day>0 && d2.day<=days_month[d2.month - 1] && bigger);
}


/** Verify if the date of the inoculation is valid
 * @param d1	Current system date
 * @param d2	Inoculation date
 * @return	1 if the date is valid otherwise 0
 */

int delete_valid_date(Date d1, Date d2) {
	if ( d2.month < 1 || d2.month > 12 || d2.year < 1)
		return 0;

	int days_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Adjust for leap years
	if (is_leap_year(d2.year)) days_month[1] = 29;

	int bigger = compareDates(d1, d2);

	if (bigger < 0) bigger = 0; // If the current date is more recent
	else bigger=1;
	
	return (d2.day>0 && d2.day<=days_month[d2.month - 1] && bigger);
}

