#include "seconde.h"


extern int labelsCounter;

extern int ICF,DCF;

/*stord the list of the all external symbol in command of j*/
typedef struct external_array{
    char name[32];
    int address; 
}external_array;

external_array * externalList;
int counterExternalWord;

/*initialize the pointer external_Array*/
void initializeExternalList()
{
    externalList = (struct external_array * )malloc(sizeof(external_array));
    if(externalList ==NULL)
    {
   	printf("Error : dosn't allocated memory to codeList \n");
	exit(1);
    } 
    counterExternalWord = 0;

}
/*add label that defined as external label 
*lable - is the name of the label that we want to add
*address - is the address of the label that we want to add*/
void addExtern(char* label , int address)
{
    externalList = (struct external_array * )realloc( externalList, (counterExternalWord+ 1)* sizeof(external_array));
    if(externalList ==NULL)
    {
   	printf("Error : dosn't allocated memory to codeList \n");
	exit(1);
    } 
    strcpy(externalList[counterExternalWord].name ,label);
    externalList[counterExternalWord].address = address;
    counterExternalWord++;
}



/*Second read line and if there isn't errors when writing the output files */
void secondeReadLine(FILE *fp ,const char * file_n)
{
    int  entryFlag = 0, labelFlag = 0; 
    int data_size = 0;/*stored the type of the data size (db , dh ,dw)*/

    int numOfParm = 0;/* stored the number of paramter*/
    int *parms; /*stored the data*/

    char original_line[MAX_LINE_LEN];/*stored the line*/
    char temp_line[MAX_LINE_LEN];
    char *token;
    char delim[]=" \t\n\r";
    char *label;/*stored the label of the line(if exist label in the line)*/

    int DC = INITIALIZE_DC;
    int IC = INITIALIZE_IC;

    int line_num = 0;
    int error_num = 0; 

    int i =0;
    int size = 2;

    int rsN ,rtN ,rdN  , immedN, regN = 0;
    struct opCodes tmp;

    initializeExternalList();

    /*for stored the name of the label for each iteration*/
    label = (char*)malloc(32 * sizeof(char));
    if(label == NULL)
    {
	error_num = 1;
	printf("Error : dosn't allocated memory to label list\n");
	exit(1);
    }

    while (fgets(original_line, 80, fp))
    {
        line_num++;
        /*returns 0 if it is not comment or empty line, and 1 otherwise*/
        if (is_not_comment_or_empty_line(original_line))
        {
            continue;
        }

        strcpy(temp_line, original_line);
        token = strtok(temp_line, delim);

        if (!strcmp(token, ".entry"))
        {
	    entryFlag = 1;
	    /*if there is not any paramter after 'entry' then it is error missing paramter */
	    if ((token = strtok(NULL, delim)) == NULL)
            {
                error_num = 1;
                print_error(MISSING_PARAM, file_n, line_num,NULL);
                continue;
            }
	    else
	    {
		if (!is_label(token, 0))
		{
		    labelFlag = 1;
		    if(isDefinedLabel(token))/*checking if there is already label  with that name , if it is true then update the label with that name with entry flag(=1) */
		    {
		    	setEntry(token,line_num);
		    }
		    else/*there is not label with that name in the file*/
		    {
		    	error_num = 1;
                    	print_error(SET_ENTRY_TO_LABEL_UNDEFINED, file_n, line_num,token);
                    	continue;
		    }
		}
		else/*if it is not valid label */
		{
		    error_num = 1;
                    print_error(LABEL_INVALID, file_n, line_num,token);
                    continue;
		}

	    }
        }

        /*skip the label*/
        if (!is_label(token, 1))
        {
            token = strtok(NULL, delim);
        }

	if (!IS_DIRECTIVE(token))
	{
	    numOfParm = 0;

	    if(!strcmp(token,".db") || !strcmp(token,".dh") || !strcmp(token,".dw"))
	    {
      		switch (token[strlen(token)-1]) /*find the type of the data */
        	{
            	    case 'b': data_size = DB; break;
            	    case 'h': data_size = DH; break;
            	    case 'w': data_size = DW; break;
        	}
	    }
	    else if(!strcmp(token,".asciz"))
	    {
		data_size = DB;
	    }

	    while(token[i] != '\0')
	    {
		while(isspace(token[i]))
		{
		    i++;
		}
		if(isdigit(token[i]))
		{
		    while(isdigit(token[i]))
		    {
			i++;
		    }
	    	    numOfParm++;/*count how many parameter*/
		}

		i++;
	    }

	    DC = (numOfParm*data_size);/*update the DC counter with number of parameter multiply with the type of the data size*/
	    
	    continue;
	}
	/*if it isn't instruction then continue to the next line*/
	if(IS_INSTRUCTION(token))
	{
	    continue;
	}
	if( !strcmp(token , "stop"))/*if it "stop" instruction then continue to the next line*/
	{
	    IC += 4;
	    continue;
	}

	/*it is not directive so it must be instuction */
	/*find the name of the opcode*/
        for (i = 0; i < 27; i++)
        {
            if (strcmp(token, getOpCodes()[i].name) == 0)
            {
             	tmp = getOpCodes()[i];
            }
        }
	/*skip the code*/
        token = strtok(NULL, delim);

	/*in this section of the code we are complete the binary code of the instruction command that related to the I group*/

	if (tmp.commandGroup == 'I')
	{
	    if(tmp.code >= 15 && tmp.code <= 18)
	    {
		strcpy(temp_line, original_line);
		/*the label will be after 3 comma so we will skip 3 comma*/
             	token = strtok(temp_line, delim);
		token  = strtok(NULL , ",");
 	        token  = strtok(NULL , ",");
		token  = strtok(NULL , ",");
		remove_spaces(token);/*after 3 comma may be space so we will remove space for to be with only the name of the label without space */

		if(isDefinedLabel(token))/*checking if there is exist label with that name in the data of the labels*/
    		{
		     updateCodeI(token ,IC);  
		}
		else
    		{
		     printf("Error: line %d, label parameter wasent declared\n", line_num);
    		}
	    }
	}

	/*in this section of the code we are complete the binary code of the instruction command that related to the J group*/
	else if (tmp.commandGroup == 'J')
	{
	    if(tmp.code == 30 )
	    {
		i= 0;
		regN = 0;

		while(token[i] != '\0')
		{
		    if(isdigit(token[i]))/*checking if is gonna be register in the line*/
		    {
			regN = 1;
		    }
		    i++;
		}
		if(regN == 0 )/*if we don't find register so it must be label*/
		{
		    if(isDefinedLabel(token))/*checking if there is exist label with that name in the data of the labels*/
    		    {
		    	updateCodeJ(token ,IC);  

			if(isExternal(token))/*this statement is for storage the external label in the output file of external labels */
			{
			    addExtern(token,IC);
			}
		    }
		    else
    		    {
		     	printf("Error: line %d, label \"%s\" parameter was not declared\n", line_num,token );
    		    }

		}
	    }

	    else if( tmp.code == 31 || tmp.code == 32)
	    {

		if(isDefinedLabel(token))/*checking if there is exist label with that name in the data of the labels*/
    		{
		     updateCodeJ(token ,IC);  
		     if(isExternal(token))/*this statement is for storage the external label in the output file of external labels */
		     {
			addExtern(token,IC);
		     }
		}
		else
    		{
		     printf("Error: line %d, label \"%s\" parameter was not declared\n", line_num,token );
    		}

	    }
	}

	IC += 4;
    }

    /*if we got to here so there is no error in the file , and now we can create all files*/
    if(error_num == 0)
    {
	FILE *fp_Obj;/*pointer for object file */
	FILE *fp_Ent;/*pointer for entry file */
	FILE *fp_Ext;/*pointer for external file */
	char *fn_Obj;/*stored the name of object file*/
	char *fn_Ext;/*stored the name of external file*/
	char *fn_Ent;/*stored the name of entry file*/

        fn_Obj =(char *)calloc(strlen(file_n),sizeof(char));
	if(fn_Obj ==NULL)
    	{
   	    printf("Error : dosn't allocated memory to fn_Obj \n");
	    exit(1);
    	}

	strncpy(fn_Obj , file_n , strlen(file_n) - 3);/*copy the name , only the name without ".as" */
	strcat(fn_Obj,".ob");

	fp_Obj = fopen (fn_Obj, "w+");
	if(!fp_Obj)
        {
            printf("Error: could not create file %s \n", fn_Obj);
	}

 	fprintf(fp_Obj , "\t\t\t     %d %d\n",(ICF - 100) ,DCF);/*write in the head of the file the total length of the instruction pictures and the total length of the directive pictures  */

	if(ICF > 100)/*checking if there is at least one code command line */
	    writeCommandInFile(fp_Obj);/*write all code in an hexa way number in the file*/

	if(DCF > 0)/*checking if there is at least one data command line*/
	    writeDataInFile(fp_Obj);/*write all data in an hexa way number in the file */

	free(fn_Obj);
	fclose(fp_Obj);

	/*need to check if there is at lesat one label that defined as external label....*/
	if(isExternalExist())
	{
	    fn_Ext =(char *)calloc(strlen(file_n) ,sizeof(char));
	    if(fn_Ext ==NULL)
    	    {
   	    	printf("Error : dosn't allocated memory to fn_Ext \n");
	     	exit(1);
    	    }

	    strncpy(fn_Ext , file_n, strlen(file_n) - 3);/*copy without ".as"*/
	    strcat(fn_Ext,".ext");

	    fp_Ext = fopen (fn_Ext, "w+");
	    if(!fn_Ext)
            {
            	printf("Error: could not create file %s \n", fn_Ext);
	    }

	    /*write external label name and the address of the labl in the file*/
	    for(i = 0 ; i < counterExternalWord;i++)
	    {
		fprintf(fp_Ext ,"%s 0%d\n",externalList[i].name , externalList[i].address);
	    }

	    free(fn_Ext);
	    fclose(fp_Ext);
	}

	/*need to check if there is at lesat one label that defined as entry label....*/
	if(isEntryExist())
	{
            fn_Ent =(char *)calloc(strlen(file_n) ,sizeof(char));
	    if(fn_Ent ==NULL)
    	    {
   	    	printf("Error : dosn't allocated memory to fn_Ent \n");
	    	exit(1);
    	    }
	    strncpy(fn_Ent , file_n, strlen(file_n) - 3);
	    strcat(fn_Ent,".ent");

	    fp_Ent = fopen (fn_Ent, "w+");
	    if(!fp_Ent)
            {
            	printf("Error: could not create file %s \n", fn_Ent);
	    }

	    writeEntryInFile(fp_Ent);/*write all the label that defined as entry label and the address of each label that  have that attribute */

	    free(fn_Ent);
	    fclose(fp_Ent);
	}
    }

    free(externalList);
    
}



