#include "labelHandler.h"

int labelsCounter;/*counter how many labels there is in each file*/

/*initialize label list*/
void initializelabelList()
{
    labelsCounter = 0;
    labelList = (struct label *)calloc(1 , sizeof(struct label));
    if(labelList ==NULL)
    {
   	printf("Error : dosn't allocated memory to label list\n");
	exit(1);
    }
    labelList->name =NULL;
    labelList->address =0;
    labelList->data =0;
    labelList->code =0;
    labelList->entry =0;
    labelList->external =0;
    labelList->size =0;
}

/*
*add label to the label list ,if there is not free memory than use realloc function
*label - name of the label
*address - hold the address of the label
*data - flag 1 = if label has data attribute ,  0 = if label has not data attribute
*code - flag 1 = if label has code attribute ,  0 = if label has not code attribute
*external - flag 1 = if label has external attribute ,  0 = if label has not external attribute
*data - flag 1 = if label has entry attribute ,  0 = if label has not entry attribute
*/
void addLabel(char *label,int address, unsigned int data , unsigned int code,unsigned int external,unsigned int entry)
{

    char *pureLabel = label;/*stored the name of the label*/
    pureLabel = subString(0, strlen(label) - 1, label );/*remove the ':' at the end of the labels */
    
    labelList = (struct label *)realloc(labelList, (labelsCounter + 1) * sizeof(struct label));
    if(labelList == NULL)
    {
   	printf("Error : dosn't allocated memory to label list\n");
	exit(1);
    }

    labelList[labelsCounter].name = (char *)malloc(31*sizeof(char));
    if(labelList[labelsCounter].name ==NULL)
    {
   	printf("Error : dosn't allocated memory to label name\n");
	exit(1);
    }

    labelList[labelsCounter].name = pureLabel;
    labelList[labelsCounter].address = address;
    labelList[labelsCounter].data = data;
    labelList[labelsCounter].code = code;
    labelList[labelsCounter].external = external;
    labelList[labelsCounter].entry = entry;
    labelsCounter++;

    labelList[labelsCounter].size = labelsCounter;
}

/*return 1 if the string(purelabel) is already defined label , otherwise returen 0.
*purelabel - is the string we want to check if it already defined label*/
int isDefinedLabel(char *purelabel)
{
    int i = 0;
    int j = 0;

    while(purelabel[j] != '\0')
    {
	j++;
    }
    for (i = 0; i < labelsCounter; i++)
    {

        if (!strncmp(labelList[i].name, purelabel , j - 1))
        {
            return 1;
        }
    }
    return 0;
}

/*return 1 if it is an exist symbol
token - is the label that we want to check if it has an external attribue  */
int isExternal(char * token)
{
    int i;

    for (i = 0; i < labelsCounter; i++)
    {
	if(!strcmp(labelList[i].name , token))
	{
	    if (labelList[i].external == 1)
            {
            	return 1;
            }
	}

    }
    return 0;
}



/*return 1 if exist at least one external label , otherwise return 0*/
int isExternalExist()
{
    int i = 0;

    for (i = 0; i < labelsCounter; i++)
    {
        if (labelList[i].external == 1)
        {
            return 1;
        }
    }
    return 0;
}

/*return 1 if exist at least one entry label */
int isEntryExist()
{
    int i = 0;


    for (i = 0; i < labelsCounter; i++)
    {
        if (labelList[i].entry == 1)
        {
            return 1;
        }
    }
    return 0;
}
/*Write all the labels that have an entry feature in the new file we create for entry labels
*fp - is the pointer to the new file of the entry labels */
void writeEntryInFile(FILE* fp)
{
    int i = 0;
    for (i = 0; i < labelsCounter; i++)
    {
        if (labelList[i].entry == 1)
        {
            fprintf(fp , "%s  0%d\n" , labelList[i].name ,labelList[i].address);
        }
    }
}

/*get address of specified label */
int getLabelAddress(char *label)
{
    int i;
    int j = 0;

    while(label[j] != '\0')
    {
	j++;
    }
    for (i = 0; i < labelsCounter; i++)
    {
	/*checking how many char there is in the name of the label if it is 1 char so use function strcmp , otherwise use strncmp for remove the char ':'*/
	if(strlen(label) == 1)
   	{
	     if (!strcmp(labelList[i].name, label))
            {
            	return labelList[i].address;
            }
    	}
        else if (!strncmp(labelList[i].name, label , j - 1))
        {
            return labelList[i].address;
        }
    }
    return 0;
}
/*return the index of the label from the labelList
* label - is the name of the label from program*/
int getLabelIndex(char * label)
{
    int i;
    int j = 0;

    while(label[j] != '\0')
    {
	j++;
    }
    for (i = 0; i < labelsCounter; i++)
    {
        if (!strncmp(labelList[i].name, label , j - 1))
        {
            return i;
        }
    }
}


/*set label to entry, if label not exist or label is entry print the error and change error to ERROR, return the address of the label and ERROR if occurred  , return 1 if there is error*/
void setEntry(char *label, int lineNum)
{
    int i;

    for (i = 0; i < labelsCounter; i++)
    {
        if (!strcmp(labelList[i].name, label))
        {
            /*internal lablel cant be external*/
            if (labelList[i].external == 1)
            {
                printf("Error, line %d, label cannot be external and entry\n", lineNum);
            }
            else
            {
                labelList[i].entry = 1;

            }
        }
    }

}


/*set label to extern, if label not exist or label is entry print the error and change error to ERROR, return the address of the label and ERROR if occurred  , return 1 if there is error*/
int setExtern(char *label, int lineNum)
{
    int err = 0;
    int i;
    for (i = 0; i < labelsCounter; i++)
    {
        if (strcmp(labelList[i].name, label) == 0)
        {
            /*internal lablel cant be external*/
            if (labelList[i].entry == 1)
            {
		err = 1;
                printf("Error, line %d, label cannot be external and entry\n", lineNum);
                return err;
            }
            else
            {
                labelList[i].external = 1;
                return labelList[i].address;
            }
        }
    }
    err = 1;
    printf("Error: line %d, entry label parameter wasent declared\n", lineNum);
    return err;
}

/*after the first readlines , use the function for update (with add ICF  to every label address)the address of all the lables*/
void updateLabels(int ICF)
{
    int i = 0;

    for (i = 0; i < labelsCounter; i++)
    {
	if(labelList[i].data == 1)
	{
            labelList[i].address += ICF;
	}

    }
}


    /*free the dynamic allocation of the label list*/
void freeSymbolTable()
{
    /*int k;
    for (k = 0; k < labelsCounter;k++)
    {
        free(labelList[k].name );
    }*/
    free(labelList);
}







