//
// Created by fiy-pc on 2024/10/4.
//

#include "affair.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#include "ORPSET.h"

void affair(int sockfdClient)
{
    int bytes;
    char buffer[BUFSIZ];

    write(sockfdClient, "begin",BUFSIZ);
    bytes = read(sockfdClient,buffer,BUFSIZ);
    if(bytes==-1)
    {
        perror("Error receiving from server");
        exit(EXIT_FAILURE);
    }
    printf("%s\n",buffer);

    char affair[MAX_AFFAIR_SIZE][BUFSIZ];

    for (int i=0; i<MAX_AFFAIR_SIZE; i++)
    {
        strcpy(buffer,"");
        fgets(buffer, BUFSIZ, stdin);
        if (strncmp(buffer, "commit", 6) == 0)
        {
            break;
        }
        if (strncmp(buffer, "reset", 5) == 0)
        {
            return;
        }
        buffer[strcspn(buffer, "\n")] = 0;

        strcpy(affair[i], buffer);
    }
    for (int i=0; i<MAX_AFFAIR_SIZE; i++)
    {
        strcpy(buffer,"");
        if(strcmp(affair[i], "") == 0)
        {
            break;
        }
        write(sockfdClient, affair[i], strlen(affair[i])+1);
        bytes = read(sockfdClient, buffer, BUFSIZ-1);
        if (bytes == -1)
        {
            perror("Error receiving from server");
            exit(EXIT_FAILURE);
        }
        if(strncmp(buffer,"affair failed",13)==0)
        {
            perror(buffer);
            return;
        }
        printf("%s\n", buffer);
    }
    printf("affair commit success!\n");
}