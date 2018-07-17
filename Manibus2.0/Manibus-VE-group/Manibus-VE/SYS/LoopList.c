#include "LoopList.h"
#include "stdlib.h"
#include "string.h"


/******************************
*Initlize your looplist Buffer.
*Input:
*Output:
******************************/

LP_ERR LoopList_Init(LoopListStruct *NewStruct,const unsigned NewBufferSize)
{
	if (NewStruct<=0)
	{
		return LP_POINTERR;
	}
	NewStruct->BufferSize=NewBufferSize;
	NewStruct->pData=(unsigned char *)malloc(sizeof(unsigned char) * NewStruct->BufferSize);
	memset(NewStruct->pData,0,NewStruct->BufferSize);//它的作用是在一段内存块中填充一个给定的值，它是对较大的结构体或者数组进行清零操作的一种最快方法.
	NewStruct->ReadPoint=0;
	NewStruct->WritePoint=0;
	NewStruct->DataSize=0;
	return LP_NOERR;
}
/*******************************
*Put a Data Into LoopListBuffer.
*Input:
*Output:
*******************************/
LP_ERR LoopList_AddData(LoopListStruct *NewStruct,const unsigned char NewData)
{
	if (NewStruct<=0)
	{
		return LP_POINTERR;
	}
		NewStruct->pData[NewStruct->WritePoint]=NewData;
	if (NewStruct->WritePoint==NewStruct->ReadPoint)
	{
		NewStruct->DataSize=0;
	}
	NewStruct->WritePoint++;  
	NewStruct->DataSize++;    
	if (NewStruct->WritePoint==NewStruct->BufferSize)
	{
		NewStruct->WritePoint=0;
	}
	return LP_NOERR;
}
/*******************************
*Put many Dataes Into LoopListBuffer.
*Input:
*Output:
*Add in 2018/3/11 by JunWenCui
*******************************/
LP_ERR LoopList_AddMultiData(LoopListStruct *NewStruct,const unsigned char *NewData,unsigned short length){
	
	unsigned short data_length;
	
	if (NewStruct<=0)
	{
		return LP_POINTERR;
	}
   
	for(data_length = 0;data_length <length ;data_length ++){
		
	    NewStruct->pData[NewStruct->WritePoint]=NewData[data_length];
		
		if (NewStruct->WritePoint==NewStruct->ReadPoint)
		{
			NewStruct->DataSize=0;
		}
	 	  NewStruct->WritePoint++;  
	    NewStruct->DataSize++; 
		
		if (NewStruct->WritePoint==NewStruct->BufferSize)
		{
			NewStruct->WritePoint=0;
		}
	}
	  return LP_NOERR;
}

/*******************************
*Get a Data From LoopListBuffer.
*Input:
*Output:
*******************************/
LP_ERR LoopList_GetOneData(LoopListStruct *NewStruct,unsigned char *pCh)
{
	if (NewStruct<=0)
	{
		return LP_POINTERR;
	}
	while (NewStruct->DataSize==0)    //这里等待数据进来  已修改
	{
		//*pCh=0;
		//return LP_NODATA;
	}
	*pCh=NewStruct->pData[NewStruct->ReadPoint];
	NewStruct->ReadPoint++;
	NewStruct->DataSize--;
	if (NewStruct->ReadPoint==NewStruct->BufferSize)
	{
		NewStruct->ReadPoint=0;
	}
	return LP_NOERR;
}
/*******************************
*Get multi data from LoopListBuffer.
*Input:
*Output:
*******************************/
LP_ERR LoopList_GetMultiData(LoopListStruct *NewStruct,unsigned char *pCh,const unsigned short Size,
															const unsigned char DataType)
{
	unsigned short tmp=Size;
	if (NewStruct<=0||pCh<=0)
	{
		return LP_POINTERR;
	}

	while (tmp>NewStruct->DataSize)             //这里已修改 等待数据进入
	{
		//return LP_DATANOTENOPH;
	}
	if(DataType==BIGLIN)
	{
		pCh=pCh+Size-1;
		while(tmp)
		{
			LoopList_GetOneData(NewStruct,pCh--);
			tmp--;
		}
	}
	else if(DataType==LITTERLIN)
	{
		while(tmp)
		{
			LoopList_GetOneData(NewStruct,pCh++);
			tmp--;
		}
	}
	return LP_NOERR;
}
/*******************************
*Delete a LoopListBuffer.
*Input:
*Output:
*******************************/
LP_ERR LoopList_Delete(LoopListStruct *NewStruct)
{
	if (NewStruct==NULL)
	{
		return LP_NOERR;
	}
	free(NewStruct->pData);
	NewStruct->pData=NULL;
	return LP_NOERR;
}
/*******************************
*Feed data back to LoopListBuffer.
*Input:
*Output:
*******************************/
LP_ERR LoopList_FeedBack(LoopListStruct *NewStruct,unsigned short read_mark,unsigned short size_mark){

		if (NewStruct==NULL)
	{
		return LP_NOERR;
	}
	
   NewStruct->ReadPoint = read_mark;
	 NewStruct->DataSize = size_mark;
	
	 return LP_NOERR;
}
/*******************************
*Mark LoopListBuffer.
*Input:
*Output:
*******************************/
LoopList_ReadFeature LoopList_Marked(LoopListStruct *NewStruct){
	
	LoopList_ReadFeature *marked;
	
		if (NewStruct==NULL)
	{
		marked->State = LP_NOERR;
	}
	marked->ReadPoint = NewStruct->ReadPoint;
	marked->DataSize = NewStruct->DataSize;

	return *marked;
	
}
