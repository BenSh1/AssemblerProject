#include "lexical_analysis.h"

/*
 * converting stream into a sequence of tokens & analyzing them , and printing errors if occur
 * file_p - pointer to given file
 * file_n - file name
 * returns - returns 0 lexing successful, 1 otherwise
 */
int lex_analysis(FILE * file_p, const char * file_n) 
{
    char original_line [MAX_LINE_LENGTH] = {0};
    char temp_line [MAX_LINE_LENGTH] = {0}; /* line to be cut to tokens */
    char * token = NULL; /*token from stream line */
    int line_num = 1; /* line number for error printing */
    int error_num = 0; 
    char ch;
    int flagErr = 0; /*flag to indicate if there is error in the syntax of code*/

    while (fgets(original_line, MAX_LINE_LENGTH, file_p))
    {
        if (original_line[strlen(original_line) - 1] != '\n' && strlen(original_line) >= MAX_LINE_LENGTH - 1) /* checking if the line overflow */
        {
	    
            while ((ch = getc(file_p)) != '\n' && !feof(file_p)); /*flush extra chars */
	    flagErr = 1;
	    error_num = 1; 
            print_error(LINE_INVALID_LENGTH, file_n, line_num++, NULL);
            continue;
        }

        strcpy(temp_line, original_line);

        if (!is_not_comment_or_empty_line(temp_line))
        {
            token = strtok(temp_line, "\n\t\r ");
            if (IS_INSTRUCTION(token)) /*it is not an instruction */
            {
                if (IS_DIRECTIVE(token)) /*it is not a directive */ 
                {
                    if (token[0] == '.') 
                    {
			flagErr =1;
			error_num = DIRECTIVE_UNRECOGNIZED;
                        print_error(error_num, file_n, line_num++, token);
                        continue;
                    } 
		    else 
		    {
                        error_num = is_label(token, 1);
                        if (error_num) /*not a label */
                        { 
			    flagErr =1;
                            print_error(error_num, file_n, line_num++, token);       
                            continue;
                        } 
			else  /*it is a label */
			{ 
			    error_num = analyse_type(token, LABEL_CMD, original_line);
                            if (error_num)
                            {
				flagErr = 1;
                        	print_error(error_num, file_n, line_num++, token);
                        	continue;
                            }
                        }
                    }
                } 
		else 
		{ /* is a directive */

                    if ((error_num = analyse_type(token, DIRECTIVE_CMD, original_line)))
                    {
			flagErr =1;
                        print_error(error_num, file_n, line_num++, token);
                        continue;
                    }
                }
            } 
	    else 
	    { /*is an instruction */
		error_num = analyse_type(token, INSTRUCTION_CMD, original_line);
                if (error_num)
                {
		    flagErr =1;
                    print_error(error_num, file_n, line_num++, token);
                    continue;
                }
            }     
        }
        line_num++;
    } /*end while fgets*/


    return flagErr;
} /*end of lexical analysis */

/*
 * check if given string is an empty line or a comment:
 * str - given string to examine - line from file
 * returns - returns 0 if not empty or comment, 1 otherwise
 */
int is_not_comment_or_empty_line(const char * str) {
    int i = 0;

    if (str[i] == ';') /* checking if the first letter in the line is ';' then it is comment line */
        return 1; 

    while (isspace(str[i++]) && str[i] != '\0'); 
    if (str[i] == '\0')/* check if empty line  or skip white letters */ 
        return 1;

    return 0; 
} /*end of is_not_comment_or_empty_line */

/*
 * check if given string is a label
 * starts with a letter, contain only letters and digits
 * with or without colon depending on flag
 * str: given string to examine
 * check_colon: flag to check ending colon or not
 * returns: returns 0 (OK) if it is a label, error number otherwise
 */
int is_label( const char * str, int check_colon) {
    int i = 0;
    const char * tmp = str;

    if (check_colon)
    {
        if (str[strlen(str)-1] != ':')
            return LABEL_NO_ENDING_COLON;
    }

    if (strlen(str) == 0 || (check_colon && strlen(str) == 1))/*checking if the length of the string is zero or 1 with colon then print error*/
    {
        return 0;
    }

    if(check_colon)/*if it is with colon then subtract 1 char at the end of the label to check equal without colon*/
    {
    	for (i = 0; i < 27; i++)
    	{ 
	     if (strncmp(tmp, getOpCodes()[i].name , strlen(tmp) - 1) == 0)
    	     	return LABEL_EQUAL_TO_SAVE_WORD;
    	}

    }
    else if(!check_colon)/*if it is without colon then only use strcmp for checking equal*/
    {
    	for (i = 0; i < 27; i++)
    	{ 
	     if (strcmp(tmp, getOpCodes()[i].name) == 0)
    	     	return LABEL_EQUAL_TO_SAVE_WORD;
    	}
    }


    i = 0;
    if (!isalpha(str[i++])) /* check label initial */
        return LABEL_INVALID_INITIAL; 
    
    while (str[i] != '\0') /*check rest of label syntax */
    {
        if (!isalnum(str[i]))
        {
            if (check_colon) 
            {
                if (!(str[i] == ':' && str[i + 1] == '\0')) /*check if end of label with colon */
                    return LABEL_INVALID;
            } else {
                return LABEL_INVALID;
            }
        }
        if (i > MAX_LABEL_LENGTH - 1)
        {
            return LABEL_INVALID_LENGTH; /* -1 max_label_length for end of string */
        }
        i++;
    }
    
    return OK; 
} /* end of is_label */
