
#include "utility.h"

/* return memory allocated substring of str, between start (include) and finish (exclude)
if there was an error in alloction stop the program
the function is designate to the symbol name , we want to cut the ':' for use only the name of the symbol*/
char * subString(int start, int end, char *str )
{
    char *result;
    int first;

    if(*(str + end) != ':')/*if there was colon*/
    {
	end++;
    	/* allocate memory for the result*/
    	result = (char *)malloc(sizeof(char) * (end - start ));
    	first = start;
    	if (!result)
    	{
           printf("Error: memory allocation failed\n");
           exit(1);
       	}

    	while (start < end )/*stored the name of the label in result variablt*/
   	{
     	   result[start - first] = str[start];
     	   start++;
    	}
    	result[end - first] = '\0';
    	return result;
    }
    /*if there wasn't colon*/
    /* allocate memory for the result*/
    result = (char *)malloc(sizeof(char) * (end - start));
    first = start;
    if (!result)
    {
        printf("Error: memory allocation failed\n");
        exit(1);
    }

    while (start < end)/*stored the name of the label in result variablt*/
    {
        result[start - first] = str[start];
        start++;
    }
    result[end - first] = '\0';
    return result;
}

/*checking if the number from the string(token) is a valid number */
int validNumber(char * token , int currIndex)
{
    int num = 0;
    int i = currIndex;
    int digit;

    while (isdigit(token[i])) /*stroed the number at the variable "num"*/
    {
	digit = token[i] - '0';
        num *= 10;
        num += digit;
	i++;
    }	    
    if (num > 32 || num < 0) /*check valid num in accordance with num of bytes, sign is irrelevant, min equal max */
    {
        return NUM_OUT_OF_RANGE;
    }
    return 0;

}

/*helper func to remove all spaces from a string*/
void remove_spaces(char* s) {
    const char* d = s;
    do {
        while (isspace(*d)) {
            ++d;
        }
    } while (*s++ = *d++);
}



