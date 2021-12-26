#include <stdio.h>
#include "slre.h"
#include <stdlib.h>
#include <string.h>
#include "File.h"
#define Max_Size 10000
char *BigSearch(char* path,char*Tomath) {
   char *message=(char*)malloc(MaxSize);
   message=LisOfFiles(path);//pouria;pouriaArefi;poya
     if(message[0]=='\0')
         return 0;
    char *out=(char*)malloc(MaxSize);
    char *temp=(char*)malloc(MaxSize);strcpy(temp,message);
    char *regex=(char*)malloc(Max_Size);
    strcpy(regex,"([a-z A-Z 0-9]*");
    strcat(regex,Tomath);
    strcat(regex,"[a-z A-Z 0-9]*)"); //(po[a-z A-Z]*)
  //  char message[] = "{\"type\":\"Token\",\"message\":\"kCsTqLP8ZIgcNYkocxwCMX0JfJ7k8JfW\"}";
    const int num_caps = 10;
    char caps [num_caps][MaxSize];
    int flag= slre_match(regex, message, Max_Size, caps, num_caps, 0);
    if(flag==0)
        return 0;
    //sprintf(Token,"%s\n", tokenptr);
    int i=0;
    temp=strtok(message,";");
   while(caps[i][0]!='\0'&&i<10)
    {

       if(i==0)
           strcpy(out,caps[i]);
       else
       {
           strcat(out,"__");
           strcat(out,caps[i]);
       }

      printf("\nThe Token cached %s\n", caps[i]);
       i++;
    }
   free(temp);
    return out;
}
int SendTweet(char* message,char*result) {

    char regex[] = "{\"type\":\"Token\",\"message\":\"([A-Z a-z 0-9 _]*)\"}";
    //  char message[] = "{\"type\":\"Token\",\"message\":\"kCsTqLP8ZIgcNYkocxwCMX0JfJ7k8JfW\"}";
    const int num_caps = 1;
    char caps[num_caps][Max_Size];
    slre_match(regex, message, Max_Size, caps, num_caps, 0);
    int i=0;
    // sprintf(Token,"%s\n", caps[i]);
    for (int i = 0; i < num_caps; i++)
    {
        printf("The resualt is: %s\n", caps[i]);
    }

    return 1;
}
int PrintComment(char *message)
{
    if(message[0]='\0')
        return 0;
    char regex[] = "\"([A-Z a-z 0-9 . _ - !@#$%^&*()~]*])\"";
    //  char message[] = "{\"type\":\"Token\",\"message\":\"kCsTqLP8ZIgcNYkocxwCMX0JfJ7k8JfW\"}";
    const int num_caps = 2;
    char caps[num_caps][Max_Size];
    int flag= slre_match(regex, message, Max_Size, caps, num_caps, 0);
    if(flag==0)
        return 0;
    //sprintf(Token,"%s\n", tokenptr);
   for (int i = 0; i < num_caps; i++)
  {
       printf(" %s\t", caps[i]);
  }
   printf("\n");
    return  1;
}
