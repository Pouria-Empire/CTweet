//
// Created by Pouria on ۰۲/۰۴/۲۰۲۱.
//
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <winbase.h>
#include <string.h>
#define MaxSize 10000
#ifndef SERVER_FILE_H
#define SERVER_FILE_H
#define SPACE "\t\r\n"
#define SPACE_S "\t\r\n\\"
static void strip(char* s)
{
    char* p = s;
    int n;
    while (*s)
    {
        n = strcspn(s, SPACE);
        strncpy(p, s, n);
        p += n;
        s += n + strspn(s + n, SPACE);
    }
    *p = 0;
}
static void strip_s(char* s)
{
    char* p = s;
    int n;
    while (*s)
    {
        n = strcspn(s,SPACE_S);
        strncpy(p, s, n);
        p += n;
        s += n + strspn(s + n, SPACE_S);
    }
    *p = 0;
}

void StoreData(const char *filepath, const char *data)
{
    FILE *fp = fopen(filepath, "w");
    if (fp != NULL)
    {
        fputs(data, fp);
        fclose(fp);
    }
}
int ReadData(const char*filepath,char*result)
{
   // result=(char*)malloc(MaxSize);memset(result,'\0',MaxSize);
    FILE *fp;
    char str[MaxSize];
    fp = fopen(filepath, "r");
    if(fp==NULL)
        return 0;
    while (fgets(str, MaxSize-2, fp) != NULL)
        strcat(result,str);
    fclose(fp);
    return 1;
}
int FileExists(const char * filename){
    /* try to open file to read */
    FILE *file;
    if (file = fopen(filename, "r")){
        fclose(file);
        return 1;
    }
    return 0;
}
char *readFile(const char *filename)
{
    char * buffer = 0;
    long length;
    FILE * f = fopen (filename, "rb");

    if (f)
    {
        fseek (f, 0, SEEK_END);
        length = ftell (f);
        fseek (f, 0, SEEK_SET);
        buffer = malloc (length+1);
        if (buffer)
        {
            fread (buffer, 1, length, f);
            buffer[length]='\0';
        }
        fclose (f);
    }

    if (buffer)
    {
        return buffer;
    } else
        return '\0';
}
int folderExist(const char *foldername)
{
    DIR* dir = opendir(foldername);
    if (dir) {
        closedir(dir);
         return 1;
    }
    else {
   return 0;
    }
}
int findTheLastFile()
{
    int id=1;
    char ids[20]={'\0'};
    char *makedFilename=(char*)malloc(MaxSize);
    strcpy(makedFilename,"Tweets\\\\");
    sprintf(ids,"%d",id);
    strcat(makedFilename,ids);
    strcat(makedFilename,".tweet.json");
    while(FileExists(makedFilename))
    {
        id++;
        strcpy(makedFilename,"Tweets\\\\");
        sprintf(ids,"%d",id);
        strcat(makedFilename,ids);
        strcat(makedFilename,".tweet.json");
    }
    free(makedFilename);
    return id;
}
char *makedFileTweet(char *id)
{
    char *makedFilename=(char*)malloc(MaxSize);
    strcpy(makedFilename,"Tweets\\\\");
    strcat(makedFilename,id);
    strcat(makedFilename,".tweet.json");
    return makedFilename;

}
char *makedFileUser(char *user)
{
    char *makedFilename=(char*)malloc(MaxSize);
    strcpy(makedFilename,"users\\\\");
    strcat(makedFilename,user);
    strcat(makedFilename,".user.json");
    return makedFilename;
}
char *RMType(char* myStr) {
    char *retStr;
    char *lastExt;
    if (myStr == NULL) return NULL;
    if ((retStr = malloc (strlen (myStr) + 1)) == NULL) return NULL;
    strcpy (retStr, myStr);
    lastExt = strrchr (retStr, '.');
    if (lastExt != NULL)
        *lastExt = '\0';
    return retStr;
}
char *LisOfFiles(char *path)
{
    char *out=(char*)malloc(MaxSize);
    strcpy(out,"\0");
    char *name;
    DIR *d;
    struct dirent *dir;
    int i=0;
  //  d = opendir(".");
    d = opendir(path);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if(strcmp(dir->d_name,".")!=0&&strcmp(dir->d_name,"..")!=0)
            {
                name=RMType(dir->d_name);
                if(i!=0)
                {
                    strcat(out,";");
                    strcat(out,name);
                }
                else
                strcpy(out,name);
            }
            printf("file names:%s\n", dir->d_name);
            i++;
        }
        closedir(d);
    }
    return out;
}

#endif //SERVER_FILE_H
