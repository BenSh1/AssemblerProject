#include "assembler.h"

/*
 * checking if the file exists then get files send each one to extern function for checking parsing in according to instruction after that stored all the data from the file 
*/
int main(int argc, char *argv[]) {
    FILE *file_p = NULL; /*pointer to file */
    char *file_n = NULL;        /*file name */
    int i;
    int error_num = 0;
    
    if (argc == 1) /*no paramater execution */
    {
        printf("Error: At least one file must be specified.\n");
	return 1;
    } 
 
    else 
    {
        for (i = 1; i < argc; i++) /* loop for each file */
        {
            file_n = argv[i];
            
	    file_p = fopen(file_n, "r"); /*file open & error check */
	    if(!file_p)
            {
                printf("Error: could not open file \"%s\" \n", argv[i]);
	    }
	    else
  	    {
		/*file lexical & syntax analysis successful*/
                if (!lex_analysis(file_p, file_n)) 
                {
		    fseek(file_p, 0, SEEK_SET);
                    error_num = firstReadLine(file_p, file_n);

		    if(!error_num)
 		    {
		        fseek(file_p, 0, SEEK_SET);
			secondeReadLine(file_p ,file_n);
		    }

		    /*note:  i chose to not execute the function freeDataList , because i didn't succeed to execute the function "freeDataList" without errors tried a lot..*/
		    freeCommandList();
    		    /*freeDataList();*/
    	 	    freeSymbolTable();

		}

               	fclose (file_p); 
            }

        } /*end for */
    }
    return 0;
} /* end main */


