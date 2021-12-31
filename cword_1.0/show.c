#include <stdio.h>
#include <stdlib.h>
extern int Show(FILE **file) /*展示单词功能函数*/
{
	int iIndex;
	FILE *WordFile;
	WordFile=*file;
	struct Show_Word_Struct
	{
		int iNumber;
		char cNumber[7];
		char cEnglish[52];
		char cChinese[102];
		struct Show_Word_Struct* pNext; //使用链表结构
	};

	int iCount=0;

	//将单词从文件读出,写入到内存
	
	struct Show_Word_Struct* pHead=NULL;
	struct Show_Word_Struct* pEnd,* pNew;
	long lDot; //流式文件当前位置
	rewind(WordFile);
	lDot=ftell(WordFile);
	pEnd=pNew=(struct Show_Word_Struct*)malloc(sizeof(struct Show_Word_Struct));
	//将单词读到内存中
	do
	{
		//printf("debug: 执行了do\n");
		if((fgetc(WordFile) == EOF)&&(iCount==0)) //单词列表为空的提示
		{
			printf("单词列表为空！快添加单词吧！\n\n");
			//fseek(WordFile,lDot,SEEK_SET); 
			//返回最后位置
			iIndex=0; //函数返回的参数
			goto exit_show;
		}
		else
		{
			iIndex=1;
			fseek(WordFile,lDot,SEEK_SET);
		}
		if(fgetc(WordFile) == '$')
		{
			//printf("debug: $执行了该步骤,lDot:%ld\n",lDot);
			lDot++; //移向下一位
			fseek(WordFile,lDot,SEEK_SET);
			int r;
			for(r=0;r<7;r++)
			{
				//printf("debug: r$\n");
				if((pNew->cNumber[r]=fgetc(WordFile)) == '#')
				{
					pNew->cNumber[r]='\0';
					r=7;
				}
			}
			pNew->iNumber=atoi(pNew->cNumber);
			lDot=ftell(WordFile)+1;
			fseek(WordFile,lDot,SEEK_SET);
			//printf("debug: $-#lDot:%ld\n",lDot);
		}
		else
		{
			printf("%d个单词已缓存,可以使用\n\n",pEnd->iNumber);
			goto break_do;
		}
		if(fgetc(WordFile) == '!')
		{
			//printf("debug: !执行了该步骤,lDot:%ld\n",lDot);
			lDot++;
			fseek(WordFile,lDot,SEEK_SET);
			//printf("debug: 1lDot:%ld\n",lDot);
			int s;
			for(s=0;s<52;s++)
			{
				//printf("debug: 01cEnglish,s:%d\n",s);
				//printf("debug: 2ftell:%ld\n",ftell(WordFile));
				if((pNew->cEnglish[s]=fgetc(WordFile)) == '#')
				{
					//printf("debug: 02cEnglish,s:%d\n",s);
					//printf("debug: 3lDot:%ld\n",lDot);
					pNew->cEnglish[s]='\0';
					s=52;
				}
			}
			lDot=ftell(WordFile)+1;
			fseek(WordFile,lDot,SEEK_SET);
			//printf("debug: !-#lDot:%ld\n",lDot);
		}
		if(fgetc(WordFile) == '@')
		{
			//printf("debug: @执行了该步骤,lDot:%ld\n",lDot);
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
			lDot=ftell(WordFile)+1;
			fseek(WordFile,lDot,SEEK_SET);
			//printf("debug: @-#lDot:%ld\n",lDot);
		}
		pNew->pNext=NULL;
		if(iCount==0)
		{
			pEnd=pNew;
			pHead=pNew;
		}
		else
		{
			pEnd->pNext=pNew;
			pEnd=pNew;
		}
		pNew=(struct Show_Word_Struct*)malloc(sizeof(struct Show_Word_Struct));
		iCount++;
	}while(1);
break_do:
	free(pNew);
	//读取单词表操作
	printf("回车查看下一个单词, 输入 h 可以查看帮助或功能，输入 q 退出\n");
	getchar();

	int iKey,u;
	int iModel=0;
	struct Show_Word_Struct* pShow;
	struct Show_Word_Struct* pTemp;
	//struct Show_Word_Struct* pPrevious;
	pTemp=pHead;
	//pPrevious=pTemp;
	for(u=0;u<99999;u++)
	{
		printf(":");
		iKey=(int)getchar();
		//printf("debug: iKey:%d\n",iKey);
		if(iKey==104) //帮助
		{
			printf("帮助：\n\t回车：下一个单词\n\tq：退出\n\tr: model\n\tn：跳转到指定数字对应的单词\n");
			getchar();
		}
		else if(iKey==114) //r:
		{
			if(iModel==0)
				iModel++;
			else if(iModel==1)
				iModel++;
			else
				iModel=0;
		}
		else if(iKey==10) //回车
		{
			pShow=pTemp;
			pTemp=pTemp->pNext;
			if(iModel==0)
			{
				printf("(%d/%d) %s   %s\n",pShow->iNumber,pEnd->iNumber,pShow->cEnglish,pShow->cChinese);
			}
			else if(iModel==1)
			{
				printf("(%d/%d) %s",pShow->iNumber,pEnd->iNumber,pShow->cEnglish);
				getchar();
				printf("  %s\n",pShow->cChinese);
				//getchar();
			}
			else
			{
				printf("(%d/%d) %s",pShow->iNumber,pEnd->iNumber,pShow->cChinese);
				getchar();
				printf("  %s\n",pShow->cEnglish);
				//getchar();
			}
			if(pTemp == NULL)
			{
				printf("\n单词表结束，退出\n");
				break;
			}
		}
		else if(iKey==110) //当输入功能键n的时候执行操作
		{
			int iJump; //跳转到对应数字
			struct Show_Word_Struct* pJump;
			printf("请输入一个数字：");
			scanf("%d",&iJump);
			//遍历单词表
			pJump=pHead; //得到链表头
			if(pEnd->iNumber < iJump)
			{
				pJump = NULL;
				printf("输入的数字大于总单词数！\n");
				getchar();
			}
			while(pJump != NULL)
			{
				if((pJump->iNumber!=iJump)&&(pJump != NULL))
				{
					pJump=pJump->pNext;
				}
				else if(pJump == NULL)
				{
					printf("所输入数字超过了单词表总单词个数！\n将返回到上个单词");
					break;
				}
				else
				{
					pShow=pJump;
					pTemp=pJump;
					printf("\n"); //回车键
					break;
				}
			}
		}
		else if(iKey==113) //输入q退出
		{
			printf("退出\n");
			break;
		}
		else if(iKey==27) //输入溢出的情况
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
