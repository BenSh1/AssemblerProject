#include "first.h"

static int isErr;
int ICF , DCF;



void handle_RType_instruction(char* token,int codeNum ,int groupNum ,int IC)
{
    int rsN ,rtN ,rdN; /* holds the numbers that need to be stored in the codelist */
    int numbersFlag = 0;
    int i, num;
    char delim[]=" \t\n\r";
		if(codeNum == 0)
                {
    		     while(token)
                     {
        	      	i = 0;
    			 while (token[i] != '\0')
     			{
     				num = 0;
     				while (isspace(token[i]) || token[i] == ',')/*skip white letters*/
     				{
            				i++;
       				}
        			if (token[i] == '$')/*then it is register*/
        			{
            				i++;
            				while (isdigit(token[i]))/*stored the register number in the variable num*/
            				{
						num *= 10;
						num += (token[i] - '0');
						i++;
	    				}
	    
            				numbersFlag++;
            			}
			        if (numbersFlag == 1)/*first parmater stored in the rsN variabel */
			        {
				 rsN = num;
			        }
			        else if (numbersFlag == 2)/*second parameter stored in the rtN variabel */
			     {
				 rtN = num;
			     }
			     else if (numbersFlag == 3)/*third parameter stored in the rdN variabel */
			     {
				rdN = num;
				numbersFlag++;
			     }

			     i++;
			}

	   		token = strtok(NULL , delim);/*to next part of the same line*/
        	    }
		    addCodeR(IC, codeNum, rsN, rtN, rdN, groupNum);
                }
                else if(codeNum == 1)
                {
		    while(token)
		    {
			i = 0;
		
                    	while (token[i] != '\0')
                    	{
                            num = 0;
			    /*skip space and ','*/
                            while (isspace(token[i]) || token[i] == ',')
                            {
                            	i++;
                            }
                            if (token[i] == '$')
                            {
                            	i++;
                                while (isdigit(token[i]))/*extract number*/
                                {
                                    num *= 10;
                                    num += (token[i] - '0');
                                    i++;
                            	}
                            	numbersFlag++;
                            }
                            if (numbersFlag == 1)/*first parmater stored in the rdN variabel */
                            {
                            	rdN = num;
                            }
                            else if (numbersFlag == 2)/*seconde parmater stored in the rsN variabel */
                            {
                            	rsN = num;
                            } 
                            i++;
                    	}
			token = strtok(NULL ,delim);
		    }

                    addCodeR(IC, codeNum , rsN, 0, rdN, groupNum);
		}
    

}

void handle_IType_instruction(char* token,int codeNum ,int groupNum ,int IC)
{
    int rsN ,rtN , immedN; /* holds the numbers that need to be stored in the codelist */
    int numbersFlag = 0;
    int i, num,isMinus;
    char delim[]=" \t\n\r";
                if( (codeNum >= 10 && codeNum <= 14) || (codeNum >= 19 && codeNum <= 24))
                {
		    while(token)
		    {
			i = 0;
			while (token[i] != '\0')
                    	{
                            num = 0;
			    isMinus = 0;
			    /*skip space  , ',' , '$' , and '+' */
                            while (isspace(token[i]) || token[i] == ',' || token[i] == '$' || token[i] == '+') 
                            {
                            	i++;
                            }

			    if(token[i] == '-')
			    {
			    	isMinus = 1;/*it is minus number*/
			    	i++;
			    }

	                    while (isdigit(token[i]))/*extract number*/
                            {
                            	num *= 10;
                            	num += (token[i] - '0');
                            	i++;
                            }
			    numbersFlag++;

			    if(isMinus)/*if true it is minus number then we multiply the number with -1*/
			    {
			    	 num = (-1) *num;
			    }

                            if (numbersFlag == 1)/*first parameter stored in the rsN variabel*/
                            { 
                            	rsN = num;
                            }
                            else if (numbersFlag == 2)/*second parameter stored in the immedN variable*/
                            {
                            	immedN = num;

                            }
                            else if (numbersFlag == 3)/*third parameter stored in the rtN variable*/
                            {
                            	rtN = num;
                            }
                            i++;

                        }
                    
			token = strtok(NULL, delim);
		    }
		    addCodeI(IC, codeNum, rsN, rtN, immedN, 0);

                }
		else if(codeNum >= 15 && codeNum <= 18)
		{
		    while(token)
		    {
			i = 0;
                    	while (token[i] != '\0')
                    	{
                            num = 0;
			    /*skip space  , ',' , '$' , and abcd.. */
                            while (isspace(token[i]) || token[i] == ',' || token[i] == '$' || isalpha(token[i])) 
                            {
                            	i++;
                            }
			    if(isdigit(token[i]))
			    {
	                    	while (isdigit(token[i]))/*extract number */
                             	{
                            	    num *= 10;
                            	    num += (token[i] - '0');
                            	    i++;
                            	}
			    	numbersFlag++;
			    }

                            if (numbersFlag == 1)/*first parameter stored in the rsN variable*/
                            {
                            	rsN = num;
                            }
                            else if (numbersFlag == 2)/*seconde parameter stored in the rtN variable*/
                            {
                            	rtN = num;
			    	numbersFlag++;
                            }

                            i++;
                    	}
                     	token = strtok(NULL, delim);
		    }
                    addCodeI(IC, codeNum, rsN, rtN, 0, 3);
		}
}


void handle_JType_instruction(char* token,int codeNum ,int groupNum ,int IC){
	int regN , addressN;/* holds the numbers that need to be stored in the codelist */
	    
	int num, i;
    	char delim[]=" \t\n\r";
    	

		if(codeNum == 30)
		{
		    while(token)
		    {
			i = 0;
                    	while (token[i] != '\0')
                    	{
                            num = 0;

                            while (isspace(token[i]) || token[i] == ',' ) /*skip white letters*/
                            {
                            	i++;
                            }

                            if (token[i] == '$')
                            {
                            	i++;
                                while (isdigit(token[i]))/*extract number */
                                {
                                    num *= 10;
                                    num += (token[i] - '0');
                                    i++;
                                }
			        regN = 1;/*it is register*/
			        addressN = num;
                            }
		    
                            i++;
                        }
                     	token = strtok(NULL, delim);
		    }

		    if(regN == 1)/*if it is register*/
		    {
		    	addCodeJ(IC, codeNum, regN, addressN, 0);
		    }

		    else/*then it is label , and we will handle the address in the seconde file */
		    {
			regN = 0;
			addressN = 0;
		    	addCodeJ(IC, codeNum, regN, 0, 3);
		    }

		}


		else if(codeNum == 31 || codeNum == 32 )
		{
 
		    regN = 0;
		    addressN = 0;
		    addCodeJ(IC, codeNum, regN, addressN, 3);
		}


}

void handle_directive_sentence(char* token ,int* ptrToDC , int * size , int* parms){
	int i, data_size, num;
    	char delim[]=" \t\n\r";
	int numOfParm = 0;/*stored the number of paramters in each line (only the lines from the type direcative )*/
	int isMinus;/*indicate if the number (with minus) little than zero*/
	    
	    /*if it is .ascii directive*/
	    if(!strcmp(token,".asciz") )
	    {
		data_size = DB;/*DB = 1*/
		token = strtok(NULL, delim);/*move to the next word in the line*/

		/* skip space at*/
		while(isspace(token[i]))
		{
			i++;
		}
		/* skip " */
	    	if(token[i] == '"')
	    	{
		    i++;
		    /* stored all letters in the line between " "  */
		    while( isalpha(token[i]))
		    {
			if(numOfParm >= *size)
			{
			    *size += 1;/*size of the variable parms */
			    parms = (int *)realloc(parms , *size * sizeof(int));
			    if(parms == NULL)
			    {
   				printf("Error : dosn't allocated memory to parms\n");
				exit(1);
			    }
			}
			
			num = token[i];/*save the decimal value of the letter */
			i++;
			parms[numOfParm] = num;/*we stored the letter with the value decimal ascii */
			numOfParm++;/*to indicate how many numbers do we have until now */
		    }

		    /*to handle situation of no more space at the end because we are planning to add zero for '\0' , we will use realloc to add 1 memory to the variable "params"*/
		    if(numOfParm == *size)
		    {
			*size += 1;
			parms = (int *)realloc(parms , *size * sizeof(int));
			if(parms == NULL)
			{
   			     printf("Error : dosn't allocated memory to parms\n");
			     exit(1);
			}
		   }

		   /*add the ('\0')zero to the "parms"*/
		   parms[numOfParm] = 0;
		   numOfParm++;/*add one for the the char '\0' */

	    	}

	    }
	    else if(!strcmp(token,".db") || !strcmp(token,".dh") || !strcmp(token,".dw"))
	    {
		/*checking which data size is it (DB = 1 , DH = 2, DW = 4)*/
      		switch (token[strlen(token)-1]) 
        	{
            	    case 'b': data_size = DB; break;
            	    case 'h': data_size = DH; break;
            	    case 'w': data_size = DW; break;
        	}

		/*move to the next word*/
		token = strtok(NULL, delim);

		while(token)
		{
		    i = 0;
		    while(token[i] != '\0')
		    {
		      	num = 0;
		    	isMinus = 0;/*flag to indicate if there is minus in the number */

		    	/*skip space in the line*/
		    	while(isspace(token[i]))
		    	{
			    i++;
		    	}
		    	/*skip ','  skip '+'  and skip '-'*/
		    	if(token[i] == ',')
		    	{
			    i++;
		    	}
		    	if(token[i] == '-')
		    	{
			    isMinus = 1;/*indicate that there is minus */
			    i++;
		    	}
		    	if(token[i] == '+')
		    	{
			    i++;
		    	}

		    	if(isdigit(token[i]))
	    	    	{
		    	     if(numOfParm >= *size)/*checking if the number of parameter until now is lower than size of that allocated to parms number if it is equal then allocate 1 more memory to the size of the parms variable*/
		    	     {
			     	*size += 1;
			     	parms = (int *)realloc(parms , *size * sizeof(int));
			     	if(parms == NULL)
			     	{
   			     	     printf("Error : dosn't allocated memory to parms\n");
			     	     exit(1);
			     	}
		   	     }			

			     while(isdigit(token[i]))/*save one number at a time in the variable num */	
			     {
			     	num *=  10;
			     	num += (token[i] - '0');
			     	i++;
			     }
			     if(isMinus)/*if it is true then it is minus number so we mul the number with -1 */
			     {
			         num = (-1) *num;
			     }
		  	     parms[numOfParm] =  num;
			     
			     numOfParm++;
			     num = 0;
			
		    	}

		     }

		     token = strtok(NULL ,delim);
		}
	    }

	    addData(*ptrToDC,numOfParm , data_size ,parms);
	    *ptrToDC += (numOfParm*data_size);
}


int firstReadLine(FILE *fp ,const char * file_n)
{
    int labelFlag = 0 , entryFlag = 0 , externalFlag = 0 , dataFlag = 0 , codeFlag =0;/*all flags in the file*/
    int numbersFlag;/* it is indicate in which number we are in the line , for example it have the 2 number so we at the second number in line*/
    int *parms; /*stored the data*/

    char original_line[MAX_LINE_LEN];/*stored the original line in each iteration , MAX_LINE_LEN = 80*/
    char temp_line[MAX_LINE_LEN]; 
    char *token;
    char delim[]=" \t\n\r";
    char *label;/*stored the name of the label*/

    int DC = INITIALIZE_DC;
    int IC = INITIALIZE_IC; /*initialize the ic variable to 100*/

    int line_num = 0; /*stored the number of the line in each iteration*/
    int error_num = 0; /*stored the number of the error */

    int i =0; 
    int size = 2;


    struct opCodes tmp;/*hold the code in the tables code (inside variables.c file)*/

    /*for stored the name of the label for each iteration*/
    label = (char*)malloc(32 * sizeof(char));
    if(label == NULL)
    {
	printf("Error : dosn't allocated memory to label list\n");
	exit(1);
    }

    /*for stored the parmeter of the data for each iteration*/
    parms = (int*)malloc(size * sizeof(int));
    if(parms == NULL)
    {
	printf("Error : dosn't allocated memory to parms \n");
	exit(1);
    }

    initializelabelList();/*initialize lable list */
    initializeCodeList();/*initialize code list */
    initializeDataList();/*initialize data list */

    while (fgets(original_line, MAX_LINE_LEN, fp))
    {
	/*flag for every attribute : external , entry , label ,data ,code 
	  and initialize them to zero each iteration*/
	externalFlag = 0;
	entryFlag = 0;
        labelFlag = 0;
	dataFlag = 0;
	codeFlag =0;


        line_num++;

        /*returns 0 if it is not comment or empty line, and 1 otherwise*/
        if (is_not_comment_or_empty_line(original_line))
        {
            continue;
        }

	/*copy the original line to the variable temp_line*/
        strcpy(temp_line, original_line);
        token = strtok(temp_line, delim);

        /*returns 0 if it is a label, and 1 otherwise*/
        if (!is_label(token, 1))
        {
	    /*stored the name label in the variable 'label'*/
            strcpy(label ,token);
	    /*checking if there is not content after label , if it is true then retrun error*/
            if ((token = strtok(NULL, delim)) == NULL)
            {
		isErr = 1;
                error_num = LABEL_WITHOUT_DIRECTIVE_OR_INSTRUCTION;
                print_error(error_num, file_n, line_num, token);/*send to extern function to handle the error */
                continue;
            }

            labelFlag = 1;
        }
	 
        /*checking if it is DIRECTIVE word*/
 	if (!(strcmp(token, ".dh")) || !(strcmp(token, ".dw")) || !(strcmp(token, ".db")) || !(strcmp(token, ".asciz")))
        {
	    dataFlag = 1;

	    i = 0;
	    
	    handle_directive_sentence(token, &DC, &size , parms);

	    if(labelFlag)
	    {
		addLabel(label , DC , dataFlag,codeFlag , externalFlag,entryFlag);
	    }

	    
	    continue;

        }

	else if (!strcmp(token, ".entry"))/*if it is entry we will handle it in the seconde.c file*/
	{
	    continue;
	}

	 
        else if (!strcmp(token, ".extern"))
        {
	    externalFlag = 1;

	    if ((token = strtok(NULL, delim)) == NULL)/*checking if there is any parmater after the word extern if it isn't so it is error*/
            {
		isErr = 1;
                error_num = MISSING_PARAM;
                print_error(error_num, file_n, line_num,NULL);
                continue;
            }

	    else
	    {
		if (!is_label(token, 0))/*checking if it is valid label*/
		{

		    if(isDefinedLabel(token))/*checking if the word is defined if the word already defined then it is error*/
		    {
			isErr = 1;
                	error_num = LABEL_ALREADY_DEFINED;
                	print_error(error_num, file_n, line_num,token);
		    }
		    else
		    {
			/*add to the label list*/
		    	addLabel(token , 0 , dataFlag,codeFlag , externalFlag,entryFlag);
			continue;
		    }
		}
		else /*not valid number then print error*/
		{
		    isErr = 1;
		    error_num = LABEL_INVALID;
                    print_error(error_num, file_n, line_num,token);
                    continue;
		}
	    }

	}
        else if (!IS_INSTRUCTION(token))
        {
	    codeFlag =1;

            for (i = 0; i < NUM_OF_COMMANDS; i++)/*searching after which code name is it in the list command */
            {
                if (strcmp(token, getOpCodes()[i].name) == 0)
                {
                    tmp = getOpCodes()[i];
                }
            }


 	    if( !strcmp(token, "stop"))/*if it is stop command then add label if there is a label and add code */
	    {
		if(labelFlag)
		{
		    addLabel(label , IC  , dataFlag,codeFlag , externalFlag,entryFlag);
		}

		addCodeJ(IC, tmp.code, 0, 0, 0);
		
		IC += 4;
	
		continue;
   	    }

	    token = strtok(NULL, delim);
	    i = 0;


	    if (tmp.commandGroup == 'R')
            {
           
		handle_RType_instruction(token , tmp.code, tmp.group , IC);
		
            }
            else if (tmp.commandGroup == 'I')
            {
		handle_IType_instruction(token , tmp.code, tmp.group , IC);

            }

	    else if (tmp.commandGroup == 'J')
	    {
	    	handle_JType_instruction(token , tmp.code,tmp.group, IC);

	    }

	    if(labelFlag)/*if the label flag is on we will add the label to the label list */
	    {
		addLabel(label , IC , dataFlag,codeFlag , externalFlag,entryFlag);
	    }
	    
	    IC = IC + 4;
	    
        }
	else/*it is not instruction and not direcative then it is error*/
	{
	    isErr = 1;
            print_error(NAME_FUNCTION_UNRECOGNIZE, file_n, line_num, token);
            continue;
	}

    }
    free(label);
    free(parms);

    if(isErr != 1)/*if there isn't any error then update all the address of the labels*/
    {
    	ICF = IC;
    	DCF = DC;

    	updateLabels(ICF);
    	updateData(ICF);
    }


    return isErr;/*it is indicate if there is found errors in the file */
    
}








