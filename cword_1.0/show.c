#include <stdio.h>
#include <stdlib.h>
extern int Show(FILE *file) /*展示单词功能函数*/
{
	signed int iIndex;
	FILE *WordFile;
	WordFile = file;
	struct Show_Word_Struct
	{
		int iNumber;
		char cNumber[7];
		char cEnglish[52];
		char cChinese[102];
		struct Show_Word_Struct* pNext; //使用链表结构
	};

	int iCount = 0;

	//将单词从文件读出,写入到内存
	
	struct Show_Word_Struct* pHead = NULL;
	struct Show_Word_Struct* pEnd,* pNew;
	long lDot; //流式文件当前位置
	rewind(WordFile);
	lDot = ftell(WordFile);
	pEnd = pNew = (struct Show_Word_Struct*)malloc(sizeof(struct Show_Word_Struct));
	//将单词读到内存中
	do
	{
		if ((fgetc(WordFile) == EOF)&&(iCount == 0)) //单词列表为空，转至写单词
		{
			iIndex = -1; //单词表为空，函数返回-1，直接转到写单词函数
			goto exit_show;
		}
		else
		{
			iIndex = 0; //若单词表不为空，iIndex为0（函数返回必为0）
			fseek(WordFile,lDot,SEEK_SET);
		}
		if(fgetc(WordFile) == '$')
		{
			lDot++; //移向下一位
			fseek(WordFile,lDot,SEEK_SET);
			int r;
			for(r=0;r<7;r++)
			{
				//写入内存，遇到#时退出
				if((pNew->cNumber[r]=fgetc(WordFile)) == '#')
				{
					pNew->cNumber[r]='\0';
					r=7;
				}
			}
			pNew->iNumber=atoi(pNew->cNumber);
			lDot = ftell(WordFile)+1; //文件指针将指向下一位
			fseek(WordFile,lDot,SEEK_SET);
		}
		else
		{
			printf("%d个单词已缓存,可以使用\n\n",pEnd->iNumber);
			goto break_do;
		}
		if(fgetc(WordFile) == '!')
		{
			lDot++;
			fseek(WordFile,lDot,SEEK_SET);
			int s;
			for(s=0;s<52;s++)
			{
				if((pNew->cEnglish[s]=fgetc(WordFile)) == '#')
				{
					pNew->cEnglish[s]='\0';
					s=52;
				}
			}
			lDot = ftell(WordFile)+1;
			fseek(WordFile,lDot,SEEK_SET);
		}
		if(fgetc(WordFile) == '@')
		{
			lDot++;
			fseek(WordFile,lDot,SEEK_SET);
			int r;
			for(r=0;r<102;r++)
			{
				if((pNew->cChinese[r]=fgetc(WordFile)) == '#')
				{
					pNew->cChinese[r]='\0';
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
		pNew = (struct Show_Word_Struct*)malloc(sizeof(struct Show_Word_Struct));
		iCount++;
	}while(1);
break_do:
	free(pNew);
	//读取单词表操作
	printf("回车查看下一个单词, 输入 h 可以查看帮助或功能，输入 q 退出\n");
	getchar();

	int iKey,u;
	int iModel = 0;
	struct Show_Word_Struct* pShow;
	struct Show_Word_Struct* pTemp;
	pTemp = pHead;
	for(u=0;u<99999;u++)
	{
		printf(":");
		iKey = (int)getchar();
		if(iKey == 104) //帮助
		{
			printf("帮助:\n\t回车:\t下一个单词\n\tq:\t退出\n\tr:\t切换一种模式\n\td:\t删除某个单词\n\tn:\t跳转到指定数字对应的单词\n");
			getchar();
		}
		else if(iKey == 100) //输入为d，函数返回要删除的单词序号
		{
			int iInput;
			printf("请输入要删除的单词序号：");
			scanf("%d",&iInput);
			if (iInput <= pEnd->iNumber)
			{
				return iInput; //函数返回该数字后，退出Show函数
			}
			else
			{
				printf("输入的数字大于总的单词数！\n");
				getchar();
			}
		}
		else if(iKey == 114) //r:切换一种模式
		{
			if(iModel == 0)
				iModel++;
			else if(iModel == 1)
				iModel++;
			else
				iModel = 0;
		}
		else if(iKey == 10) //回车
		{
			pShow = pTemp;
			pTemp = pTemp->pNext;
			if(iModel == 0)
			{
				printf("(%d/%d) %s   %s\n",pShow->iNumber,pEnd->iNumber,pShow->cEnglish,pShow->cChinese);
			}
			else if(iModel == 1)
			{
				printf("(%d/%d) %s",pShow->iNumber,pEnd->iNumber,pShow->cEnglish);
				getchar();
				printf("  %s\n",pShow->cChinese);
			}
			else
			{
				printf("(%d/%d) %s",pShow->iNumber,pEnd->iNumber,pShow->cChinese);
				getchar();
				printf("  %s\n",pShow->cEnglish);
			}
			if(pTemp == NULL)
			{
				printf("\n单词表结束，退出\n");
				break;
			}
		}
		else if(iKey == 110) //当输入功能键n的时候执行操作
		{
			int iJump; //跳转到对应数字
			struct Show_Word_Struct* pJump;
			printf("请输入一个数字：");
			scanf("%d",&iJump);
			//遍历单词表
			pJump = pHead; //得到链表头
			if(pEnd->iNumber < iJump)
			{
				pJump = NULL;
				printf("输入的数字大于总单词数！\n");
				getchar();
			}
			while(pJump != NULL)
			{
				if((pJump->iNumber != iJump)&&(pJump != NULL))
				{
					pJump = pJump->pNext;
				}
				else if(pJump == NULL)
				{
					printf("所输入数字超过了单词表总单词个数！\n将返回到上个单词");
					break;
				}
				else
				{
					pShow = pJump;
					pTemp = pJump;
					printf("\n"); //回车键
					break;
				}
			}
		}
		else if(iKey == 113) //输入q退出
		{
			printf("退出\n");
			break;
		}
		else if(iKey == 27) //输入溢出的情况
		{
			printf("输入不正确, 数据溢出！输入 h 查看帮助\n");
			getchar();
			getchar();
		}
		else
		{
			printf("输入不正确！输入 h 查看帮助\n");
			getchar();
		}
	}

exit_show:
	WordFile = NULL; /*将显示单词功能所使用的文件指针置空*/

	return iIndex;
}
