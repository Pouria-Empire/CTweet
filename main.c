#include <stdio.h>
//#include "Sconnection.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "cJSON.h"
#include "Tjson.h"
#include "File.h"
#include "Token.h"
#include "regexes.h"
#include<winsock2.h>
#include <conio.h>
#include <io.h>
#include<process.h>
#pragma comment(lib,"ws2_32.lib") //Winsock Library
#define MaxSize2 1000
//#define MaxSize 100000
#define Limit 300
#define IdLength 20

/////////
int signup(char*buffer);
int login(char*buffer);
int sendTweet(char*buffer);
char *refresh(char*buffer);
int like(char*buffer);
int comment(char*buffer);
int follow(char*buffer);
int unfollow(char*buffer);
int setBio(char*buffer);
int logout(char*buffer);
char *search(char*buffer);
int changePassword(char*buffer);
char *tweetProfile(char*buffer);
int Delete(char*buffer);
int ReTweet(char*buffer);
int APIconnect();
char *bigsearch(char*buffer);
//////////
///personal Variable
int isLogin=0;
char Token[200]= {'\0'};
char UserUsername[Limit]={'\0'};
char UserPassword[Limit]={'\0'};
///////////
//Tweet Variables
//////////
int main() {
    //printf("Hello, This Is Server!\n");
    if(!folderExist("users"))
        system("mkdir users");
    if(!folderExist("Tweets"))
        system("mkdir Tweets");
    APIconnect();
    return 0;
}
typedef struct {

    char username[Limit];
    char password[Limit];
    char bio[MaxSize];
    char followers[MaxSize];
    char followings[MaxSize];
    int personalTweets[Limit];
}PERSONS;
typedef struct {

    char author[Limit];
    char content[Limit];
    char comments[MaxSize];//list of sender and comments
    int id;
    int likes;
}TWEETS;
PERSONS *newPerson(char*username,char*password)
{
    PERSONS *person=(PERSONS*)malloc(sizeof(PERSONS));
    strcpy(person->username,username);strcpy(person->password,password);
    memset(person->followers,'\0',MaxSize);memset(person->followings,'\0',MaxSize);
    memset(person->bio,'\0',Limit);memset(person->personalTweets,0,Limit*sizeof(int));
    return person;
}
TWEETS *newTweet(int id,char*content,char*author)
{
    TWEETS *tweet=(TWEETS*)malloc(sizeof(TWEETS));
    strcpy(tweet->author,author);strcpy(tweet->content,content);
    tweet->id=id;tweet->likes=0;
    memset(tweet->comments,'\0',MaxSize);
    return tweet;
}
int login(char*buffer)
{
    if(isLogin==1)
        return 3;
    char*username=(char*)malloc(Limit);memset(username,'\0',Limit);
    char*password=(char*)malloc(Limit);memset(password,'\0',Limit);
    char*makedFilename=(char*)malloc(Limit);
    char *result=(char*)malloc(MaxSize);
    char*temp;
    temp=strtok(buffer," ,");
    temp=strtok(NULL," ,");
    strcpy(username,temp);
    temp=strtok(NULL," ,");
    strcpy(password,temp);
    strtok(password,"\r\n");
    strcpy(makedFilename,"users\\\\");
    strcat(makedFilename,username);
    strcat(makedFilename,".user.json");
    if(FileExists(makedFilename))
    {
        result=readFile(makedFilename);
      if(result!=NULL)
      {
          cJSON*root=cJSON_Parse(result);
          cJSON*Cusername=cJSON_GetObjectItem(root,"username");
          cJSON*Cpassword=cJSON_GetObjectItem(root,"password");
          if(strcmp(Cpassword->valuestring,password)==0) {
              buildToken(Token);
              strcpy(UserUsername,username);
              strcpy(UserPassword,password);
              isLogin = 1;
              return 1;
          }
          else
              return 0;
      } else
          return 2;
    }
    else
    {
        return 2;
    }
    free(result);
}
int signup(char*buffer)
{
    char*username=(char*)malloc(Limit);memset(username,'\0',Limit);
    char*password=(char*)malloc(Limit);memset(password,'\0',Limit);
    char*makedFilename=(char*)malloc(MaxSize);
    char*temp;
    temp=strtok(buffer," ,");
    temp=strtok(NULL," ,");
    strcpy(username,temp);
    temp=strtok(NULL," ,");
    strcpy(password,temp);
    strtok(password,"\r\n");
    /*
    char*bio=(char*)malloc(Limit);memset(bio,'\0',Limit);
    char*followers=(char*)malloc(MaxSize);memset(followers,'\0',MaxSize);
    char*followings=(char*)malloc(MaxSize);memset(followings,'\0',MaxSize);
    int*personalTweets=(int*)malloc(Limit);memset(personalTweets,0,Limit*sizeof(int));*/
    PERSONS person = *(newPerson(username,password));
    char*out=(char*)malloc(MaxSize);memset(out,'\0',MaxSize);
    int countFollowers=0;
    int countFollowings=0;
    int countPersonalTweets=0;
    cJSON *root=cJSON_CreateObject();
    cJSON *Cusername=cJSON_CreateString(person.username);
    cJSON *Cpassword=cJSON_CreateString(person.password);
    cJSON *Cbio=cJSON_CreateString(person.bio);
   // cJSON *Cfollowers=cJSON_CreateStringArray(person.followers,countFollowers);
   // cJSON *Cfollowings=cJSON_CreateStringArray(person.followings,countFollowings);
   // cJSON *CpersonalTweets=cJSON_CreateIntArray(person.personalTweets,countPersonalTweets);
    cJSON_AddItemToObject(root,"username",Cusername);
    cJSON_AddItemToObject(root,"password",Cpassword);
    cJSON_AddItemToObject(root,"bio",Cbio);
    cJSON_AddArrayToObject(root,"followers");
    cJSON_AddArrayToObject(root,"followings");
    cJSON_AddArrayToObject(root,"personalTweets");
    cJSON_AddArrayToObject(root,"seenTweets");
    cJSON_AddArrayToObject(root,"retweet");
    out=cJSON_Print(root);
    strip(out);
    strcpy(makedFilename,"users\\\\");
    strcat(makedFilename,username);
    strcat(makedFilename,".user.json");
    if(!FileExists(makedFilename)){
        StoreData(makedFilename,out);
        return 1;
    } else
        return 0;

}
int sendTweet(char*buffer)
{
    char*content=(char*)malloc(Limit);memset(content,'\0',Limit);
    char*result=(char*)malloc(MaxSize);memset(result,'\0',MaxSize);
    char ids[IdLength]={'\0'};
    char*makedFilename=(char*)malloc(MaxSize);
    int id =findTheLastFile();
    char*temp;
    buffer+=13+strlen(Token)-1;
    strtok(buffer,"\n\r");
    strcpy(content,buffer);
    TWEETS tweet = *newTweet(id,content,UserUsername);//UserUsername or author
    char*out=(char*)malloc(MaxSize);memset(out,'\0',MaxSize);
    int countFollowers=0;
    int countFollowings=0;
    int countPersonalTweets=0;
    cJSON *root=cJSON_CreateObject();
    cJSON *Cid=cJSON_CreateNumber(tweet.id);
    cJSON *Cauthor=cJSON_CreateString(tweet.author);
    cJSON *Ccontent=cJSON_CreateString(tweet.content);
    cJSON *RCcommments=cJSON_CreateObject();
    cJSON *Clikes=cJSON_CreateNumber(tweet.likes);
    cJSON_AddItemToObject(root,"id",Cid);
    cJSON_AddItemToObject(root,"author",Cauthor);
    cJSON_AddItemToObject(root,"content",Ccontent);
    cJSON_AddItemReferenceToObject(root,"comments",RCcommments);
    cJSON_AddItemToObject(root,"likes",Clikes);
    cJSON_AddArrayToObject(root,"likedBy");
    out=cJSON_Print(root);
    strip(out);
    strcpy(makedFilename,"Tweets\\\\");
    sprintf(ids,"%d",id);
    strcat(makedFilename,ids);
    strcat(makedFilename,".tweet.json");
    StoreData(makedFilename,out);
    //reading data to add personal tweets
    free(root);free(Cid);free(Ccontent);free(Clikes);free(RCcommments);free(out);
    strcpy(makedFilename,"users\\\\");
    strcat(makedFilename,UserUsername);
    strcat(makedFilename,".user.json");
    if(ReadData(makedFilename,result)) {
        char*out=(char*)malloc(MaxSize);memset(out,'\0',MaxSize);
        cJSON *root=cJSON_Parse(result);
        cJSON *AddTweet=cJSON_CreateNumber(id);
        cJSON *getedArray=cJSON_GetObjectItem(root,"personalTweets");
        cJSON_AddItemToArray(getedArray,AddTweet);
        out=cJSON_Print(root);
        strip(out);
        StoreData(makedFilename,out);
        free(result);free(makedFilename);free(out);
        return 1;

    } else
        return 0;
}
char* refresh(char*buffer)
{
    char*makedFilename=(char*)malloc(MaxSize);
    char*result=(char*)malloc(MaxSize);memset(result,'\0',MaxSize);
    char*out=(char*)malloc(MaxSize);memset(out,'\0',MaxSize);
    char nameOfFollower[Limit];
    char*getTweets=(char*)malloc(MaxSize);memset(getTweets,'\0',MaxSize);
    strcpy(makedFilename,"users\\\\");
    strcat(makedFilename,UserUsername);
    strcat(makedFilename,".user.json");
    if(ReadData(makedFilename,result))
    {
        cJSON *root=cJSON_Parse(result);
        cJSON *followers=cJSON_GetObjectItem(root,"followers");
        int followersSize=cJSON_GetArraySize(followers);
        for(int i=0;i<followersSize;i++)
        {
          strcpy( nameOfFollower,cJSON_GetArrayItem(followers,i)->valuestring);
          getTweetsOfUser(nameOfFollower,result);
          BIGgetTweetsOfUser(nameOfFollower,UserUsername);
          if(result[0]!='\0'){
              if(i!=0)
                  strcat(getTweets,",");
              strcat(getTweets,result);
              memset(result,'\0',MaxSize);
          }

        }
        return getTweets;
    } else return '\0';
}
int like(char*buffer)
{
    buffer+=7+strlen(Token);
    strtok(buffer,"\n\r");
    char id[IdLength]={'\0'};
    strcpy(id,buffer);
    char*makedFilename=(char*)malloc(MaxSize);
    char*result=(char*)malloc(MaxSize);memset(result,'\0',MaxSize);
    char*out=(char*)malloc(MaxSize);memset(out,'\0',MaxSize);
    strcpy(makedFilename,"Tweets\\\\");
    strcat(makedFilename,id);
    strcat(makedFilename,".tweet.json");
    if(ReadData(makedFilename,result))
    {
        cJSON *root=cJSON_Parse(result);
        cJSON *likes=cJSON_GetObjectItem(root,"likes");
        cJSON *likedBy=cJSON_GetObjectItem(root,"likedBy");
        int likeSize=cJSON_GetArraySize(likedBy);
        for(int i=0;i<likeSize;i++)
        {
            if(strcmp(cJSON_GetArrayItem(likedBy,i)->valuestring,UserUsername)==0)
            {
                return 2;
            }
        }
        cJSON *UserLiked=cJSON_CreateString(UserUsername);
        cJSON_AddItemToArray(likedBy,UserLiked);
        int newlikes=likes->valueint+1;
        cJSON *Cnewlikes=cJSON_CreateNumber(newlikes);
        cJSON_ReplaceItemInObject(root,"likes",Cnewlikes);
        out=cJSON_Print(root);
        strip(out);
        strcpy(makedFilename,"Tweets\\\\");
        strcat(makedFilename,id);
        strcat(makedFilename,".tweet.json");
        StoreData(makedFilename,out);
        free(result);free(makedFilename);free(out);
        return 1;
    } else
        return 0;
}
int comment(char*buffer)
{
    char*makedFilename=(char*)malloc(MaxSize);
    char*result=(char*)malloc(MaxSize);memset(result,'\0',MaxSize);
    char*out=(char*)malloc(MaxSize);memset(out,'\0',MaxSize);
    char*userComment=(char*)malloc(Limit);
    char*userId=(char*)malloc(Limit);
    buffer+=10+strlen(Token)-1;
    //Remmeber To check!
    userId=strtok(buffer," ,");
    userComment=strtok(NULL,"\n");//
    //
    strcpy(makedFilename,"Tweets\\\\");
    strcat(makedFilename,userId);
    strcat(makedFilename,".tweet.json");
    if(ReadData(makedFilename,result))
    {
        char Usercpy[Limit];
        strcpy(Usercpy,UserUsername);
        strcat(Usercpy,":");
        cJSON *root=cJSON_Parse(result);
        cJSON *comment=cJSON_GetObjectItem(root,"comments");//as Object
        cJSON *CuserComment=cJSON_CreateString(userComment);

        cJSON_AddItemToObject(comment,UserUsername,CuserComment);
        //cJSON_DeleteItemFromObject(root,"comments");
       // cJSON_AddObjectToObject(root,"comments");
        out=cJSON_Print(root);
        strip(out);
        StoreData(makedFilename,out);
        free(root);free(comment);free(CuserComment);
        free(result);free(makedFilename);free(out);
        return 1;
    } else return 0;
}
int follow(char*buffer)
{
    char*makedFilename=(char*)malloc(MaxSize);
    char*result=(char*)malloc(MaxSize);memset(result,'\0',MaxSize);
    char*out=(char*)malloc(MaxSize);memset(out,'\0',MaxSize);
    buffer+=9+strlen(Token);
    char*userTofollow=(char*)malloc(Limit);
    strtok(buffer," \n");
    strcpy(userTofollow,buffer);
    strcpy(makedFilename,"users\\\\");
    strcat(makedFilename,UserUsername);
    strcat(makedFilename,".user.json");
    if(ReadData(makedFilename,result))
    {
        cJSON *root=cJSON_Parse(result);
        cJSON *followings=cJSON_GetObjectItem(root,"followings");
        int followingsSize=cJSON_GetArraySize(followings);
        for(int i=0;i<followingsSize;i++)
        {
            if(strcmp(cJSON_GetArrayItem(followings,i)->valuestring,userTofollow)==0)
            {
                return 2;
            }
        }
        cJSON *newFollower=cJSON_CreateString(userTofollow);
        cJSON_AddItemToArray(followings,newFollower);
        out=cJSON_Print(root);
        strip(out);
        StoreData(makedFilename,out);
        strcpy(makedFilename,"\0");
        strcpy(result,"\0");
        strcpy(out,"\0");
        makedFilename=makedFileUser(userTofollow);
        ReadData(makedFilename,result);
        cJSON *root2=cJSON_Parse(result);
        cJSON *followers=cJSON_GetObjectItem(root2,"followers");
        cJSON *thisUser=cJSON_CreateString(UserUsername);
        cJSON_AddItemToArray(followers,thisUser);
        out=cJSON_Print(root2);
        strip(out);
        StoreData(makedFilename,out);

        free(followings);free(root);free(newFollower);
        free(result);free(makedFilename);free(out);
        return 1;

    } else
        return 0;



}
int unfollow(char*buffer)
{
    char*makedFilename=(char*)malloc(MaxSize);
    char*result=(char*)malloc(MaxSize);memset(result,'\0',MaxSize);
    char*out=(char*)malloc(MaxSize);memset(out,'\0',MaxSize);
    buffer+=11+strlen(Token);
    strtok(buffer,"\n");
    char*userToUnfollow=(char*)malloc(Limit);
    strcpy(userToUnfollow,buffer);
    strcpy(makedFilename,"users\\\\");
    strcat(makedFilename,UserUsername);
    strcat(makedFilename,".user.json");
    if(ReadData(makedFilename,result))
    {
        cJSON *root=cJSON_Parse(result);
        cJSON *followings=cJSON_GetObjectItem(root,"followings");
        int followingsSize=cJSON_GetArraySize(followings);
        int counter=0;
        for(int i=0;i<followingsSize;i++)
        {
            if(strcmp(cJSON_GetArrayItem(followings,i)->valuestring,userToUnfollow)==0)
            {
                counter++;
                cJSON_DeleteItemFromArray(followings,i);
            }
        }
        if(counter==0)
            return 2;
        else{
            out=cJSON_Print(root);
            strip(out);
            StoreData(makedFilename,out);
            strcpy(makedFilename,"\0");
            strcpy(result,"\0");
            strcpy(out,"\0");
            makedFilename=makedFileUser(userToUnfollow);
            ReadData(makedFilename,result);
            cJSON *root2=cJSON_Parse(result);
            cJSON *followers=cJSON_GetObjectItem(root2,"followers");
            int arraySize=cJSON_GetArraySize(followers);
            for(int i=0;i<arraySize;i++)
            {
                if(strcmp(cJSON_GetArrayItem(followers,i)->valuestring,UserUsername)==0)
                {
                    cJSON_DeleteItemFromArray(followers,i);
                }
            }
            out=cJSON_Print(root2);
            strip(out);
            StoreData(makedFilename,out);
            free(followings);free(root);
            free(result);free(makedFilename);free(out);
            return 1;
        }


    } else
        return 0;

}
int setBio(char*buffer)
{
    char*makedFilename=(char*)malloc(MaxSize);
    char*result=(char*)malloc(MaxSize);memset(result,'\0',MaxSize);
    char*out=(char*)malloc(MaxSize);memset(out,'\0',MaxSize);
    buffer+=10+strlen(Token)-1;
    strtok(buffer,"\n\r");
    char*bio=(char*)malloc(Limit);
    strcpy(bio,buffer);
    strcpy(makedFilename,"users\\\\");
    strcat(makedFilename,UserUsername);
    strcat(makedFilename,".user.json");
    if(ReadData(makedFilename,result)) {
        cJSON *root=cJSON_Parse(result);
        cJSON *CreadBio=cJSON_GetObjectItem(root,"bio");
        char *readbio=(char*)malloc(Limit);
        strcpy(readbio,CreadBio->valuestring);
        if(readbio[0]!='\0')
            return 2;
        //check
        //cJSON_DeleteItemFromObject(root,"bio");
        cJSON *Cbio=cJSON_CreateString(bio);
        cJSON_AddItemToObject(root,"bio",Cbio);
        out=cJSON_Print(root);
        strip(out);
        StoreData(makedFilename,out);
        free(root);free(Cbio);free(CreadBio);free(readbio);
        free(result);free(makedFilename);free(out);
        return 1;
    }
    else
    {
        free(result);free(makedFilename);free(out);
        return 0;
    }
}
int logout(char*buffer)
{
    strcpy(Token,"\0");
    strcpy(UserUsername,"\0");
    strcpy(UserPassword,"\0");
    isLogin=0;
    return 1;

}
char* search(char*buffer)
{
    char*makedFilename=(char*)malloc(MaxSize);
    char*result=(char*)malloc(MaxSize);memset(result,'\0',MaxSize);
    char*out=(char*)malloc(MaxSize);memset(out,'\0',MaxSize);
    char status[15];char id[IdLength]={'\0'};
    buffer+=9+strlen(Token);
    strtok(buffer,"\n\r");
    char*User=(char*)malloc(Limit);
    strcpy(User,buffer);
    strcpy(makedFilename,"users\\\\");
    strcat(makedFilename,User);
    strcat(makedFilename,".user.json");
   // strcpy(makedFilename,"users\\\\pouria.user.json");
    int numberOfFollowings;
    int numberOfFollowers;
    int followStatus;//1 YES 0 NO
    char Userbio[Limit];
    if(FileExists(makedFilename))
    {
        ///building answer
        ReadData(makedFilename,result);
        //First of all mine the data I want
        cJSON *root=cJSON_Parse(result);
        cJSON *Cbio=cJSON_GetObjectItem(root,"bio");
        cJSON *followers=cJSON_GetObjectItem(root,"followers");
        cJSON *followings=cJSON_GetObjectItem(root,"followings");
        cJSON *personalTweets=cJSON_GetObjectItem(root,"personalTweets");
        strcpy(Userbio,Cbio->valuestring);
        numberOfFollowings=cJSON_GetArraySize(followings);
        numberOfFollowers=cJSON_GetArraySize(followers);
        int personalTweetSize=cJSON_GetArraySize(personalTweets);
        numberOfFollowers=cJSON_GetArraySize(followers);
        for(int i=0;i<numberOfFollowers;i++)
        {
            if(strcmp(UserUsername,cJSON_GetArrayItem(followers,i)->valuestring)==0)
                followStatus=1;
        }
        //making an answer :)
        cJSON *message=cJSON_CreateObject();
        cJSON *C2username=cJSON_CreateString(User);
        cJSON *C2bio=cJSON_CreateString(Userbio);
        if(followStatus==1)
            strcpy(status,"Followed");
        else
            strcpy(status,"NotFollowed");
        cJSON *C2followStatus=cJSON_CreateString(status);
        cJSON *allTweets=cJSON_CreateArray();
        cJSON_AddItemToObject(message,"username",C2username);
        cJSON_AddItemToObject(message,"bio",C2bio);
        cJSON_AddNumberToObject(message,"numberOfFollowers",numberOfFollowers);
        cJSON_AddNumberToObject(message,"numberOfFollowings",numberOfFollowings);
        cJSON_AddItemToObject(message,"followStatus",C2followStatus);
        cJSON *Cresult;
        for(int i=0;i<personalTweetSize;i++)
        {
            int idInt=cJSON_GetArrayItem(personalTweets,i)->valueint;
            sprintf(id,"%d",idInt);
            Cresult=cJSON_Parse(getTweetById(id));
            cJSON_AddItemToArray(allTweets,Cresult);
        }
        cJSON_AddItemToObject(message,"allTweets",allTweets);
        out=cJSON_Print(message);
        strip_s(out);
        free(makedFilename);free(result);free(User);free(Cbio);free(followers);free(followings);
        free(root);free(C2followStatus);free(C2username);free(allTweets);free(Cresult);
        return out;
    } else
    {
        free(result);free(makedFilename);free(out);free(User);
        return '\0';
    }

}
int changePassword(char*buffer)
{
    char*makedFilename=(char*)malloc(MaxSize);
    char*result=(char*)malloc(MaxSize);memset(result,'\0',MaxSize);
    char*out=(char*)malloc(MaxSize);
    char*newPassword=(char*)malloc(Limit);
    buffer+=20+strlen(Token)+strlen(UserPassword);
    strtok(buffer,"\n ,");
    strcpy(newPassword,buffer);
    strcpy(makedFilename,"users\\\\");
    strcat(makedFilename,UserUsername);
    strcat(makedFilename,".user.json");
    ReadData(makedFilename,result);
    if(strcmp(newPassword,UserPassword)==0)
    {
        return 3;
    }
    else if(strcmp(newPassword,UserPassword)!=0)
    {
        if(strlen(newPassword)>16)
            return 2;
        cJSON *root=cJSON_Parse(result);
        cJSON *Cpassword=cJSON_CreateString(newPassword);
        cJSON_ReplaceItemInObject(root,"password",Cpassword);
        out=cJSON_Print(root);
        strip(out);
        StoreData(makedFilename,out);
        free(root);free(Cpassword);free(result);free(newPassword);free(out);
        return 1;

    } else
    {
        free(result);free(newPassword);free(out);
        return 0;
    }
}
char *tweetProfile(char*buffer)
{
    char*makedFilename=(char*)malloc(MaxSize);
    char*result=(char*)malloc(MaxSize);memset(result,'\0',MaxSize);
    char*out=(char*)malloc(MaxSize);memset(out,'\0',MaxSize);
    char status[15];char id[IdLength]={'\0'};
    char*User=(char*)malloc(MaxSize);
    strcpy(User,UserUsername);
    strcpy(makedFilename,"users\\\\");
    strcat(makedFilename,UserUsername);
    strcat(makedFilename,".user.json");
    // strcpy(makedFilename,"users\\\\pouria.user.json");
    int numberOfFollowings;
    int numberOfFollowers;
    int followStatus;//1 YES 0 NO
    char Userbio[Limit];
    if(FileExists(makedFilename))
    {
        ///building answer
        ReadData(makedFilename,result);
        //First of all mine the data I want
        cJSON *root=cJSON_Parse(result);
        cJSON *Cbio=cJSON_GetObjectItem(root,"bio");
        cJSON *followers=cJSON_GetObjectItem(root,"followers");
        cJSON *followings=cJSON_GetObjectItem(root,"followings");
        cJSON *personalTweets=cJSON_GetObjectItem(root,"personalTweets");
        strcpy(Userbio,Cbio->valuestring);
        numberOfFollowings=cJSON_GetArraySize(followings);
        numberOfFollowers=cJSON_GetArraySize(followers);
        int personalTweetSize=cJSON_GetArraySize(personalTweets);
        for(int i=0;i<numberOfFollowers-1;i++)
        {
            if(strcmp(UserUsername,cJSON_GetArrayItem(followers,i)->valuestring)==0)
                followStatus=1;
        }
        //making an answer :)
        cJSON *message=cJSON_CreateObject();
        cJSON *C2username=cJSON_CreateString(User);
        cJSON *C2bio=cJSON_CreateString(Userbio);
        if(followStatus==1)
            strcpy(status,"Followed");
        else
            strcpy(status,"NotFollowed");
        cJSON *C2followStatus=cJSON_CreateString(status);
        cJSON *allTweets=cJSON_CreateArray();
        cJSON_AddItemToObject(message,"username",C2username);
        cJSON_AddItemToObject(message,"bio",C2bio);
        cJSON_AddNumberToObject(message,"numberOfFollowers",numberOfFollowers);
        cJSON_AddNumberToObject(message,"numberOfFollowings",numberOfFollowings);
        cJSON_AddItemToObject(message,"followStatus",C2followStatus);
        cJSON *Cresult;
        for(int i=0;i<personalTweetSize;i++)
        {
            int idInt=cJSON_GetArrayItem(personalTweets,i)->valueint;
            sprintf(id,"%d",idInt);
            Cresult=cJSON_Parse(getTweetById(id));
            cJSON_AddItemToArray(allTweets,Cresult);
        }
        cJSON_AddItemToObject(message,"allTweets",allTweets);
        out=cJSON_Print(message);
        strip_s(out);
        free(makedFilename);free(result);free(Cbio);free(followers);free(followings);
        free(root);free(C2followStatus);free(C2username);free(allTweets);free(Cresult);
        return out;
    } else
    {
        free(result);free(makedFilename);free(out);
        return '\0';
    }
}
int Delete(char*buffer)
{
    int isFound=0;
    buffer+=8+strlen(Token);
    strtok(buffer,",");
    char ID[Limit];
    strcpy(ID,strtok(buffer,"\n\r"));
    char*makedFilename=(char*)malloc(MaxSize);
    char*retweetName=(char*)malloc(MaxSize);
    char*result=(char*)malloc(MaxSize);memset(result,'\0',MaxSize);
    char*result2=(char*)malloc(MaxSize);memset(result,'\0',MaxSize);
    char*out=(char*)malloc(MaxSize);memset(out,'\0',MaxSize);
    char status[15];char id[IdLength]={'\0'};
    strcpy(makedFilename,"users\\\\");
    strcat(makedFilename,UserUsername);
    strcat(makedFilename,".user.json");
    if(ReadData(makedFilename,result))
    {
        cJSON *root=cJSON_Parse(result);
        cJSON *personalTweets=cJSON_GetObjectItem(root,"personalTweets");
        int arraySize=cJSON_GetArraySize(personalTweets);
        for(int i=0;i<arraySize;i++)
        {
            if(cJSON_GetArrayItem(personalTweets,i)->valueint==atoi(ID))
            {
                isFound=1;
//                retweetName=makedFileTweet(ID);
//                ReadData(retweetName,result);
                cJSON *retweet=cJSON_GetObjectItem(root,"retweet");
                cJSON_DeleteItemFromArray(personalTweets,i);
                if(retweet!=NULL)
                {
                    int retweetsize=cJSON_GetArraySize(retweet);
                    for(int j=0;j<retweetsize;j++)
                    {
                     char *userForDelete;
                     userForDelete=makedFileUser(cJSON_GetArrayItem(retweet,j)->valuestring);
                        ReadData(retweetName,result);
                        cJSON *root3=cJSON_Parse(result);
                           cJSON *getedArray=cJSON_GetObjectItem(root3,"personalTweets");
                           int personaltweetArraySize=cJSON_GetArraySize(getedArray);
                           for(int k=0;k<personaltweetArraySize;k++)
                           {
                               if(cJSON_GetArrayItem(getedArray,k)->valueint/99999!=0)
                               {
                                   cJSON_DeleteItemFromArray(getedArray,k);
                               }
                           }

                        out=cJSON_Print(root3);StoreData(userForDelete,out);
                    }
                }
            }

        }
        if(isFound==1)
        {
            out=cJSON_Print(root);
            strip_s(out);
            StoreData(makedFilename,out);
            strcpy(makedFilename,"Tweets\\\\");
            strcat(makedFilename,ID);
            strcat(makedFilename,".tweet.json");
            int del = remove(makedFilename);
            if(!del)
                return 1;
            else
                return 3;
        } else{
            return 2;
        }
    } else return 0;

}
int ReTweet(char*buffer)
{
    char tweetId[20]={'\0'};
    char*makedFilename=(char*)malloc(MaxSize);
    char*nameOfTweet=(char*)malloc(MaxSize);
    char*result=(char*)malloc(MaxSize);memset(result,'\0',MaxSize);
    char*out=(char*)malloc(MaxSize);memset(out,'\0',MaxSize);
    buffer+=32+10;
    strtok(buffer,"\n");
    strcpy(tweetId,buffer);
    strcpy(makedFilename,"Tweets\\\\");
    strcat(makedFilename,tweetId);
    strcat(makedFilename,".tweet.json");
    strcpy(nameOfTweet,makedFilename);
    if(ReadData(makedFilename,result))
    {
        cJSON *root=cJSON_Parse(result);
        cJSON *retweet=cJSON_GetObjectItem(root,"retweet");
        if(retweet==NULL)
        {
            cJSON *Cretweet=cJSON_CreateArray();
            cJSON *User=cJSON_CreateString(UserUsername);
            cJSON_AddItemToArray(Cretweet,User);
            cJSON_AddItemToObject(root,"retweet",Cretweet);
        } else{
            cJSON *Cretweet=cJSON_GetObjectItem(root,"retweet");
            cJSON *User=cJSON_CreateString(UserUsername);
            cJSON_AddItemToArray(Cretweet,User);
        }
        cJSON *content=cJSON_GetObjectItem(root,"content");
        cJSON *author=cJSON_GetObjectItem(root,"author");
        char newTweet[MaxSize];
        strcpy(newTweet,"(");strcat(newTweet,author->valuestring);strcat(newTweet,")");
        strcat(newTweet,content->valuestring);
        cJSON *CnewTweet=cJSON_CreateString(newTweet);
        cJSON_ReplaceItemInObject(root,"content",CnewTweet);
        int next=findTheLastFile();
        next+=9999900;
        strcpy(tweetId,"\0");sprintf(tweetId,"%d",next);
        cJSON *CnewId=cJSON_CreateNumber(next);
        cJSON_ReplaceItemInObject(root,"id",next);
        makedFilename=makedFileTweet(tweetId);
        out=cJSON_Print(root);
        strip_s(out);
        StoreData(makedFilename,out);
        free(root);free(retweet);
        strcpy(makedFilename,"\0");
        strcpy(out,"\0");strcpy(result,"\0");
        makedFilename=makedFileUser(UserUsername);
        ReadData(makedFilename,result);
        cJSON *root2=cJSON_Parse(result);
        cJSON *addrRetweet=cJSON_GetObjectItem(root2,"addrRetweet");
        if(addrRetweet==NULL)
        {
            cJSON *CaddrRetweet=cJSON_CreateArray();
            cJSON *CnameOfTweet=cJSON_CreateString(nameOfTweet);
            cJSON_AddItemToArray(CaddrRetweet,CnameOfTweet);
            cJSON_AddItemToObject(root2,"addrRetweet",CaddrRetweet);
        } else{
            cJSON *CaddrRetweet=cJSON_GetObjectItem(root2,"addrRetweet");
            cJSON *CnameOfTweet=cJSON_CreateString(nameOfTweet);
            cJSON_AddItemToArray(CaddrRetweet,CnameOfTweet);
        }
        cJSON *AddTweet=cJSON_CreateNumber(next);
        cJSON *getedArray=cJSON_GetObjectItem(root,"personalTweets");
        cJSON_AddItemToArray(getedArray,AddTweet);
        out=cJSON_Print(root2);
        strip(out);
        StoreData(makedFilename,out);
        free(result);free(makedFilename);
        return 1;
    }
    else return 0;

}
char *bigsearch(char*buffer)
{
    char tomatch[Limit];
    char*path="users\\\\";
    char*result=(char*)malloc(MaxSize);
    buffer+=10;
    strtok(buffer,"\n\r");
    strcpy(tomatch,buffer);
    strcpy(result,"cd users && dir /s *");strcat(result,tomatch);
    strcat(result,"* > test.txt");
    system("cd users && dir /s *po* > test.txt");
    result=BigSearch(path,tomatch);
    return result;
}

int APIconnect()
{
    WSADATA wsa;
    SOCKET s , new_socket;
    struct sockaddr_in server , client;
    int c;
    char *message,*command,*type,*request,*buffer,*buffercpy;
    message=(char*)malloc(MaxSize2);
    command=(char*)malloc(MaxSize2);
    type=(char*)malloc(MaxSize2);
    request=(char*)malloc(MaxSize2);
    buffer=(char*)malloc(MaxSize2);buffercpy=(char*)malloc(MaxSize2);
    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        return 1;
    }

    printf("Initialised.\n");

    //Create a socket
    if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d" , WSAGetLastError());
    }

    printf("Socket created.\n");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 12345 );

    //Bind
    if( bind(s ,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR)
    {
        printf("Bind failed with error code : %d" , WSAGetLastError());
    }

    puts("Bind done");

    //Listen to incoming connections
    listen(s , 3);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");

    do {
        memset(buffer,'\0',MaxSize2);
        memset(command,'\0',MaxSize2);
        c = sizeof(struct sockaddr_in);
        new_socket = accept(s , (struct sockaddr *)&client, &c);
        if (new_socket == INVALID_SOCKET)
        {
            printf("accept failed with error code : %d" , WSAGetLastError());
        }

        puts("Connection accepted");

        //Reply to client
        /* message = "Recieving data From Client until 120 seconds\n";
         send(new_socket , message , strlen(message) , 0);*/



        int iResult = recv(new_socket, buffer, MaxSize2, 0);
        if (iResult > 0)
        {
            memcpy(buffercpy,buffer,MaxSize2);
            printf("Bytes received: %d\n", iResult);
            command= strtok(buffercpy," ");
            if(strcmp(command,"login")==0)
            {
                int lbuffer=login(buffer);
                if(lbuffer==1)
                {
                    char makedMessage[Limit];
                    strcpy(makedMessage,"{\"type\":\"Successful\",\"message\":\"");
                    strcat(makedMessage,Token);
                    strcat(makedMessage,"\"}");
                    strcpy(message,makedMessage);
                    //strcat(Token,",");
                }
                else if(lbuffer==2)
                {
                    strcpy(message,"{\"type\":\"Error\",\"message\":\"This username is not valid.\"}");
                }
                else if(lbuffer==3)
                {
                    strcpy(message,"{\"type\":\"Error\",\"message\":\"The user is already logged in.\"}");
                }
                else if(lbuffer==0)
                {
                    strcpy(message,"{\"type\":\"Error\",\"message\":\"Incorrect password.\"}");
                }
            }
            else if(strcmp(command,"signup")==0)
            {
                if(signup(buffer))
                {
                    strcpy(message,"{\"type\":\"Successful\",\"message\":\"\"}");
                }
                else
                {
                    strcpy(message,"{\"type\":\"Error\",\"message\":\"This username is already taken.\"}");
                }
            }
            else if(strcmp(command,"send")==0)//send tweet
            {
                command=strtok(NULL," ,");
                if(strcmp(command,"tweet")==0)
                {
                    command=strtok(NULL," ,");
                    if(strcmp(command,Token)==0)
                    {
                      if(sendTweet(buffer))
                          strcpy(message,"{\"type\":\"Successful\",\"message\":\"Tweet is sent successfully.\"}");
                    } else
                    {
                        //edit needded
                        strcpy(message,"{\"type\":\"Error\",\"message\":\"Token NotValid\"}");
                    }
                }
            }
            else if(strcmp(command,"refresh")==0)
            {
                command=strtok(NULL," ");
                strtok(command,"\n\r");
                strtok(Token,",");
                if(strcmp(command,Token)==0)
                {
                    char *gettweets=(char*)malloc(MaxSize);memset(gettweets,'\0',MaxSize);
                    char mixedresult[MaxSize];
                    strcpy(gettweets,refresh(buffer));
                    if(gettweets[0]!='\0')
                    {
                        strcpy(mixedresult,"{\"type\":\"List\",\"message\":[");
                        strcat(mixedresult,gettweets);
                        strcat(mixedresult,"]}");
                        strcpy(message,mixedresult);
                    } else
                    {
                        strcpy(message,"{\"type\":\"Error\",\"message\":\"No tweet Yet!\"}");
                    }
                } else
                {
                    strcpy(message,"{\"type\":\"Error\",\"message\":\"Authentication Failed!\"}");
                }
            }
            else if(strcmp(command,"like")==0)
            {
                command=strtok(NULL," ,");

                if(strcmp(command,Token)==0)
                {
                    int Liked=like(buffer);
                    if(Liked==1)
                        strcpy(message,"{\"type\":\"Successful\",\"message\":\"Liked has been submited succesfully\"}");
                    else if(Liked==2)
                        strcpy(message,"{\"type\":\"Error\",\"message\":\"You have Already liked this tweet!\"}");
                    else
                        strcpy(message,"{\"type\":\"Error\",\"message\":\"Eror while submitting\"}");
                } else
                {
                    strcpy(message,"{\"type\":\"Error\",\"message\":\"Authentication Failed!\"}");
                }

            }
            else if(strcmp(command,"comment")==0)
            {
                // remmeber that the user cant like and comment his tweets;
                command=strtok(NULL," ,");
                if(strcmp(command,Token)==0)
                {
                    if(comment(buffer))
                        strcpy(message,"{\"type\":\"Successful\",\"message\":\"comment Submited!\"}");
                } else
                {
                    strcpy(message,"{\"type\":\"Error\",\"message\":\"Authentication Failed!\"}");
                }
            }
            else if(strcmp(command,"search")==0)
            {
                command=strtok(NULL," ,");
                if(strcmp(command,Token)==0)
                {
                    char *gettweets=(char*)malloc(MaxSize);
                    char mixedresult[MaxSize];
                    strcpy(gettweets,search(buffer));
                    if(gettweets[0]!='\0')
                    {
                        strcpy(mixedresult,"{\"type\":\"Profile\",\"message\":");
                        strcat(mixedresult,gettweets);
                        strcat(mixedresult,"}");
                        strcpy(message,mixedresult);
                    } else
                    {
                        strcpy(message,"{\"type\":\"Error\",\"message\":\"User Not Found\"}");
                    }
                } else
                {
                    strcpy(message,"{\"type\":\"Error\",\"message\":\"Authentication Failed!\"}");
                }
            }
            else if(strcmp(command,"follow")==0)
            {
                command=strtok(NULL," ,");
                if(strcmp(command,Token)==0)
                {
                    int valuefollow=follow(buffer);
                    if(valuefollow==1)
                    {
                        strcpy(message,"{\"type\":\"Successful\",\"message\":\"User followed Successfully\"}");
                    }
                    else if(valuefollow==2)
                    {
                        strcpy(message,"{\"type\":\"Error\",\"message\":\"The user that you select is is Already in your followers!\"}");
                    } else{
                        strcpy(message,"{\"type\":\"Error\",\"message\":\"Following Failed!\"}");
                    }
                } else
                {
                    strcpy(message,"{\"type\":\"Error\",\"message\":\"Authentication Failed!\"}");
                }
            }
            else if(strcmp(command,"unfollow")==0)
            {
                command=strtok(NULL," ,");
                if(strcmp(command,Token)==0)
                {
                    int valueUnfollow=unfollow(buffer);
                    if(valueUnfollow==1)
                    {
                        strcpy(message,"{\"type\":\"Successful\",\"message\":\"User unfollowed Successfully\"}");
                    }
                    else if(valueUnfollow==2)
                    {
                        strcpy(message,"{\"type\":\"Error\",\"message\":\"The user that you select is not in the followers Yet!\"}");
                    } else{
                        strcpy(message,"{\"type\":\"Error\",\"message\":\"Unfollowing Failed!\"}");
                    }
                } else
                {
                    strcpy(message,"{\"type\":\"Error\",\"message\":\"Authentication Failed!\"}");
                }
            }
            else if(strcmp(command,"set")==0)//set bio command
            {
                command=strtok(NULL," ");
                if(strcmp(command,"bio")==0)
                {
                    command=strtok(NULL," ,");
                    if(strcmp(command,Token)==0)
                    {
                        int valuebio=setBio(buffer);
                        if(valuebio==1)
                        {
                            strcpy(message,"{\"type\":\"Successful\",\"message\":\"User Bio Successfully set\"}");
                        }
                        else if(valuebio==2)
                        {
                            strcpy(message,"{\"type\":\"Error\",\"message\":\"The bio has already set!\"}");
                        } else{
                            strcpy(message,"{\"type\":\"Error\",\"message\":\"setting bio Failed!\"}");
                        }
                    } else
                    {
                        strcpy(message,"{\"type\":\"Error\",\"message\":\"Authentication Failed!\"}");
                    }
                }
            }
            else if(strcmp(command,"logout")==0)
            {
                command=strtok(NULL,"\n\r");
                    if(strcmp(command,Token)==0)
                    {
                        if(logout(buffer))
                        strcpy(message,"{\"type\":\"Successful\",\"message\":\"User Successfully loged out\"}");
                        else
                        strcpy(message,"{\"type\":\"Error\",\"message\":\"Failed on loging out\"}");
                    } else
                    {
                        strcpy(message,"{\"type\":\"Error\",\"message\":\"Authentication Failed!\"}");
                    }
            }
            else if(strcmp(command,"profile")==0)
            {
              command=strtok(NULL," ,");
              strtok(command,"\n\r");
                if(strcmp(command,Token)==0)
                {
                    char *gettweets=(char*)malloc(MaxSize);
                    char mixedresult[MaxSize];
                    strcpy(gettweets,tweetProfile(buffer));
                    if(gettweets[0]!='\0')
                    {
                        strcpy(mixedresult,"{\"type\":\"Profile\",\"message\":");
                        strcat(mixedresult,gettweets);
                        strcat(mixedresult,"}");
                        strcpy(message,mixedresult);
                    } else
                    {
                        strcpy(message,"{\"type\":\"Error\",\"message\":\"No tweets Found\"}");
                    }
                } else
                {
                    strcpy(message,"{\"type\":\"Error\",\"message\":\"Authentication Failed!\"}");
                }
            }
            else if(strcmp(command,"change")==0)//change password
            {
                command=strtok(NULL," ");
                if(strcmp(command,"password")==0)
                {
                    command=strtok(NULL," ,");
                    if(strcmp(command,Token)==0)
                    {
                        int returnedChangePassword=changePassword(buffer);
                        if(returnedChangePassword==1)
                        {
                            strcpy(message,"{\"type\":\"Successful\",\"message\":\"your password changed Successfully\"}");
                        } else if(returnedChangePassword==2)
                        {
                            strcpy(message,"{\"type\":\"Error\",\"message\":\"upper than 16 character\"}");
                        }
                        else if(returnedChangePassword==3)
                        {
                            strcpy(message,"{\"type\":\"Error\",\"message\":\"newpassword and the old password are the same!\"}");
                        }
                        else{
                            strcpy(message,"{\"type\":\"Error\",\"message\":\"Something Bad happend on saving password\"}");
                        }
                    } else
                    {
                        strcpy(message,"{\"type\":\"Error\",\"message\":\"Authentication Failed!\"}");
                    }
                }
            }
            else if(strcmp(command,"delete")==0)
            {
                int returnedDelete=Delete(buffer);
                if(returnedDelete==1)
                {
                    strcpy(message,"{\"type\":\"Successful\",\"message\":\"Deleted successfully\"}");
                }
                else if(returnedDelete==2)
                {
                    strcpy(message,"{\"type\":\"Error\",\"message\":\"You dont have this Tweet\"}");
                }
                else if(returnedDelete==3)
                {
                    strcpy(message,"{\"type\":\"Error\",\"message\":\"Failed to Deleted/Persmission Denied\"}");
                }
                    else{
                    strcpy(message,"{\"type\":\"Error\",\"message\":\"Recieving data failed\"}");
                }
            }
            else if(strcmp(command,"retweet")==0)
            {
                command=strtok(NULL," ,");
                if(strcmp(command,Token)==0) {
                    if (ReTweet(buffer)) {
                        strcpy(message, "{\"type\":\"Successful\",\"message\":\"Tweet ReTweeted succesfully!\"}");
                    } else
                        strcpy(message, "{\"type\":\"Error\",\"message\":\"Tweet Not Found\"}");
                } else
                    strcpy(message,"{\"type\":\"Error\",\"message\":\"Authentication Failed!\"}");
            }
            else if(strcmp(command,"bigsearch")==0)
            {
                char *resultSearch=(char*)malloc(MaxSize);
               // char mixedresult[MaxSize];
                strcpy(resultSearch,bigsearch(buffer));
                if(resultSearch[0]!='\0')
                {
/*                    strcpy(mixedresult,"{\"type\":\"Profile\",\"message\":");
                    strcat(mixedresult,resultSearch);
                    strcat(mixedresult,"}");*/
                    strcpy(message,resultSearch);
                } else
                {
                    strcpy(message,"{\"type\":\"Error\",\"message\":\"Not match\"}");
                }
            }
            else
            {
                    strcpy(message,"{\"type\":\"Error\",\"message\":\"Bad request format.\"}");
            }


            // Echo the buffer back to the sender
            // this is the way that we chose what to send
            int iSendResult = send( new_socket, message, strlen(message), 0 );
            if (iSendResult == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(new_socket);
                WSACleanup();
                return 1;
            }
            printf("Bytes sent: %d\n", iSendResult);
            printf(message);
        }
        else if (iResult == 0)
        {
            printf("Connection closing...\n");
            break;
        }

        else  {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(new_socket);
            WSACleanup();
            return 1;
        }

    } while (1);


    int iResult = shutdown(new_socket,SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(new_socket);
        WSACleanup();
        return 1;
    }

    closesocket(s);
    WSACleanup();
    free(command);free(message);
    return 0;
}
