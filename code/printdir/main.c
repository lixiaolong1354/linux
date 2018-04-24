#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

void printdir(char *dir,int num);

int main(int argc,char *argv[])
{
        char * patch;
        patch = malloc(100);

        if(argc < 2)
        {
                printf("entern patch : ");
                scanf("%s",patch);
        }
        else{
                strcpy(patch,argv[1]);
        }

        printf("%s\n",patch);


        printdir(patch,0);

        return 0;
}

void printdir(char * dir,int num)
{
        printf("%s",dir);
        DIR * dp;
        struct dirent * entry;
        struct stat statbuf;

        if((dp = opendir(dir))== NULL)
        {
                fprintf(stderr,"cannot open directory : %s \n",dir);
                return ;
        }

        chdir(dir);
        while((entry = readdir(dp))!= NULL)
        {
                //lstat(entry->d_name,&statbuf);

                if(strcmp(".",entry->d_name) == 0|| strcmp("..",entry->d_name)== 0)
                {
                        continue;
                }

                printf("%*s%s%s",num,"|-",entry->d_name);
                if(S_ISDIR(statbuf.st_mode))
                {
                        printf("\\\n");
                        printdir(entry->d_name,num+2);
                }
                else{
                        printf("\n");
                }
        }

        chdir("..");
        closedir(dp);
        return;
}
