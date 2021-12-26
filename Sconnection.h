
#include<io.h>
#include<stdio.h>
#include<winsock2.h>
#include "Tdelay.h"
#pragma comment(lib,"ws2_32.lib") //Winsock Library
#define MaxSize2 1000
int APIconnect()
{
    WSADATA wsa;
    SOCKET s , new_socket;
    struct sockaddr_in server , client;
    int c;
    char *message,*command,*type,*request,*buffer;
    message=(char*)malloc(MaxSize2);
    command=(char*)malloc(MaxSize2);
    type=(char*)malloc(MaxSize2);
    request=(char*)malloc(MaxSize2);
    buffer=(char*)malloc(MaxSize2);
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
            printf("Bytes received: %d\n", iResult);
            command= strtok(buffer,' ');
            if(strcmp(command,"login")==0)
            {

            }
            else if(strcmp(command,"signup")==0)
            {
                if(signup(buffer))
                {
                    message
                }
            }
            else if(strcmp(command,"send")==0)//send tweet
            {
                command=strtok(NULL,' ');
                if(strcmp(command,"tweet")==0)
                {

                }
            }
            else if(strcmp(command,"refresh")==0)
            {

            }
            else if(strcmp(command,"like")==0)
            {

            }
            else if(strcmp(command,"comment")==0)
            {

            }
            else if(strcmp(command,"search")==0)
            {

            }
            else if(strcmp(command,"follow")==0)
            {

            }
            else if(strcmp(command,"unfollow")==0)
            {

            }
            else if(strcmp(command,"set")==0)//set bio command
            {
              command=strtok(NULL,' ');
              if(strcmp(command,"bio")==0)
              {

              }
            }
            else if(strcmp(command,"logout")==0)
            {

            }
            else if(strcmp(command,"profile")==0)
            {

            }
            else if(strcmp(command,"change")==0)//change password
            {
                command=strtok(NULL,' ');
                if(strcmp(command,"password")==0)
                {

                }
            }

            // Echo the buffer back to the sender
            // this is the way that we chose what to send
           int iSendResult = send( new_socket, message, MaxSize2, 0 );
            if (iSendResult == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(new_socket);
                WSACleanup();
                return 1;
            }
            printf("Bytes sent: %d\n", iSendResult);
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

    return 0;
}