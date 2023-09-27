#include "dataHandler.h"

data_array * dataList;
int dataWordsCounter;/*counter how many data there is in each file*/

/*initialize data list*/
void initializeDataList()
{
    dataWordsCounter = 0;
    dataList = (data_array *)calloc(1 , sizeof(data_array));
    if(dataList ==NULL)
    {
   	printf("Error : dosn't allocated memory to dataList \n");
	exit(1);
    }

    dataList->arr = (data_line *)calloc(1,sizeof(data_line));
    if(dataList->arr ==NULL)
    {
   	printf("Error : dosn't allocated memory to dataList->arr\n");
	exit(1);
    }
    dataList->size = 0;
    dataList->DC = 0;
    dataList->curr_index = 0;

    dataList->arr->data = (int *)calloc(1,sizeof(int));
    if(dataList->arr->data ==NULL)
    {
   	printf("Error : dosn't allocated memory to dataList->arr->data\n");
	exit(1);
    }
    dataList->arr->typeByte = 0;
    dataList->arr->size = 0;
    dataList->arr->DC = 0;
    dataList->arr->address = 0;
    dataWordsCounter  = 0;
   
}


/*
*add data to the data list ,if there is not free memory than use realloc function
*DC - hold the address of the data
*numOfParm - number of paramter
*typeByte - hold the type of the byte
*parms - is a  pointer to the list of number that need to be stored
*/
void addData(int DC, int numOfParm , int typeByte ,int * parms)
{

    int i;
    dataList->arr = (data_line *)realloc(dataList->arr ,(dataWordsCounter + 1) * sizeof(data_line));
    if(dataList->arr ==NULL)
    {
   	printf("Error : dosn't allocated memory to dataList->arr\n");
	exit(1);
    }
    dataList->arr[dataWordsCounter].data = (int *)malloc( numOfParm  *sizeof(int));

    if(dataList->arr[dataWordsCounter].data ==NULL)
    {
   	printf("Error : dosn't allocated memory to dataList->arr[dataWordsCounter].data\n");
	exit(1);
    }

    dataList->arr[dataWordsCounter].typeByte = typeByte;
    dataList->arr[dataWordsCounter].DC = DC;
    dataList->arr[dataWordsCounter].address = DC;
    for(i = 0; i < numOfParm;i++)
    {
	*(dataList->arr[dataWordsCounter].data + i) = *(parms + i);
    }

    dataList->arr[dataWordsCounter].size = numOfParm;/*(numOfParm *typeByte);*/
    dataList->DC = DC ;/*DB = 1 , DH = 2 , DW = 4*/

    dataList->size += (numOfParm *typeByte);/*DB = 1 , DH = 2 , DW = 4*/
    dataWordsCounter++;


}
/*
*update the data such that the address of data will start after the code , do it with add ICF to each address of the data
*ICF - is the total code 
*/
void updateData(int ICF)
{
    int i = 0;

    for(i = 0; i < dataWordsCounter; i++)
    {
	dataList->arr[i].address += ICF;

    }
}

/*
*write all data into the file object
*fp - is pointer to the file that data need to be written
*/
void writeDataInFile(FILE* fp)
{
    unsigned int my_32bit = 0;/*stored each number*/
    int unityHexa;
    int dozensHexa;
    int k = 0;
    int l = 0;
    int s = 0;
    int count4Number = 0;
    int DC = dataList[k].arr[l].address;

    fprintf(fp, "\t\t\t");
    fprintf(fp, "0%d ",dataList[k].arr[l].address);/*print to the file the address of the each data*/

    for(k = 0; k < dataWordsCounter;k++)
    {
	if(dataList->arr[k].typeByte == 1)/*data_size == 1*/
	{
	    for(l = 0; l <dataList->arr[k].size; l++)/*each iteration for every number in the line that the data size of him is 1 byte*/
	    {
		my_32bit = 0;
	    	if(count4Number == 4)/*checking if we need to enter to the next line*/
	    	{
		    count4Number = 0;
		    DC += 4;
        	    fprintf(fp, "\n\t\t\t");
		    fprintf(fp, "0%d ",DC);
		
	    	}

		count4Number++;/*count how many numbers there have in the line */
	    	my_32bit = *(dataList->arr[k].data + l) | my_32bit;/*my_32bit = *(dataList->arr[k].data + l) */

		dozensHexa = (my_32bit & 0xF0)>>4;/*store the dozens digit of the hexa number*/
		unityHexa = (my_32bit & 0x0F);/*store the unity digit of the hexa number*/

            	fprintf(fp,"%X", dozensHexa);  /*printt to the file*/
            	fprintf(fp,"%X ", unityHexa);  
	    }
	}
	else if(dataList->arr[k].typeByte == 2)/*data_size == 2*/
	{
	    for(l = 0; l <dataList->arr[k].size; l++)
	    {
		my_32bit = 0;
	    	my_32bit = *(dataList->arr[k].data + l) | my_32bit;
		for(s= 0; s < 2; s++)
		{

		    if(count4Number == 4)/*checking if we need to enter to the next line*/
	    	    {
		    	count4Number = 0;
		    	DC += 4;
        	    	fprintf(fp, "\n\t\t\t");
		    	fprintf(fp, "0%d ",DC);
		    } 
		    count4Number++; 

		    dozensHexa = (my_32bit & 0xF0)>>4;/*taking the desired digit */
		    unityHexa = (my_32bit & 0x0F);/*taking the desired digit */

		     /*printf to the file*/
            	    fprintf(fp,"%X", dozensHexa);  
            	    fprintf(fp,"%X ", unityHexa); 
		
		    my_32bit >>= 8;
		}
	    }
	}
	else if(dataList->arr[k].typeByte == 4)
	{
	    for(l = 0; l <dataList->arr[k].size; l++)
	    {
		my_32bit = 0;
	    	my_32bit = *(dataList->arr[k].data + l) | my_32bit;/*stored the number in the variable my_32bit */
		for(s= 0; s < 4; s++)
		{
		    if(count4Number == 4)/*checking if we need new line*/
	    	    {
		    	count4Number = 0;
		    	DC += 4;
        	    	fprintf(fp, "\n\t\t\t");
		    	fprintf(fp, "0%d ",DC);
		    }   

		    dozensHexa = (my_32bit & 0xF0)>>4;/*taking the desired digit */
		    unityHexa = (my_32bit & 0x0F);/*taking the desired digit */

            	    fprintf(fp,"%X", dozensHexa);  
            	    fprintf(fp,"%X ", unityHexa); 

		    count4Number++;
		    my_32bit >>= 8;/*move ot the next part two digit */
		}

	    }

	}

    }

}

/*free data list*/
void freeDataList()
{
    /*int k;
    for (k = 0; k < dataWordsCounter;k++)
    {
        free(dataList->arr[k].data);
    }
    free(dataList->arr);*/

    free(dataList);
}




