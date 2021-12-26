//
// Created by Pouria on ۰۲/۰۴/۲۰۲۱.
//

#ifndef SERVER_TJSON_H
#define SERVER_TJSON_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "cJSON.h"
#include "File.h"
//#define MaxSize 100000
#define Limit 300
#define IdLength 20
#endif //SERVER_TJSON_H
char* getTweetById(char*id)
{
    char*makedFilename=(char*)malloc(MaxSize);
    char*out=(char*)malloc(MaxSize);memset(out,'\0',MaxSize);
    char*result=(char*)malloc(MaxSize);memset(result,'\0',MaxSize);
    strcpy(makedFilename,"Tweets\\\\");
    strcat(makedFilename,id);
    strcat(makedFilename,".tweet.json");
    if(ReadData(makedFilename,result))
    {
        return result;
    }

}
char*getuser(char*buffer)
{
    cJSON *root = cJSON_Parse(buffer);

}
void getTweetsOfUser(char*name,char *output)
{

    char*makedFilename=(char*)malloc(MaxSize);
    char*out=(char*)malloc(MaxSize);memset(out,'\0',MaxSize);
    char*result=(char*)malloc(MaxSize);memset(result,'\0',MaxSize);
    char*makedFilename2=(char*)malloc(MaxSize);
    char id[IdLength]={'\0'};
    strcpy(makedFilename,"users\\\\");
    strcat(makedFilename,name);
    strcat(makedFilename,".user.json");
    if(ReadData(makedFilename,result))
    {
        cJSON *root=cJSON_Parse(result);
        cJSON *personalTweets=cJSON_GetObjectItem(root,"personalTweets");
        int arraySize=cJSON_GetArraySize(personalTweets);
        for(int i=arraySize-1;i>=0;i--)
        {
            int idInt=cJSON_GetArrayItem(personalTweets,i)->valueint;
           sprintf(id,"%d",idInt);
           if(i!=0)
               strcat(out,",");
           strcat(out,getTweetById(id));
        }
        strcpy(output,out);
    }
}
void BIGgetTweetById(char*id,int i)
{
    char*makedFilename=(char*)malloc(MaxSize);
    char*out=(char*)malloc(MaxSize);memset(out,'\0',MaxSize);
    char*result=(char*)malloc(MaxSize);memset(result,'\0',MaxSize);
    strcpy(makedFilename,"Tweets\\\\");
    strcat(makedFilename,id);
    strcat(makedFilename,".tweet.json");
    if(ReadData(makedFilename,result))
    {
        FILE *file;
        if(i==0)
            file=fopen("refresh.txt","w+");
        else
            file=fopen("refresh.txt","a+");
        cJSON *tweet = cJSON_Parse(result);
        fprintf(file,"id:%d\n",cJSON_GetObjectItem(tweet, "id")->valueint);
        fprintf(file,"\nauthor:%s\n",cJSON_GetObjectItem(tweet, "author")->valuestring);
        fprintf(file,"\ncontent:%s\n",cJSON_GetObjectItem(tweet, "content")->valuestring);
        //printf("\ncomments:%s\n",cJSON_GetObjectItem(tweet, "comments")->valuestring);
        fprintf(file,"\nlikes:%d\n",cJSON_GetObjectItem(tweet, "likes")->valueint);
        fprintf(file,"---------------------------\n");
        fclose(file);
        i=1;
    }

}
void BIGgetTweetsOfUser(char*name,char*UseUsername)
{
    int j=0;int q=1;
    char*makedFilename=(char*)malloc(MaxSize);
    char*out=(char*)malloc(MaxSize);memset(out,'\0',MaxSize);
    char*result=(char*)malloc(MaxSize);memset(result,'\0',MaxSize);
    char*result2=(char*)malloc(MaxSize);memset(result2,'\0',MaxSize);
    char*makedFilename2=(char*)malloc(MaxSize);
    char id[IdLength]={'\0'};
    strcpy(makedFilename,"users\\\\");
    strcat(makedFilename,name);
    strcat(makedFilename,".user.json");
    strcpy(makedFilename2,"users\\\\");
    strcat(makedFilename2,UseUsername);
    strcat(makedFilename2,".user.json");
    if(ReadData(makedFilename,result))
    {
        ReadData(makedFilename2,result2);
        cJSON *root=cJSON_Parse(result);
        cJSON *root2=cJSON_Parse(result2);
        cJSON *personalTweets=cJSON_GetObjectItem(root,"personalTweets");
        cJSON *seenTweets=cJSON_GetObjectItem(root2,"seenTweets");
        int seenTweetsSize=cJSON_GetArraySize(seenTweets);
        int arraySize=cJSON_GetArraySize(personalTweets);
        for(int i=arraySize-1;i>=0;i--)
        {
            int idInt=cJSON_GetArrayItem(personalTweets,i)->valueint;
            sprintf(id,"%d",idInt);
            for(int k=0;k<seenTweetsSize;k++)
            {
                if(cJSON_GetArrayItem(seenTweets,k)->valueint==idInt)
                    q=0;
            }
            if(q!=0)
            {
                cJSON *seen=cJSON_CreateNumber(cJSON_GetArrayItem(personalTweets,i)->valueint);
                cJSON_AddItemToArray(seenTweets,seen);
                BIGgetTweetById(id,j);
                j++;
            }
        }
        out=cJSON_Print(root2);
        strip(out);
        StoreData(makedFilename2,out);
    }
}