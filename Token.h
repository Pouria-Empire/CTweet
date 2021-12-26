//
// Created by Pouria on ۰۲/۰۵/۲۰۲۱.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#ifndef SERVER_TOKEN_H
#define SERVER_TOKEN_H
void buildToken(char* output) {
        srand(time(0));
        int Length=32;
        static char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ123456789#?!";
        char *randomString = NULL;


            randomString = malloc(sizeof(char) * (Length +1));

            if (randomString) {
                for (int n = 0;n < Length;n++) {
                    int key = rand() % (int)(sizeof(charset) -1);
                    randomString[n] = charset[key];
                }

                randomString[Length] = '\0';
            }


        strcpy(output,randomString);

}
#endif //SERVER_TOKEN_H
