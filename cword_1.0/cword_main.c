#include <stdio.h>
#include "cword.h"
/*  注意事项：
	Del_Word_Struct 为链表结构体
*/
extern int FileNameDetect(char cLong[]);
extern void Write(FILE *file);
extern int WordListDetect(FILE *file);
extern void FixWordList();
extern void WriteHelp();
extern int Show(FILE *file);
extern Del_Word_Struct* DeleteMemSave(FILE *file); //删除前先将单词提交到内存
extern void DeleteWrite(FILE *file,Del_Word_Struct* pStructHead,int iNum); //删除功能

int main(int argc,char *argv[])
{
	FILE *cword_config; /*准备指向cword配置文件的空指针*/
	FILE *fFileName;
	fFileName=NULL;
	int file_index = 0; /*显示文件存在状态，为0则不存在文件，程序末尾不会关闭该指针*/
	cword_config = fopen("./.cword_config","r+"); /*以读写方式查看配置文件是否存在*/
	char cDefaultName[50];
	char cVersion[] = {"v1.0-fix220101"}; //程序版本

	/*检测配置文件是否存在*/
	if(cword_config == NULL)
	{
		printf("未发现配置文件，将新建一个配置文件\n");
		cword_config = fopen("./.cword_config","w+"); /*创建配置文件并使cword_config指向该文件*/
	}

	/*程序argc的执行部分*/
	if(argc == 2) /*当程序参数为1时程序的首选项*/
	{
		if((argv[1][0] != '-')||(argv[1][2] != '\0'))
		{
			printf("不适用的选项“%s”\n",argv[1]);
			goto help;
		}
		else
		{
			switch(argv[1][1])
			{
				case 'h':
help:
					printf("cword帮助：\n\t");
					printf("-h\t\t\t:显示帮助\n\t");
					printf("-v\t\t\t:显示程序版本\n\t");
					printf("-p\t\t\t:重置程序（清空配置文件）\n\t");
					printf("-c [文件名]\t\t:创建一个单词表\n\t");
					printf("-o [单词表文件]\t\t:打开一个单词表\n\t");
					printf("-d [单词表文件]\t\t:清空一个单词表\n\t");
					printf("-m [单词表文件]\t\t:指定一个默认单词表\n\t");
					printf("若无任何参数，则打开默认单词表\n");
					printf("\t请注意，配置文件被清空后，不加参数执行文件不会打开任意一个单词表\n\n");
					goto exit; /*显示帮助文件后都退出程序*/
					break;
				case 'p':
					printf("已清空配置文件\n");
					cword_config = fopen("./.cword_config","w"); /*清空配置表操作*/
					//向配置文件末尾追加一个#，以表示该文件为配置文件（用于探测配置文件内容）
                    fclose(cword_config);
                    cword_config = fopen("./.cword_config","r+"); /*变回原读写状态*/
					goto exit;
					break;
				case 'o':
				case 'c':
				case 'm':
				case 'd':
					printf("“-%c”选项后缺少参数\n\n",argv[1][1]);
					goto exit;
					break;
				case 'v':
					printf("版本：%s\n--(首版本：v1.0-fix211214)\n\n",cVersion);
					goto exit;
					break;
				default:
					printf("不适用的选项“%s”\n",argv[1]);
					goto help;
					break;
			}
		}
	}
	else if(argc == 3)
	{
		if((argv[1][0] != '-')||(argv[1][2] != '\0'))
		{
			printf("不适用的选项“%s”\n",argv[1]);
			goto help;
		}
		else
		{
			switch(argv[1][1])
			{
				case 'o':
					if(argv[2][0] == '.')
					{
						printf("不能打开以“.”开头的文件\n\n");
						goto exit;
					}
					if((fFileName = fopen(argv[2],"r")) == NULL)
					{
						printf("指定的文件 %s 不存在\n\n",argv[2]);
						goto exit;
					}
					else /*打开单词表操作*/
					{
						fFileName = fopen(argv[2],"r+"); /* r+ 读写操作，指针位于程序开头*/
						file_index=1;
					}
					break;
				case 'd':
					if(argv[2][0] == '.')
					{
						printf("不能清空以“.”开头的文件\n\n");
						goto exit;
					}
					if((fFileName = fopen(argv[2],"r")) == NULL)
                    {
                        printf("指定的文件 %s 不存在\n\n",argv[2]);
                        goto exit;
                    }
                    else /*清空单词表操作*/
                    {
						fFileName = fopen(argv[2],"w"); /*使用w选项清空了单词表*/
                        file_index=1;
						goto exit;
					}
					break;
				case 'm':
					if(argv[2][0] == '.')
					{
						printf("以“.”开头的文件不能作为默认文件\n\n");
						goto exit;
					}
					if((fFileName = fopen(argv[2],"r+")) == NULL) /*注意，此处以r+读写方式打开文件*/
                    {
                        printf("指定的文件 %s 不存在\n\n",argv[2]);
                        goto exit;
                    }
                    else /*指定默认单词表操作*/
                    {
						cword_config = fopen("./.cword_config","w"); /*先清空配置文件*/
						fputs(argv[2],cword_config); /*将默认单词表文件名写入配置文件*/
						fclose(cword_config);
						cword_config = fopen("./.cword_config","a");
						/*向配置文件末尾追加一个#，以表示该文件为配置文件（用于探测配置文件内容）*/
						fputc('#',cword_config); /*追加一个结束标记*/
						fputc('\n',cword_config);
						fclose(cword_config);
						cword_config = fopen("./.cword_config","r+");
                        file_index=1;
					}
					break;
				case 'c': /*创建单词表操作*/
					if((fFileName = fopen(argv[2],"r")) != NULL)
                    {
						printf("指定的单词表文件 %s 已存在，无需创建\n\n",argv[2]);
                        goto exit;
                    }
					else
					{
						if((FileNameDetect(argv[2])) == 1)
						{
							printf("文件长度超过了50个字符，请重命名\n\n");
							goto exit;
						}
						if(argv[2][0] == '.')
						{
							printf("单词表文件名不可以“.”开头！请重命名\n\n");
							goto exit;
						}
						fFileName = fopen(argv[2],"w+");
						if((fFileName = fopen(argv[2],"r")) != NULL) /*判断是否创建成功*/
						{
							printf("单词表创建成功！\n");
						}
						else
						{
							printf("单词表创建出现问题！\n");
						}
						file_index=1;
						goto exit;
					}
					break;
				default:
					printf("不适用的选项“%s %s”\n",argv[1],argv[2]);
                                        goto help;
					break;
			} /*switch*/
		}
	} /*判断主函数参数的括号*/
	else if(argc >= 4)
	{
		printf("程序参数过多！！！\n\n");
		goto exit;
	}
	else
	{
		printf("C词斩主程序启动，版本:%s\n\n",cVersion);
		/*检测配置文件部分*/
		int i;
		for(i=0;i<=50;i++)
		{
			cDefaultName[i]=fgetc(cword_config); //读取配置文件
			if(cDefaultName[i] == '#') /*将配置表中的文件名信息输出到变量中*/
			{
				cDefaultName[i] = '\0';
				break; /*遇到#则退出循环，且将最后一位置为\0*/
			}
		}
		/*注意，下面这条语句同时以r+打开了cDefaultName描述的文件*/
		if((fFileName = fopen(cDefaultName,"r+")) == NULL) /*配置文件中没有找到对应文件*/
		{
			//printf("%s\n",cDefaultName);
			printf("在配置文件中没有找到默认单词表文件，该文件是否被删除？\n你可以尝试-p选项来清空配置文件，并使用-m选项来指定一个新的默认单词表\n也可以使用-c选项来创建一个新单词表\n\n");
			goto exit;
		}
		else
		{
			printf("[info]: 打开了默认单词表 “%s”\n\n",cDefaultName);
		}
	}

/*C词斩主程序部分*/
	static char cInput;
	signed int iCorrect=-1;
	short int iInputcharInstruction=0; //决定是否输入功能键
	Del_Word_Struct* pWordHead; //定义一个DWS型的指针，指向结构体
	printf("欢迎来到C词斩！\n输入w录入单词，输入s回忆或操作单词，输入q退出:");
	while(iCorrect != 0)
	{
		if(iInputcharInstruction==0)
		{
			cInput=getchar(); /*输入*/
		}
		if(cInput == 'w')
		{
			Write(fFileName); /*调用Write函数执行相关功能*/
			iCorrect=0;
		}
		else if(cInput == 's')
		{
			iCorrect=Show(fFileName); /*调用Show函数执行相关功能*/
			//Show函数的返回检测
			if(iCorrect == -1) //Show函数为空，进入写单词操作
			{
				iInputcharInstruction=1; //不输入指令，直接循环
				cInput = 'w';
			}
			else if(iCorrect == 0) //输出版本信息，退出循环然后退出程序
			{
				printf("%s\n",cVersion);
			}
			else //删除单词操作，Show函数的返回表示要删除的单词序号
			{
				pWordHead = DeleteMemSave(fFileName); //缓存单词
				//清空单词表
				if(argc == 1)
				{
					fFileName = fopen(cDefaultName,"w");
				}
				else
				{
					fFileName = fopen(argv[2],"w");
				}
				//调用DeleteWrite
				//printf("debug: 03开始调用DeleteWrite\n");
				DeleteWrite(fFileName,pWordHead,iCorrect);
				//关闭当前文件指针
				fclose(fFileName);
				//重新以r+打开
				if(argc == 1)
				{
					fFileName = fopen(cDefaultName,"r+");
				}
				else
				{
					fFileName = fopen(argv[2],"r+");
				}
				iInputcharInstruction=1; //不输入指令，直接循环
			}
		}
		else if(cInput == 'q')
		{
			goto exit;
		}
		else
		{
			printf("你的输入不正确，请重新输入:");
		}
	}

exit: /*退出程序，程序出口*/
	fclose(cword_config);
	if(file_index != 0) /*若文件存在则使用后关闭该文件指针*/
	{
		fclose(fFileName);
	}
	return 0;
}
