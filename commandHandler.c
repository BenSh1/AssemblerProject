#include "commandHandler.h"


int commandWordsCounter;/*counter how many code there is in each file*/

/*initialize code list*/
void initializeCodeList()
{

    codeList = (code_array *)calloc(1 , sizeof(code_array));
    if(codeList ==NULL)
    {
   	printf("Error : dosn't allocated memory to codeList \n");
	exit(1);
    }

    codeList->arr = (code_line *)calloc(1,sizeof(code_line));
    if(codeList->arr ==NULL)
    {
   	printf("Error : dosn't allocated memory to codeList->arr\n");
	exit(1);
    }
    codeList->size = 0;
    codeList->IC = 0;
    codeList->flagKnownSymbol = 0;
    commandWordsCounter = 0;

}

/*
*add code that belong to the part of the group R command to the commmand list ,if there is not free memory than use realloc function
*IC - hold the address of the code
*opcode - hold the value of the opcode of the command
*rs - hold the value of the rs of the command
*rt - hold the value of the rt of the command
*rd - hold the value of the rd of the command
*funct - hold the value of the funct of the command
*/
void addCodeR( int IC, int opcode , int rs , int rt , int rd , int funct)
{
    codeList = (code_array *)realloc(codeList , (commandWordsCounter + 1)*sizeof(code_array));
    if(codeList ==NULL)
    {
   	printf("Error : dosn't allocated memory to codeList \n");
	exit(1);
    }

    codeList[commandWordsCounter ].arr = (code_line *)malloc(sizeof(code_line));
    if(codeList->arr ==NULL)
    {
   	printf("Error : dosn't allocated memory to codeList->arr\n");
	exit(1);
    }

    codeList[commandWordsCounter].arr->ops.r.opcode = opcode;
    codeList[commandWordsCounter].arr->ops.r.funct = funct;
    codeList[commandWordsCounter].arr->ops.r.rs = rs;
    codeList[commandWordsCounter].arr->ops.r.rt = rt;
    codeList[commandWordsCounter].arr->ops.r.rd = rd;
    codeList[commandWordsCounter].arr->ops.r.unUsed = 0;
    codeList[commandWordsCounter].IC = IC;
    codeList[commandWordsCounter].flagKnownSymbol = 0;
    codeList->size++;
    commandWordsCounter++;
}

/*
*add code that belong to the part of the group I command to the commmand list ,if there is not free memory than use realloc function
*IC - hold the address of the code
*opcode - hold the value of the opcode of the command
*rs - hold the value of the rs of the command
*rt - hold the value of the rt of the command
*immed - hold the value of the immed of the command
*flagKnownSymbol - 0 = it is indicate that we know the address of the symbol, otherwise 3 = we don't know  the address of the label so it is indicate that we will handle that in the seconde read line
*/
void addCodeI( int IC, int opcode ,  int rs , int rt , int immed , int flagKnownSymbol)
{

    codeList = (code_array *)realloc(codeList,(commandWordsCounter + 1)* sizeof(code_array));
    if(codeList ==NULL)
    {
   	printf("Error : dosn't allocated memory to codeList \n");
	exit(1);
    }

    codeList[commandWordsCounter].arr = (code_line *)malloc(sizeof(code_line));
    if(codeList->arr ==NULL)
    {
   	printf("Error : dosn't allocated memory to codeList->arr\n");
	exit(1);
    }

    codeList[commandWordsCounter].arr->ops.i.opcode = opcode;
    codeList[commandWordsCounter].arr->ops.i.rs = rs;
    codeList[commandWordsCounter].arr->ops.i.rt = rt;
    codeList[commandWordsCounter].arr->ops.i.immed = immed;/* immed; put false value if unknown otherwise put true value*/
    codeList[commandWordsCounter].IC = IC;
    codeList[commandWordsCounter].flagKnownSymbol = flagKnownSymbol;
    codeList->size++;
    commandWordsCounter++;

}

/*
*add code that belong to the part of the group J command to the commmand list ,if there is not free memory than use realloc function
*IC - hold the address of the code
*opcode - hold the value of the opcode of the command
*reg - hold the value of the reg of the command (1 = if it is register , 0 = if it is label)
*address - hold the value of the address of the command
*flagKnownSymbol - 0 = it is indicate that we know the address of the symbol, otherwise 3 = we don't know  the address of the label so it is indicate that we will handle that in the seconde read line
*/
void addCodeJ( int IC, int opcode ,  int reg , int address , int flagKnownSymbol)
{

    codeList = (code_array *)realloc(codeList,(commandWordsCounter + 1)* sizeof(code_array));
    if(codeList ==NULL)
    {
   	printf("Error : dosn't allocated memory to codeList \n");
	exit(1);
    }

    codeList[commandWordsCounter].arr = (code_line *)malloc(sizeof(code_line));
    if(codeList->arr ==NULL)
    {
   	printf("Error : dosn't allocated memory to codeList->arr\n");
	exit(1);
    }

    codeList[commandWordsCounter].arr->ops.j.opcode = opcode;
    codeList[commandWordsCounter].arr->ops.j.reg = reg;/*0 = if it is symbol , 1 = if it is register*/
    codeList[commandWordsCounter].arr->ops.j.address = address;
    codeList[commandWordsCounter].IC = IC;

    codeList[commandWordsCounter].flagKnownSymbol = flagKnownSymbol;
    codeList->size++;
    commandWordsCounter++;
}

/*
*update the code that belong to the group I , it do it with use extern function 'getLabelAddress' - we got the address of the label and subtract the current address
*IC - stored the current address that we are in the program
*label - stored the name of the label
*/
void updateCodeI(char * label, int IC)
{
    int addressOfLabel;
    int addressN;
    int  i;
    addressOfLabel = getLabelAddress(label);/*extract the value of the address of the label to the variable addressN*/
    addressN =  addressOfLabel - IC;

    for(i = 0; i < commandWordsCounter;i++)/* for find the designated code that have equal IC(address) to the argument IC*/
    {
	if(codeList[i].IC == IC )
	{
    	    codeList[i].arr->ops.i.immed = addressN;/*insert the addresN to the designated area*/
	    codeList[i].flagKnownSymbol = 1;
	}
    }

}

/*
*update the code that belong to the group J , it do it with use extern function 'getLabelAddress' - we got the address of the label 
*IC - is the current address that we are in the program
*label - is the name of the label
*/
void updateCodeJ(char * label, int IC)
{
    int addressOfLabel;
    int addressN;
    int i;
    addressN = getLabelAddress(label);/*extract the value of the address of the label to the variable addressN*/

    for(i = 0; i < commandWordsCounter;i++)/* for find the designated code that have equal IC(address) to the argument IC*/
    {
	if(codeList[i].IC == IC )
	{
    	    codeList[i].arr->ops.j.address = addressN;/*insert the addresN to the designated area*/
	    codeList[i].flagKnownSymbol = 1;
	}
    }

}

/*Write all the commands in the hexa type in the file 
*fp - is the pointer to the new file of the command object*/
int writeCommandInFile(FILE* fp)
{

    unsigned int my_32bit = 0;
    int my_halfNegative32bit = 0;
    unsigned int my_halfpositive32bit = 0;
    int unityHexa;
    int dozensHexa;
    int mask;
    int k = 0;
    int l = 0;
	/*iteration every command */
    for(k = 0; k < commandWordsCounter;k++)
    {
	my_32bit = 0;/*stored all number in the organized way in one 32 bits*/
	my_halfpositive32bit =0;/* for negative number, stored the half positive number in 16(right)bits */
	my_halfNegative32bit =0;/* for negative number ,stored the half negative number in 16(left)bits */
	/*if it is belong to the group R command */
    	if(codeList[k].arr->ops.r.opcode >= 0 && codeList[k].arr->ops.r.opcode <= 1)
    	{
	     my_32bit = (codeList[k].arr->ops.r.opcode << 26) | my_32bit;/*add to the 26-32 position the num opcode*/
    	     my_32bit = (codeList[k].arr->ops.r.rs << 21) | my_32bit;/*add to the 21-26 position the num rs*/
    	     my_32bit = (codeList[k].arr->ops.r.rt << 16) | my_32bit;/*add to the 16-21 position the num rt*/
    	     my_32bit = (codeList[k].arr->ops.r.rd << 11) | my_32bit;/*add to the 11-16 position the num rd*/
    	     my_32bit = (codeList[k].arr->ops.r.funct << 6) | my_32bit;/*add to the 6-11 position the num funct*/
    	     my_32bit = codeList[k].arr->ops.r.unUsed  | my_32bit;	/*add to the 0-6 position the num unUsed*/
 	}
	/*if it is belong to the group I command */
    	else if(codeList[k].arr->ops.i.opcode >= 10 && codeList[k].arr->ops.i.opcode <= 24)
    	{
	     /*if it is belong to the command  group I that the opcode is between 10 to 18 ,we are using that becuse of the negative number that we can get in the file*/
	     if(codeList[k].arr->ops.i.opcode >= 10 && codeList[k].arr->ops.i.opcode <= 18)
	     {
	     	my_halfpositive32bit = (codeList[k].arr->ops.i.opcode << 26) | my_halfpositive32bit;/*add to the 26-32 position the num opcode*/
    	     	my_halfpositive32bit = (codeList[k].arr->ops.i.rs << 21) | my_halfpositive32bit;/*add to the 21-26  position the num rs*/
    	     	my_halfpositive32bit = (codeList[k].arr->ops.i.rt << 16) | my_halfpositive32bit;/*add to the 16-21 position the num rt*/
    	     	my_halfNegative32bit = codeList[k].arr->ops.i.immed | my_halfNegative32bit;	/*add to the 0-16 position the num immed*/

		fprintf(fp, "\t\t\t");
         	fprintf(fp, "0%d ",codeList[k].IC );
    		for (l = 0; l < 2; l++)/*print in hexa number the half of 32 bit that can be negative number*/
    		{
            	     fprintf(fp,"%X", (my_halfNegative32bit & 0xF0) >> 4);  
            	     fprintf(fp,"%X ", my_halfNegative32bit & 0x0F);  
            	     my_halfNegative32bit >>= 8;
    		}

    		for (l = 0; l < 2; l++)/*print in hexa number the half of 32 bit that can be only positive number*/
    		{
            	     fprintf(fp,"%X", (my_halfpositive32bit & 0xF00000) >> 20);  
            	     fprintf(fp,"%X ",( my_halfpositive32bit & 0x0F0000) >> 16);       
            	     my_halfpositive32bit >>= 8;
    		}
		fprintf(fp,"\n");
		continue;
	     }
	     /*if it is belong to the command  group I that the opcode is between 19 to 24 or 10 to 14 */
	     else
	     {
		my_32bit = (codeList[k].arr->ops.i.opcode << 26) | my_32bit;/*add to the 26-32 position the num opcode*/
    	     	my_32bit = (codeList[k].arr->ops.i.rs << 21) | my_32bit;/*add to the 21-26 position the num rs*/
    	     	my_32bit = (codeList[k].arr->ops.i.rt << 16) | my_32bit;/*add to the 16-21 position the num rt*/
    	     	my_32bit = codeList[k].arr->ops.i.immed | my_32bit;/*add to the 0-16 position the num immed*/
	     }
	     
    	}
    	else if(codeList[k].arr->ops.j.opcode >= 30)
    	{
	     my_32bit = (codeList[k].arr->ops.j.opcode << 26) | my_32bit;/*add to the 26-32 position the num opcode*/
    	     my_32bit = (codeList[k].arr->ops.j.reg << 25) | my_32bit;/*add to the 25-26 position the num reg*/
    	     my_32bit = (codeList[k].arr->ops.j.address | my_32bit);/*add to the 0-25 position the num address*/
    	}

        fprintf(fp, "\t\t\t");
        fprintf(fp, "0%d ",codeList[k].IC );
    	for (l = 0; l < 4; l++)
    	{	
            fprintf(fp,"%X", (my_32bit & 0xF0) >> 4);  
            fprintf(fp,"%X ", my_32bit & 0x0F);        
            my_32bit >>= 8;
    	}
	fprintf(fp,"\n");
    }

}

void freeCommandList()
{

    int k;
    /*for (k = 0; k < codeList->size;k++)
    {
        free(codeList[k].arr);
    }*/

    free(codeList);
    
}






