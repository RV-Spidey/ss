#include <stdio.h>
#include <string.h>
#include <stdlib.h>
void main()
{
FILE *fp;
int i,j,staddr1;
char name[10],line[50],name1[10],ch,staddr[10];
printf("enter the name of the program");
scanf("%s",name);
fp=fopen("objcode.txt","r");
fscanf(fp,"%s",line);
for(i=2,j=0;i<7;i++,j++)
        name1[j]=line[i];
name1[j]='\0';
printf("name from object file %s\n",name1);
if(strcmp(name,name1)==0)
{
        do
        {
                fscanf(fp,"%s",line);
                if(line[0]=='T')
                {
                        for(i=2,j=0;i<8;i++,j++)
                         staddr[j]=line[i];
                        staddr[j]='\0';
                        staddr1=atoi(staddr);
                        i=12;
                        while(line[i]!='\0')
                        {
                                if(line[i]!='^')
                                {
                                        printf("00%d\t%c%c\n",staddr1,line[i],line[i+1]);
                                        staddr1++;
                                        i=i+2;
                                }
                                else
                                        i++;
                        }
                }
                                
                        
                else if(line[0]=='E')
                {
                        printf("\nExecution Address");
                        for(i=2;i<8;i++)
                                printf("%c",line[i]);
                        break;
                }
        }
        while(!feof(fp));
        }
fclose(fp);
}
