//
// Created by Pouria on ۰۱/۰۸/۲۰۲۱.
//

#ifndef PROJECT_TDELAY_H
#define PROJECT_TDELAY_H
// C function showing how to do time delay
#include <stdio.h>
// To use time library of C
#include <time.h>

void delay(int number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;

    // Storing start time
    clock_t start_time = clock();

    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds)
        ;
}

// Driver code to test above function
int Tdelay(int n)
{
    int i;
    for (i = 0; i < n; i++) {
        // delay of one second
        delay(1);
        //printf("%d seconds have passed\n", i + 1);
    }
    return 0;
}
#endif //PROJECT_TDELAY_H
