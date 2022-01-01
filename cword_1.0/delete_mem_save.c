#include <stdio.h>
#include <stdlib.h>
#include "cword.h"
//使用cword.h中的Del_Word_Struct类型结构体链表
extern Del_Word_Struct* DeleteMemSave(FILE *file)
{
    int iCount = 0; //仅用于判断是否处于链表头
    FILE *WordFile;
    WordFile = file;
    Del_Word_Struct *pHead = NULL;
    Del_Word_Struct *pEnd,*pNew;
	long lDot; //流式文件当前位置
	rewind(WordFile);
	lDot = ftell(WordFile);
	pEnd = pNew = (Del_Word_Struct*)malloc(sizeof(Del_Word_Struct));
	//将单词读到内存中
	do
	{
		fseek(WordFile,lDot,SEEK_SET);
		if(fgetc(WordFile) == '$')
		{
			fseek(WordFile,lDot+1,SEEK_SET);
			int r;
			for(r=0;r<7;r++)
			{
				if((pNew->cNumber[r]=fgetc(WordFile)) == '#')
				{
					pNew->cNumber[r] = '\0';
					break;
				}
			}
			pNew->iNumber = atoi(pNew->cNumber);
			lDot = ftell(WordFile)+1;
			fseek(WordFile,lDot,SEEK_SET);
		}
		else
		{
			pEnd->pNext = NULL; //链表尾置为NULL
			goto break_do;
		}
		if(fgetc(WordFile) == '!')
		{
			fseek(WordFile,lDot,SEEK_SET);
			int s;
			for(s=0;s<52;s++)
			{
				if((pNew->cEnglish[s]=fgetc(WordFile)) == '#')
				{
					break;
				}
			}
			lDot = ftell(WordFile)+1;
			fseek(WordFile,lDot,SEEK_SET);
		}
		if(fgetc(WordFile) == '@')
		{
			fseek(WordFile,lDot,SEEK_SET);
			int r;
			for(r=0;r<102;r++)
			{
				if((pNew->cChinese[r]=fgetc(WordFile)) == '#')
				{
					break;
				}
			}
			lDot = ftell(WordFile)+1;
			fseek(WordFile,lDot,SEEK_SET);
		}
		pNew->pNext = NULL;
		if(iCount == 0)
		{
			pEnd = pNew;
			pHead = pNew;
		}
		else
		{
			pEnd->pNext = pNew;
			pEnd = pNew;
		}
		pNew = (Del_Word_Struct*)malloc(sizeof(Del_Word_Struct));
		iCount++;
	}while(1);
break_do:
	free(pNew);

    WordFile = NULL; //使用过的文件指针置空
    return pHead;
};