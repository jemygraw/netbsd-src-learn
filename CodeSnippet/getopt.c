/*
	Show the usage of the getopt function.
*/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int main(int argc, char *argv[])
{
	int aflag=0,nflag=0,lflag=0;
	int ch;
	while((ch=getopt(argc, argv, "an:l:"))!=-1)
	{
		switch(ch)
		{
		case 'a':
			printf("next option index:%d current opt arg:%s\n",optind,optarg);
			break;
		case 'n':
			printf("next option index:%d current opt arg:%s\n",optind,optarg);
			break;
		case 'l':
			printf("next option index:%d current opt arg:%s\n",optind,optarg);
			break;
		default:
			printf("no option"); 
		}
	}
	argc-=optind;
	argv+=optind;
	printf("Remaining argv count:%d\n",argc);
	while(*argv)
	{
		printf("%s\n",*argv);
		argv++;
	}
	return 0;
	
}
