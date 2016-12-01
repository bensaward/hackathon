#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include "covermax.h"


int main (int argc, char **argv)
{
    srand(time(NULL)); //seed prng
    FILE *filepoint=fopen("test.fasta", "r+");
    if (filepoint == NULL)
    {
        printf("Could not open file, exiting now.\n");
    }
    char *read=malloc(sizeof(char)*3000), *header=malloc(sizeof(char)*100);
    char *read2=malloc(sizeof(char)*3000), *header2=malloc(sizeof(char)*100);
    get_fasta_chunk(filepoint, &header, &read, 100, 3000);
    get_fasta_chunk(filepoint, &header2, &read2, 100, 3000);
   // printf("obtained header: %s\nobtained data: %s\n", header, read);
   // printf("obtained header: %s\nobtained data: %s\n", header2, read2);
    printf("mutating: %s\n", header);
    amplify(&read, strlen(read), 30);
    return 0;
}
