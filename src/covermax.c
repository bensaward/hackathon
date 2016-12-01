//usage covermax -n to-mutate -c coverage filename.fasta
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
    int coverage=0, tomutate=0;
    if (argc > 5)
    {
        for (int i=1; i<4; i++)
        {
            if (strstr(argv[i], "-n") != NULL)
            {
                if (strchr(argv[i+1], '*') != NULL)
                {
                    tomutate=-1;
                    //printf("set mutate = %d\n", tomutate);
                }
                else
                {
                    tomutate=atoi(argv[i+1]);
                    //printf("set mutate = %d\n", tomutate);
                }
            }
            if (strstr(argv[i], "-c") != NULL)
            {
                coverage=atoi(argv[i+1]);
               // printf("coverage = %d\n", coverage);
            }
        }
        //FILE *filepoint=fopen("test.fasta", "r+");
        FILE *filepoint=fopen(argv[5], "r");
        if (filepoint == NULL)
        {
            printf("Could not open file, exiting now.\n");
        }
        char *read=malloc(sizeof(char)*3000), *header=malloc(sizeof(char)*100);
        if (coverage <= -1)
        {
            while (!feof(filepoint))
            {
              //  printf("cont mutating: %s\n", header);
                get_fasta_chunk(filepoint, &header, &read, 100, 3000);
                amplify(&read, strlen(read), coverage);
            }
        }
        else
        {
            //printf("to mutate= %d coverage = %d\n", tomutate, coverage);
            for (int i=0; i<tomutate; i++)
            {
                get_fasta_chunk(filepoint, &header, &read, 100, 3000);
                printf("mutating: %s\n", header);
                amplify(&read, strlen(read), coverage);
            }
        }
       // get_fasta_chunk(filepoint, &header, &read, 100, 3000);
        // printf("obtained header: %s\nobtained data: %s\n", header, read);
        // printf("obtained header: %s\nobtained data: %s\n", header2, read2);
       // printf("mutating: %s\n", header);
       // amplify(&read, strlen(read), 30);
    }
    else
    {
        printf("COVERMAX\nUsage: covermax -c <coverage> -n <number-of-fastas-to-mutate> <input-file>\n");
    }
    return 0;
}
