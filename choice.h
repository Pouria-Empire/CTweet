//
// Created by Pouria on ۰۲/۰۴/۲۰۲۱.
//

#ifndef SERVER_CHOICE_H
#define SERVER_CHOICE_H
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MaxSize2 1000
#define MaxSize 100000
#define Limit 300
///////////////////////

/////////////////////////////


int login(char*buffer)
{

}
int signup(char*buffer)
{
    char*username=(char*)malloc(Limit);memset(username,'\0',Limit);
    char*password=(char*)malloc(Limit);memset(password,'\0',Limit);
    username=strtok(buffer,',');
    password=strtok(NULL,',');
    /*
    char*bio=(char*)malloc(Limit);memset(bio,'\0',Limit);
    char*followers=(char*)malloc(MaxSize);memset(followers,'\0',MaxSize);
    char*followings=(char*)malloc(MaxSize);memset(followings,'\0',MaxSize);
    int*personalTweets=(int*)malloc(Limit);memset(personalTweets,0,Limit*sizeof(int));*/
    PERSONS *person=(PERSONS *)malloc(sizeof(PERSONS));
    person =newPerson(username,password);
    char*out=(char*)malloc(Limit);memset(out,'\0',MaxSize);
    int countFollowers=0;
    int countFollowings=0;
    int countPersonalTweets=0;
    cJSON *root=cJSON_CreateObject();
    cJSON *Cusername=cJSON_CreateString(person->username);
    cJSON *Cpassword=cJSON_CreateString(person->password);
    cJSON *Cbio=cJSON_CreateString(person->bio);
    cJSON *Cfollowers=cJSON_CreateStringArray(person->followers,countFollowers);
    cJSON *Cfollowings=cJSON_CreateStringArray(person->followings,countFollowings);
    cJSON *CpersonalTweets=cJSON_CreateIntArray(person->personalTweets,countPersonalTweets);
    cJSON_AddItemToObject(root,"username",Cusername);
    cJSON_AddItemToObject(root,"password",Cpassword);
    cJSON_AddItemToObject(root,"bio",Cbio);
    cJSON_AddArrayToObject(root,"followers");
    cJSON_AddArrayToObject(root,"followings");
    cJSON_AddArrayToObject(root,"personalTweets");
    out=cJSON_Print(root);
    free(person);
}
int sendTweet(char*buffer)
{

}
int refresh(char*buffer)
{

}
int like(char*buffer)
{

}
int comment(char*buffer)
{

}
int follow(char*buffer)
{

}
int unfollow(char*buffer)
{

}
int setBio(char*buffer)
{

}
int logout(char*buffer)
{

}
int changePassword(char*buffer)
{

}
int profile(char*buffer)
{

}
#endif //SERVER_CHOICE_H
