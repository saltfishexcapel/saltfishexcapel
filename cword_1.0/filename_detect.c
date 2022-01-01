/*文件名长度检测*/
extern int FileNameDetect(char cLong[])
{
	int iIndex=0,i=0;
	while(iIndex!=1)
	{
		if(cLong[i] == '\0')
		{
			break;
		}
		i++;
	}
	if(i<=51)
	{
		iIndex=0;
	}
	return iIndex;
}
