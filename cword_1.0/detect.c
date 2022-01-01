#include <stdio.h>
#include <ctype.h>
#include "cword.h"
extern int WordListDetect(FILE *file) /*检测函数*/
{
	FILE *WordFile;
	WordFile=file;
	int m,n;
	for(m=0;m<10;m++)
	{
		for(n=0;n<10;n++)
		{
			iCorrectIndex[m][n]=0;
		}
	} /*初始化排错数组*/
	/*开头的检测与修复*/
	//修复功能待完善
	rewind(WordFile);
	if(fgetc(WordFile) == '$')
	{
		iCorrectIndex[0][0]=0; /*单词表头部未检测出错误*/
	}
	else
	{
		iCorrectIndex[0][0]=1;
	}
	int i;
	for(i=0;i<9999;i++) /*设置9999次循环是为了检出单词表部分，为自动纠错做准备*/
	{
		//printf("debug:第一处for被执行\n");
		if(iCorrectIndex[0][0] == 0) /*若开头标识无错误，则检测数字选项是否正确*/
		{
			if(i == 0)
			{
				if(fgetc(WordFile) == '1') /*开头正确的数字*/
				{
					iCorrectIndex[0][1]=0;
					if(fgetc(WordFile) == '#')
					{
						iCorrectIndex[0][2]=0;
					}
					else
					{
						iCorrectIndex[0][1]=1;
					}
				}
				else
				{
					iCorrectIndex[0][1]=1; /*开头数字不正确*/
				}
			}
			else if(i == 1)
			{
				rewind(WordFile);
				fgetc(WordFile);
				fgetc(WordFile); /*读两次*/
				if(fgetc(WordFile) == '#') /*开头正确的结束标识*/
				{
					iCorrectIndex[0][2]=0;
				}
				else
				{
					iCorrectIndex[0][2]=1; /*开头结束标识有误*/
				}
			}
			else
			{
				if(i < 6) /*继续探测结束符的存在*/
				{
					if(fgetc(WordFile) == '#')
                    {
                        iCorrectIndex[0][2]=0;
						goto exit_for_a; /*探测到结束符就退出循环*/
                    }
                    else
                    {
                        iCorrectIndex[0][2]=1;
                    }
				}
				else
				{
					goto exit_for_a;
				}
			}
		}
		else if(iCorrectIndex[0][0] == 1) /*开头标识未检测到的情况下检测之后是否含有头部 “$1#” */
		{
			int l;
			for(l=0;l<9999;l++)
			{
				if(fgetc(WordFile) == '$')
				{
					printf("debug: 检测到$\n");
					iCorrectIndex[0][3]=0; /*表示包含头部符号“$”，程序有被修复的可能性*/
					if(fgetc(WordFile) == '1') /*检测下一位是否是1，若不是则表示可能不是文件头部*/
					{
						printf("debug: 检测到1\n");
						iCorrectIndex[0][1]=0; /*1号位正确*/
						if(fgetc(WordFile) == '#')
						{
							printf("debug: 检测到#\n");
							iCorrectIndex[0][2]=0; /*结束位正确，包含单词表头部，可被修复*/
						}
						else
						{
							int j;
							for(j=0;j<=5;j++) /*检测剩下是否为数字，是否包含结束位*/
							{
								if(fgetc(WordFile) == '#')
								{
									iCorrectIndex[0][2]=0; /*在有限数字位数内包含结束位*/
									j=6; /*退出循环*/
								}
								else
								{
									iCorrectIndex[0][2]=1; /*没找到结束位*/
								}
							}
						}
					}	
					else /*1号位不是1*/
					{
						if(isdigit(fgetc(WordFile)) != 0) /*检测1号位上是否为数字*/
						{
							int j;
                            for(j=0;j<=5;j++) /*检测剩下是否为数字，是否包含结束位*/
        		           	{
                                if(fgetc(WordFile) == '#')
                                {
									iCorrectIndex[0][2]=0; /*在有限数字位数内包含结束位*/
                                    j=6; /*退出循环*/
                                }
                            	else
                              	{
                                   	iCorrectIndex[0][2]=1; /*没找到结束位*/
            	             	}
	                    	}
							iCorrectIndex[0][1]=1;
						}
						else /*不是数字*/
						{
							iCorrectIndex[0][1]=1;
						}
					}
					goto exit_for_a; /*检测到头部，退出循环*/
				}
				else
				{
					//printf("debug:没检测到$\n");
					iCorrectIndex[0][3]=1; /*未检测到头部，单词表没有被修复的可能*/
				}
				printf("debug:l:%d\n",l);
			} /*for循环*/
		}
		else 
		{
			printf("###出现了程序错误！###\n"); //小概率事件
			goto exit_for_a;
		}
	}

exit_for_a:
	return 0;

//	头部检测debug
//	{
//	int k;
//	for(k=0;k<4;k++)
//	{
//		printf("debug: iCorrectIndex[0][%d]:%d\n",k,iCorrectIndex[0][k]);
//	}
//	}
}
