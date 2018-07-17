# ifndef  __LOOPLIST_H_
#define __LOOPLIST_H_
#define		LP_ERR			unsigned char
#define		LP_NOERR		(unsigned char)0x01
#define     LP_POINTERR		(unsigned char)0x02
#define     LP_NODATA		(unsigned char)0x03
#define     LP_INPUTERR		(unsigned char)0x04	
#define     LP_DATANOTENOPH	(unsigned char)0x05

#define			BIGLIN			0x01
#define			LITTERLIN		0x02

typedef struct
{
	
	unsigned char *pData; //存放数据的数组
	unsigned short ReadPoint;
	unsigned short WritePoint;
	unsigned short BufferSize;
	unsigned short DataSize;
	
}LoopListStruct;


typedef struct{  //标记队列数据所用

	unsigned short ReadPoint;
	unsigned short DataSize;
  unsigned char  State;
	
}LoopList_ReadFeature;


LP_ERR LoopList_Init(LoopListStruct *NewStruct,const unsigned NewBufferSize);
LP_ERR LoopList_AddData(LoopListStruct *NewStruct,const unsigned char NewData);
LP_ERR LoopList_AddMultiData(LoopListStruct *NewStruct,const unsigned char *NewData,unsigned short length);
LP_ERR LoopList_GetOneData(LoopListStruct *NewStruct,unsigned char *pCh);
LP_ERR LoopList_GetMultiData(LoopListStruct *NewStruct,unsigned char *pCh,const unsigned short Size,
															const unsigned char DataType);
LP_ERR LoopList_Delete(LoopListStruct *NewStruct);

LP_ERR LoopList_FeedBack(LoopListStruct *NewStruct,unsigned short read_mark,unsigned short size_mark);
LoopList_ReadFeature LoopList_Marked(LoopListStruct *NewStruct);

#endif
