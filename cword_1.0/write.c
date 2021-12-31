#include <stdio.h>
#include <stdlib.h>
#include "cword.h"
extern void WriteHelp();
extern int iCorrectIndex[10][10];
extern void FixWordList();
extern void Write(FILE **file) /*写单词功能函数*/
{
	FILE *WordFile;
	WordFile=*file;
	struct Write_Word_Struct
	{
		int iNumber;
		char cNumber[7];
		char cEnglish[52];
		char cChinese[102];
	}sArrayWrite;
	int m;
	for(m=0;m<102;m++) /*初始化结构体*/
	{
		if(m<7)
		{
			sArrayWrite.cNumber[m]='\0';
		}
		if(m<52)
		{
			sArrayWrite.cEnglish[m]='\0';
		}
		sArrayWrite.cChinese[m]='\0';
	}
	printf("单词、短语录入功能\n\n");
	/*判断单词列表是否为空*/
	rewind(WordFile); /*文件指针指向文件开头*/
	if(fgetc(WordFile) == EOF) /*文件为空fgetc将返回EOF*/
	{
		printf("0v0单词列表为空，开始添加你的单词吧！\n");
		iCorrectIndex[1][0]=1;
		sArrayWrite.iNumber=0;
		goto not_detect; /*单词表为空，不检测完整性*/
	}
/*单词表完整性检测*/


	/*单词表主体的检测与修复尚未开发*/
	FixWordList();

	/*在单词表没有错误或为空的情况下录入单词主体*/
	/*录入程序主体*/

not_detect:
	/*先检测已有多少条目，若为空，则跳过该步骤*/
	if(iCorrectIndex[1][0]!=1) /*不为空*/
	{
		rewind(WordFile);
		int p=0;
		for(p=0;p<99999999;p++)
		{
			char cState; /*描述文件指针目前状态*/
			cState = fgetc(WordFile);
			if(cState == '$')
			{
				int o=0;
				for(o=0;o<6;o++)
				{
					sArrayWrite.cNumber[o]=fgetc(WordFile);
					if(sArrayWrite.cNumber[o] == '#') /*检测到#退出循环，得到数字字符串*/
					{
						//printf("debug: 检测到了#, 下一个字符debug为%c\n",fgetc(WordFile));
						sArrayWrite.cNumber[o]='\0'; /*将#置为空以便得到当前位数*/
						o=6;
					}
				}
			}
			else if(cState == '\n')
			{
				//printf("debug: 检测到了换行的情况\n");
			}
			else if(cState == '!')
			{
				//printf("debug: 检测到执行了!\n");
				int o=0;
				for(o=0;o<50;o++)
				{
					sArrayWrite.cEnglish[o]=fgetc(WordFile);
					if(sArrayWrite.cEnglish[o] == '#')
					{
						sArrayWrite.cEnglish[o] = '\0';
						o=50;
					}
				}
			}
			else if(cState == '@')
			{
				//printf("debug: 检测到执行了@\n");
				int o=0;
				for(o=0;o<100;o++)
				{
					sArrayWrite.cChinese[o]=fgetc(WordFile);
					if(sArrayWrite.cChinese[o] == '#')
					{
						sArrayWrite.cChinese[o] = '\0';
						o=100;
					}
				}
			}
			else if(cState == EOF) /*EOF的情况*/
			{
				//printf("debug:发现了EOF的情况\n");
				//char cEnter = '\n';
				/*先向单词表中写入一个空格符*/
				fseek(WordFile,0,SEEK_END); /*文件指针指向文件末尾*/
				//fputc(cEnter,WordFile);
				//换行操作
				p=100000000;
			}
			else
			{
				printf("出现一个严重错误! debug: p:%d, fgetc为%c\n",p,fgetc(WordFile));
				p=100000000;
			}
		}
		sArrayWrite.iNumber=atoi(sArrayWrite.cNumber);
		printf("已记录%d条单词条目\n",sArrayWrite.iNumber);
	}
/*录入*/
	printf("录入英语或中文时时可输入#退出（不保存正在输入的数据），输入<重新输入，输入?显示帮助\n\n");
	int q=0;
	int iNumberState;
	int iCet; /*首次<符纠错指令*/
	int *iCorrect=&iCet;
	int iInstr=0; /*写入指令实际变量*/
	int *iInstruction=&iInstr; /*写入指令*/
	iNumberState=sArrayWrite.iNumber;
	sArrayWrite.iNumber++;
	char cInput_en[100],cInput_zh_s[100],cNumberTemp[5]; /*cNumberTemp用于暂时存储数字 (sArrayWrite.iNumber)*/
	char *cInput_zh=cInput_zh_s;

	for(q=0;q<(10000-iNumberState);q++) /*录入单词操作*/
	{
		int r=0;
		for(r=0;r<100;r++) /*每输入一组单词都需重置数组*/
		{
			if(r<5)
			{
				cNumberTemp[r] = '\0';
			}
			cInput_en[r] = '\0';
			*(cInput_zh+r) = '\0';
		}
		for(m=0;m<102;m++) /*每输入一组单词都重新初始化结构体*/
		{
			if(m<7)
			{
				sArrayWrite.cNumber[m]='\0';
			}
			if(m<52)
			{
				sArrayWrite.cEnglish[m]='\0';
			}
			sArrayWrite.cChinese[m]='\0';
		}

		printf("\n第%d个条目\n",sArrayWrite.iNumber);
		itoa(sArrayWrite.iNumber,cNumberTemp,10);
		sArrayWrite.cNumber[0] = '$'; /*写入数字序号头部*/
		int s=0;
		for(s=0;s<5;s++)
		{
			if(cNumberTemp[s] == '\0')
			{
				sArrayWrite.cNumber[s+1] = '#';
				sArrayWrite.cNumber[s+2] = '\n'; 
				s=5;
			}
			else
			{
				sArrayWrite.cNumber[s+1]=cNumberTemp[s];			
			}
		}

		printf("请输入英语：");
		if(q==0 && *iCorrect!=1) /*iCorrect:输入<后产生了一个回车符*/
		{
			getchar(); /*PATCH: 产生一个回车符，使用getchar()将回车符吸收掉*/
		}
		fgets(cInput_en,50,stdin);
		//printf("debug: cInput_en输入为: %s\n",cInput_en);
		if(cInput_en[0] == '#')
		{
			printf("退出录入\n");
			*iInstruction=0; /*不录入*/
			goto exit_write;
		}
		else if(cInput_en[0] == '?')
		{
			//WriteHelp();
			*cInput_zh = '?';
			//*iInstruction=0;
			/*不录入*/
		}
		else if(cInput_en[0] == '<')
		{
			*cInput_zh = '<'; /*转交给中文输入部分*/
		}
		else if(cInput_en[0] == '\n')
		{
			//printf("什么都没有输入！\n");
			//*iInstruction=0;
			//sArrayWrite.iNumber--;
			//q--;
			*cInput_zh = '\n'; /*转交给中文输入部分*/
		}
		else /*将单词录入内存*/
		{
			*iInstruction=1;
			sArrayWrite.cEnglish[0] = '!';
			//int r=0;
			r=0; 
			for(r=0;r<50;r++)
			{
				if(cInput_en[r] == '\n')
				{
					sArrayWrite.cEnglish[r+1] = '#'; /*设置结束位*/
					sArrayWrite.cEnglish[r+2] = '\n';
					r=50; /*退出循环*/
				}
				else
				{
					sArrayWrite.cEnglish[r+1]=cInput_en[r];
				}
			}
		}
		//printf("debug: 目前cInput_zh:%c\n",*cInput_zh);

		if((cInput_en[0] != '<')&&(cInput_en[0] != '?')&&(cInput_en[0] != '\n'))
		{
			printf("请输入中文：");
			fgets(cInput_zh,100,stdin);
		}
		if(*cInput_zh == '#')
		{
			printf("退出录入\n");
			*iInstruction=0;
			goto exit_write;
		}
		else if(*cInput_zh == '?')
		{
			//printf("什么都没有输入！\n");
			//getchar();
			WriteHelp();
			*iInstruction=0;
			sArrayWrite.iNumber--;
			q--;
			*iCorrect=1;
		}
		else if(*cInput_zh == '\n')
		{
			printf("！什么都没输入！重新输入\n");
			*iInstruction=0;
			sArrayWrite.iNumber--;
			q--;
			*iCorrect=1;
		}
		else if(*cInput_zh == '<')
		{
			printf("重新输入\n");
			//getchar();
			*iInstruction=0;
			sArrayWrite.iNumber--;
			q--;
			*iCorrect=1;
		}
		else /*将中文录入内存*/
		{
			*iInstruction=1;
			sArrayWrite.cChinese[0] = '@';
			//int r=0;
			r=0; 
			for(r=0;r<100;r++)
			{
				if(*(cInput_zh+r) == '\n') /*录入后回车即是\n，可通过检测回车判断输入结束点*/
				{
					sArrayWrite.cChinese[r+1] = '#';
					sArrayWrite.cChinese[r+2] = '\n';
					r=100; /*退出循环*/
				}
				else
				{
					sArrayWrite.cChinese[r+1]=*(cInput_zh+r);
				}
			}
		}
		/*将更改写入硬盘*/
		fseek(WordFile,0,SEEK_END); /*文件指针指向文件末尾*/
		//printf("debug: 写入的cNumber:%s\n",sArrayWrite.cNumber);
		//printf("debug: 写入的cEnglish:%s\n",sArrayWrite.cEnglish);
		//printf("debug: 写入的cChinese:%s\n",sArrayWrite.cChinese);
		if(*iInstruction==1)
		{
			fputs(sArrayWrite.cNumber,WordFile);
			fputs(sArrayWrite.cEnglish,WordFile);
			fputs(sArrayWrite.cChinese,WordFile);
		}
		*iInstruction=0; /*重置指示变量*/
		sArrayWrite.iNumber++;
	}

exit_write:

	WordFile=NULL; /*将录入单词功能所使用的文件指针置空*/
}
