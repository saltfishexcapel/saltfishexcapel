#include<stdio.h>
int iCorrectIndex[10][10]; /*排错大数组*/
//Del_Word_Struct类型的结构体链表
typedef struct Del_Word_Struct
{
    int iNumber;
    char cNumber[7];
    char cEnglish[52];
    char cChinese[102];
    struct Del_Word_Struct* pNext;
}Del_Word_Struct;