/*
	Linux下键盘非阻断输入
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>

char *menu[]  = {
	"a - add new record",
	"d - delete record",
	"q - quit",
	NULL,
};

int getchoice(char *greet,char *choices[],FILE *in,FILE *out);
int main(int argc,char *argv[])
{
	int choice;
	FILE *input,*output;
	struct termios init,new_t;

	if(!isatty(fileno(stdin)))
	{
		fprintf(stderr,"output error\n");
	}

	input = fopen("/dev/tty","r");
	output= fopen("/dev/tty","w");
	if(!input || !output)
	{
		fprintf(stderr,"fopen tty error\n");
		exit(-1);
	}

	tcgetattr(fileno(stdin),&init);
	new_t = init;
	new_t.c_lflag &= ~ICANON;
	new_t.c_lflag &= ~ECHO;
	new_t.c_cc[VMIN] = 1;
	new_t.c_cc[VTIME]= 0;
	new_t.c_lflag &= ~ISIG;
	if(tcsetattr(fileno(stdin),TCSANOW,&new_t))
	{
		fprintf(stderr,"termios not set\n");
	}

	do{
		choice = getchoice("please select an action",menu,input,output);
		printf("you have chosen: %c\n",choice);
	}while(choice != 'q');
	tcsetattr(fileno(stdin),TCSANOW,&init);
    return 0;
}

int getchoice(char *greet,char* choices[],FILE*in,FILE*OUT)
{
	int choice = 0,num;
	char tem;
	char **option;
	do{
		num = 0;
		option = choices;
		printf("%s\n",greet);
		while(option[num] != NULL)
		{
			printf("%s\n",option[num++]);
		}

		do{
			tem = fgetc(in);
		}while(tem == '\n' || tem == '\r');

		num = 0;
        option = choices;
        while(option[num] != NULL)
		{
			if(tem == option[num++][0])
			{
				choice = tem;
				break;
			}
		}
	}while(!choice);
	return choice;
}
