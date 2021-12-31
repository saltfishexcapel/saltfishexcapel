#include <stdio.h>
extern void WriteHelp()
{
	printf("录入帮助：\n");
	printf("输入<可重录入，输入#退出录入\n");
	printf("单词中不可输入!#$?@样的字符(会导致程序错误)，支持输入的字符有'/()\n");
	getchar();
}
