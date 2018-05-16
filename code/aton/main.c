/*
浮雕刀具路径转换程序
可将刀具路径改变下切步距
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_MAX 1024                       //定义基本字符串大小

struct info{                                //定义全局结构
    char *str;
    FILE * source_file;
    FILE * target_file;
    int top;
    int line;
    double current;
};

int convert(struct info * buf);
double scanning(struct info * buf);
int load(struct info * tem,char ** str);

int main(int argc,char* argv[])
{
    if(argc != 5)
    {
        printf("error");
        exit(-5);
    }
    struct info * mmg = malloc(sizeof(struct info));
    load(mmg,argv);                         //装载参数
    convert(mmg);                           //数据转换

    fclose(mmg->target_file);               //释放内存空间
    fclose(mmg->source_file);
    free(mmg->str);
    free(mmg);
    return 0;
}

int convert(struct info * buf)
{
    double z_max= buf->current,num=0;
    do{
        printf("Z: %f\n",buf->current);
        fseek(buf->source_file,0,SEEK_SET); //每次重复重复读取源
        while(!feof(buf->source_file))
        {
            fgets(buf->str,LINE_MAX,buf->source_file);
            num = scanning(buf);            //具体转换

            if(num < z_max)                 //判断刀具高度最低位置
            {
                z_max = num;                //提供最低判断位置
            }
        }
        buf->current -= buf->line;
    }while(z_max-buf->line < buf->current); //判断是否完全转换到最低位置
    return 0;
}

double scanning(struct info * buf)
{
    double max =0;
    if(buf->str[0] == 'G')                  //判断是否是刀具移动行
    {
        int num = strcspn(buf->str,"Z");    //找到刀具高度参数标记
        if(num != strlen(buf->str))         //判断当前行是否有刀具高度参数 没有跳出
        {
            num += strcspn(&(buf->str[num]),"-.1234567890");        //找到刀具高度参数位置

            max = atof(&(buf->str[num]));                           //解析刀具高度参数
            if(max < buf->current)                                  //判断刀具产生是否超过限定  过线将处理
            {
                int x = strspn(&(buf->str[num]),"-.1234567890");    //覆盖原刀具高度参数为空
                while(x+1)
                {
                    buf->str[num+x--] = ' ';
                }
                sprintf(&(buf->str[num]),"%.4f",buf->current);      //填充新刀具高度参数
            }
        }
    }
    fputs(buf->str,buf->target_file);        //写入行
    fputc('\n',buf->target_file);            //写入换行
    return max;                              //返回刀具高度参数
}

int load(struct info * tem,char ** str)      //装载参数
{
        tem->str = malloc(LINE_MAX);
        tem->source_file = fopen(str[1],"r");
        tem->target_file = fopen(str[2],"w");
        tem->top = atoi(str[3]);
        tem->line= atoi(str[4]);
        tem->current = tem->top;
        if(tem->source_file == NULL || tem->target_file == NULL)
        {
            printf("file open error!");
            return -1;
        }
        printf("file: %s to file : %s.\nfirst height: %d \t overlay height: %d\n",str[1],str[2],tem->top,tem->line);
        return 0;
}
