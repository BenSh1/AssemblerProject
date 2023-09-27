#include "syntax_analysis.h"


extern int islabel;
/*
 * the function analyse the type of the line and handle the command line in according to the type of the line
 * token - is the instruction command name or directive command name or label name
 * cmd_type - is the type of the line 
 * this_line - is the all line 
 */
int analyse_type(char * token, const int cmd_type, char this_line[]) {
    switch(cmd_type) {
        case INSTRUCTION_CMD: 
	    return (syntax_analyse_ins(token, this_line));
            break;
        case DIRECTIVE_CMD:
            return (syntax_analyse_dir(token, this_line));
            break;
        case LABEL_CMD:
            token = strtok(NULL, "\n\t\r ");
	    if(token == NULL)/*checking if after label there isn't any parmater if it is true then print error*/
	     	return MISSING_PARAM;
            if (!IS_INSTRUCTION(token))
                return (syntax_analyse_ins(token , this_line));
            if (!IS_DIRECTIVE(token))
                return(syntax_analyse_dir(token , this_line));
            break;
    }
    return 0;
}

/*
 * checking if there is any errors in the instruction type of line 
 * command - is the rest of the line without the label if there in the line
 * this_line - is the all of the line includeing symbol if there
 * return 0 if no errors , otherwise return any number that not equal to the number zero.
 */
int syntax_analyse_ins(char *command, char this_line[])
{
    int error_num = 0;
    struct opCodes tmp;
    int i = 0;
    int numberOfParms = 0;
    int num = 0;
    int comma_flag = 1; /*0 if possible to add comma (after number), 1 if not possible (at start or after another comma)*/ 
    int registerFlag = 0;/*1 = if it is register , 0 = if it is not regitster*/
    int immedFlag = 0;
    char delim[]=" \t\n\r";
    char * token;
    int labelFlag = 0;/*indicate if there is label in the line*/
    int digit;/*stored digit of the number in the line*/
    char code[4];
    int commandFlag = 0;/*indicate if the command name is a valid command*/
    int size = 1;
    char *label = (char *)calloc(size,sizeof(char));/*indicate if there is label in the line*/
    int index_inLabel = 0;

    token = strtok(this_line, delim);

    if (token[i] == ',')/*checking if there is comma at the start of the line*/
    {
        if (comma_flag)
    	    return MISPLACED_COMMA_START;
    }
    
    if(!is_label(token , 1))/*checking if there is lable in the line*/
    {
	token = strtok(NULL, delim);
	labelFlag = 1;
    }

    if (token[i] == ',')/*checking if there is comma after label*/
    {
        if (comma_flag)
    	    return MISPLACED_COMMA_START;
    }

    if(!strcmp(token , "stop") )/*checking if it is "stop" command*/
    {
	if( (token = strtok(NULL, delim)) != NULL)/*checking if after "stop" command there is parameter*/
	{
	     error_num = EXTRANEOUS_OPERAND; 
	}
	else
	{
	     error_num = OK;
	}
	return error_num;
    }

    for (i = 0; i < 27; i++)/*find the opcode in the table code*/
    { 
	if (strcmp(command, getOpCodes()[i].name) == 0)
        {
            tmp = getOpCodes()[i]; 
	    commandFlag = 1;         
    	}
    }

    token = strtok(NULL, delim);

    if(token == NULL)/*checking if after command (for example add ,addi ,jmp,..) there isn't any paramter , if it is true then print error */
    {
	return MISSING_PARAM;
    } 


    i = 0;

    if (token[i] == ',')/*checking if after command name there is comma*/
    {
        if (comma_flag)
    	    return MISPLACED_COMMA_START;
        else
            comma_flag = 1;
      	i++;
    }

    numberOfParms = 0;

    /*in this part of the code we will handle the syntax of the group R , I and J*/

    if (tmp.commandGroup == 'R')
    {
        while (token)
        {
            i = 0;
            while (token[i] != '\0')
            {
                while (isspace(token[i]))/*skip space*/
                {
                    i++;
                }

                if (token[i] == ',')/*checking consecutive comma*/
                {
                    if (comma_flag)
                    {
                        return CONSECUTIVE_COMMA;
                    }
                    else
                        comma_flag = 1;
                    i++;
                }
                if (token[i] == '$')/*checking register number*/
                {
                    i++;

                    if (isdigit(token[i]))
                    {
                        if (numberOfParms > 0 && comma_flag == 0)/*checking if the number of paramter is more than one so before each register need to b e comma (so if the comma flag = 0 , then it is meaning no comma before was found )*/
                        {
                            return MISSING_COMMA;
                        }

                        error_num = validNumber(token, i);/*checking valid number of the reigster*/
                        if (error_num)
                        {
                            return error_num;
                        }

                        while (isdigit(token[i]))/*skip the register*/
                        {
                            i++;
                        }

                        numberOfParms++;
                        comma_flag = 0;
                    }
                    else
                    {
                        return INVALID_REGISTER;
                    }
                }
		/*if token[i] does not equal to one of each things( space , $ \0) then print errorr */
                if (!isspace(token[i]) && token[i] != ',' && token[i] != '$' && token[i] != '\0')
                {
                    return INVALID_PARAM;
                }
            }
            token = strtok(NULL, delim);/*move to the next part of the line*/
        }
    	if (comma_flag) /*check if exist trailing comma */
	    return MISPLACED_COMMA_END;

	/*checking if the number of paramter is right in depnce of the opcode */
        if (tmp.code == 0)
        {
            if (numberOfParms > 3)
            {
                return EXTRANEOUS_OPERAND;
            }
            else if (numberOfParms < 3)
            {
                return MISSING_PARAM;
            }
        }
        else if (tmp.code == 1)
        {
            if (numberOfParms != 2 && numberOfParms > 2)
            {
                return EXTRANEOUS_OPERAND;
            }
            else if (numberOfParms != 2 && numberOfParms < 2)
            {
                return MISSING_PARAM;
            }
        }

    }

    else if (tmp.commandGroup == 'I')
    {
        if ((tmp.code >= 10 && tmp.code <= 14) || (tmp.code >= 19 && tmp.code <= 24))
        {
            while(token)
            {
                i = 0;
                while (token[i] != '\0')
                {
                    num = 0;
                    while (isspace(token[i]))/*skip space*/
                    {
                        i++;
                    }

                    if (token[i] == ',')/*checking consecutive comma*/
                    {
                        if (comma_flag)
                            return CONSECUTIVE_COMMA;
                        else
                            comma_flag = 1;
                        i++;
                    }

                    if (token[i] == '$')/*checking register*/
                    {
			/*checking order parmeter in line : 
			one situation  wasn't register and immed number
			two situation was register and immed number  */
                        if ((registerFlag == 0 && immedFlag == 0) || (registerFlag == 1 && immedFlag == 1))
                        {
                            i++;
                            if (isdigit(token[i]))
                            {
				if(comma_flag == 0)
                        	{
                                    return MISSING_COMMA;
                        	}
                                /*return 0 if it is valid number*/
                                error_num = validNumber(token, i);
                                if (error_num)
                                {
                                    return error_num;
                                }

                                while (isdigit(token[i]))
                                {
                                    i++;
                                }

                                comma_flag = 0;
                                registerFlag = 1;
                                numberOfParms++;
                            }
                            else
                            {
                                return INVALID_REGISTER;
                            }
                        }
			/*if already was register and wasn't immed number and this time is seconde register number then print error */
                        else if (registerFlag == 1 && immedFlag == 0)
                        {
                            return INVALID_PARAM; /*INVALID IMMED NUMBER*/
                        }
                    }

		    /*handle immed nubmer with sign*/
                    if (token[i] == '+' || token[i] == '-')
                    {
                        i++;
                        if (!isdigit(token[i]))/*checking if the next char is not digit if it is true then print error*/
                        {
                            return INVALID_PARAM;
                        }
                        else
                        {
			    /*only if there was reigster before immed number*/
                            if (registerFlag)
                            {
				if(comma_flag == 0)
                        	{
                                    return MISSING_COMMA;
                        	}
                                /*return 0 if it is valid number*/
                                error_num = validNumber(token, i);
                                if (error_num)
                                {
                                    return error_num;
                                }

                                while (isdigit(token[i]))/*skip the numberof the register*/
                                {
                                    i++;
                                }

                                comma_flag = 0;
                                numberOfParms++;
                                immedFlag = 1;
                            }
				/*there wasn't register before immed number so print error */
                            else
                            {
                                return INVALID_REGISTER;
                            }
                        }
                    }
		    /*handle immed nubmer without sign*/
                    else if (isdigit(token[i]) && token[i] != '\0')
                    {
                        num = 0;
                        if (registerFlag)
                        {
                            /*return 0 if it is valid number*/
                            error_num = validNumber(token, i);
                            if (error_num)
                            {
                                return error_num;
                            }

                            while (isdigit(token[i]))
                            {
                                i++;
                            }

                            comma_flag = 0;
                            numberOfParms++;
                            immedFlag = 1;
                        }
                        else
                        {
                            return INVALID_REGISTER;
                        }
                    }

                }
                token = strtok(NULL, delim);
            }
	    /*checking how many parameter in the line*/
            if (numberOfParms > 3)
            {
                return EXTRANEOUS_OPERAND;
            }
            else if (numberOfParms < 3)
            {
                return MISSING_PARAM;
            }

    	    if (comma_flag) /*check if exist trailing comma */
	    	return MISPLACED_COMMA_END;
	    
	}
	else if (tmp.code >= 15 && tmp.code <= 18)
	{
            while(token)
            {
		index_inLabel=0;
                i = 0;
                while (token[i] != '\0')
                {
		    if(numberOfParms > 3 )
		    {
			return EXTRANEOUS_OPERAND;
		    }
                    num = 0;
                    while (isspace(token[i]))/*skip space*/
                    {
                        i++;
                    }

                    if (token[i] == ',')/*checing consecutive comma*/
                    {
                        if (comma_flag)
                            return CONSECUTIVE_COMMA;
                        else
                            comma_flag = 1;
                        i++;
                    }
		    else if (token[i] == '$')/*checking register*/
            	    {
                    	i++;
                    	if (isdigit(token[i]))
                    	{
                        
                    	    error_num = validNumber(token, i);
                    	    if (error_num)
                    	    {
                            	return error_num;
                    	    }

                    	    while (isdigit(token[i]))
                    	    {
                            	i++;
                    	    }

                    	    comma_flag = 0;
                    	    numberOfParms++;
                    	}
               	    	else
                    	{
                    	    return INVALID_REGISTER;
                    	}
                    
            	    }
		    else if(isalpha(token[i]))/*checking label*/
		    {
		    	while(isalnum(token[i]))
		    	{
		    	    if(numberOfParms < 2)/*checking if was less than 2 parameter before label */
		    	    {
			    	return COMMAND_WRONG_ORDER;
		    	    }
		    	    else if(numberOfParms > 2 )/*checking if was more than 2 parameter before label */
		    	    {
			    	return EXTRANEOUS_OPERAND;
		    	    }

                	    *(label + index_inLabel) = token[i];/*stored the name of the label */

                	    index_inLabel++;/*counter how many letters is in label*/

                	    if (index_inLabel == size)/*add memory to the pointer label the number of letters is equal to size of label*/
                	    {
                    	    	size++;
                    	    	label = (char *)realloc(label,size * sizeof(char));
                	    }
			    i++;
		        }
			comma_flag = 0;
			numberOfParms++;
		    }
		    else 
		    {
			return INVALID_PARAM;
		    }

		}
		
		
		token = strtok(NULL, delim);
	    }

 	    error_num = is_label(label , 0);/*checking if the name of the label is habe problem*/
	    if(error_num)
	    {		
		return error_num;
	    }

	    free(label);

    	    if (comma_flag) /*check if exist trailing comma */
	    	return MISPLACED_COMMA_END;
	}


    }

    else if (tmp.commandGroup == 'J')
    {

        if ((tmp.code == 30))
        {
            while(token)
            {
                i = 0;
                index_inLabel = 0;
                while(token[i] !='\0')
                {
                    while (isspace(token[i]))
		    {
			i++;
		    }

                    if (isalpha(token[i]))/*stored the name of the label*/
                    {
                        while (isalnum(token[i]))
                        {
                            *(label + index_inLabel) = token[i];

                            index_inLabel++;/*how many letters in the word that needed to be label*/

                            if (index_inLabel == size)
                            {
                                size++;
                                label = (char *)realloc(label, size * sizeof(char));
                            }
                            i++;
                        }
                        numberOfParms++;
                    }

                    else if (token[i] == '$')/*checking if it is register*/
                    {
                        i++;
                        if (isdigit(token[i]))
                        {
                            error_num = validNumber(token, i);/*checking number of register*/
                            if (error_num)
                            {
                                return error_num;
                            }
                            while (isdigit(token[i]))
                            {
                                i++;
                            }
                            numberOfParms++;
                        }
                        else
                        {
                            return INVALID_REGISTER;
                        }
                    }
                    else
                    {
			if(token[i] == ',')/*checking if it is comma if it is true then error becuse no neeed comma at this type of line*/
			{
			    return INVALID_COMMA;
			}
			else
                            return INVALID_PARAM;/*if not register , label or comma then it is unknow parmeter and print error*/
                    }
                }
                token = strtok(NULL, delim);
            }
	    /*checking how many parmeter in the line*/
	    if(numberOfParms > 1)
	    {
		return EXTRANEOUS_OPERAND;
	    }
	    else if(numberOfParms <1)
 	    {
                return MISSING_PARAM;
	    }
	    /*checking label*/
            error_num = is_label(label, 0);
            if (error_num)
            {
                return error_num;
            }
            free(label);


        }

        else if ((tmp.code == 31) ||(tmp.code == 32) )
        {
            while(token)
            {
                i = 0;
                index_inLabel = 0;
                while(token[i] !='\0')
                {
                    while (isspace(token[i]))/*skip space*/
		    {
			i++;
		    }

                    if (isalpha(token[i]))
                    {
                        while (isalnum(token[i]))/*stored the name of the label*/
                        {
                            *(label + index_inLabel) = token[i];
                            index_inLabel++;

                            if (index_inLabel == size)
                            {
                                size++;
                                label = (char *)realloc(label, size * sizeof(char));
                            }
                            i++;
                        }
                        numberOfParms++;
                    }
                    else
                    {
			if(token[i] == ',')
			{
			    return INVALID_COMMA;
			}
			else
                            return INVALID_PARAM;
                    }
                }

		token = strtok(NULL, delim);
	    }
	   /*checking how many parmeter*/
	    if(numberOfParms > 1)
	    {
		return EXTRANEOUS_OPERAND;
	    }
	    else if(numberOfParms < 1)
 	    {
                return MISSING_PARAM;
	    }
	   /*checking valid label*/
            error_num = is_label(label, 0);
            if (error_num)
            {
                return error_num;
            }
            free(label);
	}

    }
	
    return OK;
}

/*
 *checking if there is any errors in the directive type of line 
 * token - is the rest of the line without the label if there in the line
 * this_line - is the all of the line includeing symbol if there
 * return 0 if no errors , otherwise return any number that not equal to the number zero.
 */
int syntax_analyse_dir(char * token, char this_line[]) 
{
    int error_num;
    int data_size = 0;

    if (!strcmp(token, ".db") || !strcmp(token, ".dh") || !strcmp(token, ".dw") )
    {
        switch (token[strlen(token)-1]) /*find the type of the data size*/
        {
            case 'b': data_size = DB; break;
            case 'h': data_size = DH; break;
            case 'w': data_size = DW; break;
        }

        token = strtok(NULL, "\n\t\r ");

        if ((error_num = check_start(token)))
            return error_num;

        return (numbers_analysis(token, data_size ));
    }

    else if (!strcmp(token, ".asciz"))
    {

        token = strtok(NULL, "\n\t\r ");

	if(strlen(token)>MAX_STRING_LENGTH)
	{
	    return STRING_INVALID_LENGTH;
	}
        if ((error_num = check_start(token)))
            return error_num;   
  
        return (string_analysis(token));
    }

    else if (!strcmp(token, ".entry") || !strcmp(token, ".extern"))
    {
	token = strtok(NULL, "\n\t\r ");
        if ((error_num = check_start(token)))
            return error_num;

        if ((error_num = is_label(token, 0)))
            return error_num; 
   
        token = strtok(NULL, "\n\t\r ");

        if (token != NULL) /*only one parameter allowed in entry / extern*/
            return EXTRANEOUS_OPERAND;
    }
    else 
	return DIRECTIVE_UNRECOGNIZED;

    return OK;
}
/*
 * checking if the start of the line is doesn't start with comma or there is exist parmeter
 * token - is pointer to the first part of the line
 * return 0 if it ok then return any non zero number
*/
int check_start(char *token) {
    if (token == NULL)
        return MISSING_PARAM;
    if (token[0] == ',')
        return MISPLACED_COMMA_START;
    return OK;
}
/*
 * 
 * this_line - is the line that we want to check validate
 */
int string_analysis(char this_line[]) 
{

    char asci_str[MAX_STRING_LENGTH];
    int quot_start = -1, quot_end = -1;
    /*int quoteFlag = 0;*/
    int len = strlen(this_line);
    /*int numOfQuote = 0;*/
    int i = 0;
    char delim[]=" \t\n\r";
    char * token = this_line;

    /* get first quote */
    while (this_line[i] != '"' && i++ < len);

    if(this_line[i] != '"' && i >= len)
	return INVALID_STRING;/*not found starting quote */

    else if (this_line[i] == '"')
    {
        quot_start = i;/*save the index*/
    }
   
    /* get last quote */
    i = len - 1;
    while (this_line[i] != '"' && i-- >0);

    if(this_line[i] != '"' && i <= 0)
	return INVALID_STRING;/*not found ending quote */

    else if (this_line[i] == '"')
        quot_end = i;    

    if (quot_start != 0) /*first char is not quote */
        return STRING_NOT_IN_QUOTE;

    if (quot_end != len - 1)  /*last char is not quote */
        return INVALID_STRING;

    if ((quot_end - quot_start - 1) > MAX_STRING_LENGTH - 1)/*checking length of the string*/
        return STRING_INVALID_LENGTH;

    if( (token = strtok(NULL,delim)) != NULL) /*checking if there is no white letters in trailing end of the sentence */
    {
	return INVALID_STRING;
    }

    return OK;
}



/*
 * analysis the line for checking validation of the line
 * token - is the part of the line that need to check validate
 * bytes - is the data size
 * return zero if there are not any errors in the synta otherwise return non zero number.
 */
int numbers_analysis(char * token, int bytes) {
    int i = 0;
    int comma_flag = 1; /*0 if possible to add comma (after number), 1 if not possible (at start or after another comma) */
    int num; /*number parameter */
    char delim[]=" \t\n\r";

    if (token[i] == ',') /*check  comma start */
    {
	if (comma_flag)
    	    return MISPLACED_COMMA_START;
    } 

    while(token)
    {
	i = 0;
    	while (token[i] != '\0')  
    	{   
            num = 0;
	    while(isspace(token[i]))/*skip space at the start of the line*/
	    {
	    	i++;
	    }
            if (token[i] == ',') /*check comma */
            {
            	if (comma_flag)
                    return CONSECUTIVE_COMMA;
                else
                    comma_flag = 1;
                i++;
            }
            if (token[i] == '-' || token[i] == '+') /*check if sign exists */
            {
	    	if(!isdigit(token[i + 1]) )/*check if the letter after the sign + or - is digit , if it's not digit then return error invalid paramter */
	    	{
		    return INVALID_PARAM;
	    	}
	    	i++;
            }

	    if(isdigit(token[i]))/*checking nubmer*/
	    {
	    	if (comma_flag == 0)/*checking if wasn't comma before number if it true then print error*/
            	{
		    return MISSING_COMMA;
	    	}
	        while (isdigit(token[i])) /*check digits and store in num*/
                {
            	    num *= 10;
            	    num += (token[i] - '0');
		    i++;
            	    comma_flag = 0;
                }
                if ((num > MAX_NUM(bytes))) /*check valid num in accordance with number of bytes*/ 
            	{
            	    return NUM_OUT_OF_RANGE;
            	}
	    	num = 0;
	    	comma_flag = 0;
	    }

	    if (token[i] == ',') /*checking consecutive comma*/
	    {
            	if (comma_flag)
            	{
                    return CONSECUTIVE_COMMA;
            	} 
	    	else 
	    	{
                    comma_flag = 1;
                    i++;
            	}
            } 
	    /*checking if there is any paramter that it doesn't number , comma , '+' , '-' , space , '\0' and '\n'*/
	    if(!isdigit(token[i]) && token[i] != ',' && token[i] != '+'&& token[i] != '-' && !(isspace(token[i])) && token[i] != '\0' && token[i] != '\n')
	    {
		return INVALID_PARAM;
	    }
    	}
 	token = strtok(NULL ,delim);
    }

    if (comma_flag) /*check if exist trailing comma */
        return MISPLACED_COMMA_END;
    return OK;
} /* end of number analysis */




