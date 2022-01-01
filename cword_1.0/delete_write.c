/*  DeleteWrite函数说明：
    FILE **file：                  传入一个指向空文件指针的指针
    Del_Word_Struct* pStructHead： 传入一个链表结构体头部的指针
    int iNum：                     传入要删除的单词序号
*/
#include <stdio.h>
#include <stdlib.h>
#include "cword.h"
//将新单词表写入到文件中
extern void DeleteWrite(FILE *file,Del_Word_Struct* pStructHead,int iNum) //在文件中删除某个单词，并重新为其他单词排好序
{
    char cChar = '#';
    while (1)
    {
        if ((pStructHead != NULL) && (pStructHead->iNumber < iNum))
        {
            fputc('$',file);
            fputs(pStructHead->cNumber,file);
            fputc('#',file);
            fputc('\n',file);
            fputs(pStructHead->cEnglish,file);
            fputc('\n',file);
            fputs(pStructHead->cChinese,file);
            fputc('\n',file);
            pStructHead = pStructHead->pNext;
        }
        else if(pStructHead->iNumber == iNum)
        {
            if (pStructHead->pNext == NULL) //要删除的单词在末尾的情况
            {
                //指针为空
                break;
            }
            else
            {
                //指针不为空，不为最后一个单词
                pStructHead = pStructHead->pNext;
            }
        }
        else
        {
            fputc('$',file);
            //此处单词序数减一，和前面的单词联系起来
            fprintf(file,"%d%c\n",pStructHead->iNumber-1,cChar);
            fputs(pStructHead->cEnglish,file);
            fputc('\n',file);
            fputs(pStructHead->cChinese,file);
            fputc('\n',file);
            pStructHead = pStructHead->pNext;
            if (pStructHead == NULL)
            {
                //读到最后一个单词，退出
                break;
            }
        }
    }
}