// covermax.h
#ifndef COVERMAX
#define COVERMAX
//draw function from rand if value is smaller than these
#define SKIP 0.8
#define INDELRATE 0.98
//#define KMERDEL 1.0

typedef unsigned short uint8_t;
//////////////////////////
// function initialise //
////////////////////////

int get_fasta_chunk (FILE *fp, char **header_var, char **data, size_t header_size, size_t data_size);
int amplify (char **source, size_t sizes, size_t coverage);
void mutate(char *source, size_t coverage, size_t length);
char generateindel();
int generatekmerdel(char **input, int mutation_pos, size_t length);
////////////////////////
/// define functions //
//////////////////////

int get_fasta_chunk(FILE *fp, char **header_var, char **data, size_t header_size, size_t data_size)
{
    uint8_t lastpipeposition; //use this to find rewind our file pointer
    size_t byteswritten; //keep count of how many bases we've read
    char *buffer=malloc(sizeof(char)*256);
    fgets(buffer, 255, fp);
    lastpipeposition=strlen(buffer);
   // printf("%d\n", strchr(buffer, '>'));
    if (strchr(buffer, '>') != NULL ) //does our buffer contain the >
    {
        strtok(buffer, "\n");
        strncpy(*header_var, buffer, header_size);
    }
    fgets(buffer, 255, fp);
   // printf("%d\n", strchr(buffer, '>'));
    while (strchr(buffer, '>')== NULL)
    {
       // printf("entered loop\n");
        lastpipeposition+=strlen(buffer);
        byteswritten+=strlen(buffer);
        strtok(buffer, "\n");
        if (byteswritten < data_size)
        {
           strncat(*data, buffer, data_size);
        }
        else
        {
            return -1;
        }
        fgets(buffer, 255, fp);

    }
    fseek(fp, lastpipeposition, SEEK_SET); //rewind our file pointer
    return 0;
}

int amplify (char **source, size_t sizes, size_t coverage) //returns number of mutation calls
{
    switch (sizes/1000)
    {
        case 0:
        {
            printf("Sequence is too short!\n");
            return 0; //its likely this read will be discarded anyway
        }
        case 1:
        {
            mutate(*source, coverage, sizes);
            return 1;
        }
        case 2:
        {
            mutate(*source, coverage, sizes);
            return 1;
        }
        case 3:
        {
            char *var1=malloc(sizeof(char)*2000);
            char *var2=malloc(sizeof(char)*2000);
            size_t midpoint = sizes/2;
            var2=*source+midpoint+1;
            strncpy(var1, *source, midpoint);
            mutate(var1, coverage, strlen(var1));
            mutate(var2, coverage, strlen(var2));
            var1=*source+(sizeof(char)*(midpoint/2));
            mutate(var1, coverage, strlen(var1));
            free(var1);
            free(var2);
            return 3;
        }
         case 4:
        {
            char *var1=malloc(sizeof(char)*2500);
            char *var2=malloc(sizeof(char)*2500);
            size_t midpoint = sizes/2;
            var2=*source+midpoint+1;
            strncpy(var1, *source, midpoint);
            mutate(var1, coverage, strlen(var1));
            mutate(var2, coverage, strlen(var2));
            var1=*source+(sizeof(char)*(midpoint/2));
            mutate(var1, coverage, strlen(var1));
            free(var1);
            free(var2);
            return 3;
        }
        default: //lets divide and conquer!
        {
            size_t midpoint = sizes/2;
            if ((sizes/1000) < 10)
            {
                char *variable1=malloc(sizeof(char)*5000);
                char *variable2=malloc(sizeof(char)*5000);
                variable2=*source+midpoint+1;
                strncpy(variable1, *source, midpoint);
                amplify(&variable1, strlen(variable1), coverage);
                amplify(&variable2, strlen(variable2), coverage);
                free(variable2);
                variable1=*source+(sizeof(char)*midpoint/2);
                amplify(&variable1, strlen(variable1), coverage);
                free(variable1);
                return 4;
            }
            if ((sizes/1000) < 20)
            {
                char *variable1=malloc(sizeof(char)*10000);
                char *variable2=malloc(sizeof(char)*10000);
                variable2=*source+midpoint+1;
                strncpy(variable1, *source, midpoint);
                amplify(&variable1, strlen(variable1), coverage/2);
                amplify(&variable2, strlen(variable2), coverage/2);
                free(variable2);
                variable1=*source+(sizeof(char)*midpoint/2);
                amplify(&variable1, strlen(variable1), coverage/2);
                free(variable1);
                return 4;
            }
             if ((sizes/1000) < 30)
            {
                char *variable1=malloc(sizeof(char)*15000);
                char *variable2=malloc(sizeof(char)*15000);
                variable2=*source+midpoint+1;
                strncpy(variable1, *source, midpoint);
                amplify(&variable1, strlen(variable1), coverage);
                amplify(&variable2, strlen(variable2), coverage);
                free(variable2);
                variable1=*source+(sizeof(char)*midpoint/2);
                amplify(&variable1, strlen(variable1), coverage);
                free(variable1);
                return 4;
            }
             if ((sizes/1000) < 50) //should be large enough
            {
                char *variable1=malloc(sizeof(char)*25000);
                char *variable2=malloc(sizeof(char)*25000);
                variable2=*source+midpoint+1;
                strncpy(variable1, *source, midpoint);
                amplify(&variable1, strlen(variable1), coverage);
                amplify(&variable2, strlen(variable2), coverage);
                free(variable2);
                variable1=*source+(sizeof(char)*midpoint/2);
                amplify(&variable1, strlen(variable1), coverage);
                free(variable1);
                return 4;
            }
        }
    }
}

void mutate(char *source, size_t coverage, size_t length)
{
    FILE *fout=fopen("covermax.fasta", "a");
    char *mutatedstring=malloc(sizeof(char)*length);
    float drawmutationmode;
    size_t mutatedstringlen;
    for (int i=0; i<coverage; i++)
    {
        strcpy(mutatedstring, source);
        mutatedstringlen=strlen(mutatedstring);
        fprintf(fout, ">%d-%d-%d-mutate.fasta\n", rand(), rand(), i);
        for (int pos=0; pos<mutatedstringlen; pos++)
        {
            drawmutationmode=rand()/(double)RAND_MAX;
            //get random float between 0.0 and 1
            //printf("random = %f\n", drawmutationmode);
            if (drawmutationmode > SKIP && drawmutationmode <= INDELRATE) // float is between 0.700..1 and 0.85
            {
                mutatedstring[pos]=generateindel();
               // printf("point mutation!\n");
            }
            if (drawmutationmode > INDELRATE)
            {
                int kmerdeletedlen=generatekmerdel(&mutatedstring, pos, strlen(mutatedstring));
                mutatedstringlen-=kmerdeletedlen;
              //  printf("kmer deletion\n");
            }
        }
        fprintf(fout, "%s\n", mutatedstring);
    }

}

char generateindel()
{
    int base_option=rand()%4;
    switch (base_option)
        {
            case 0:
            {
                return 'A';
            }
             case 1:
            {
                return 'C';
            }
             case 2:
            {
                return'G';
            }
             case 3:
            {
                return 'T';
            }
        }
}

int generatekmerdel(char **input, int mutation_pos, size_t length) //return kmer length deleted
{
    int kmersize=(rand()%3)+4; //kmer length between 4-6
    char *beforedel=malloc(sizeof(char)*length);
    char *afterdel=malloc(sizeof(char)*length);
    beforedel=*input;
    afterdel=*input+mutation_pos;
    *afterdel='\0';
    afterdel+=kmersize;
    strcpy(*input, beforedel);
    strcat(*input, afterdel);
    return kmersize;
}
#endif
