#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100000
void main()
{
    char Word_Mean[200],s1[2],s2[2],s[20];
    int i,j,length,pivot,count;
    FILE *file;
    strcpy(s1,"[");
    strcpy(s,"  //ta van dat");
    strcpy(s2,"]");
    file=fopen("dictionary1.txt","w");
    if(file==NULL)
    {
        printf("Have error when opening file.Exit program.\n");
        return ;
    }
    fprintf(file,"%s\n",s1);
    Word_Mean[0]=' ';
    Word_Mean[1]=' ';
    Word_Mean[2]='{';
    for(i=0;i<MAX;i++)
	{
		length=rand()%100+40;
	    count=3;
        pivot=rand()%20+10;
        while(count<pivot)
        {
            Word_Mean[count++]=rand()%25+97;
        }
        Word_Mean[count++]='}';
        Word_Mean[count++]=':';
        Word_Mean[count++]='{';
        while(count<length-2)
        {
            Word_Mean[count++]=rand()%25+97;
        }
        Word_Mean[count++]='}';
        if(i!=MAX-1)
        {
            Word_Mean[count++]=',';
        }
	if(i==MAX-10||i==MAX-20)
	{
		fprintf(file,"%s\n",s);
	}
        Word_Mean[count]='\0';
        fprintf(file,"%s\n",Word_Mean);
    } 
    fprintf(file,"%s\n",s2);
	fclose(file);
}
