// covermax.h
#ifndef COVERMAX
#define COVERMAX

typedef unsigned short uint8_t;
//////////////////////////
// function initialise //
////////////////////////

int get_fasta_chunk (FILE *fp, char **header_var, char **data, size_t header_size, size_t data_size);
int amplify (char **source, /*char **var1, char **var2, char **var3, char **var4, char **var5,*/ size_t sizes, size_t coverage);
void mutate(char *source, size_t coverage, size_t length);
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

int amplify (char **source, /*char **var1, char **var2, char **var3, char **var4, char **var5,*/ size_t sizes, size_t coverage) //returns number of mutation calls
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
                amplify(&variable1, strlen(variable1), coverage/2);
                amplify(&variable2, strlen(variable2), coverage/2);
                free(variable2);
                variable1=*source+(sizeof(char)*midpoint/2);
                amplify(&variable1, strlen(variable1), coverage/2);
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
                amplify(&variable1, strlen(variable1), coverage/2);
                amplify(&variable2, strlen(variable2), coverage/2);
                free(variable2);
                variable1=*source+(sizeof(char)*midpoint/2);
                amplify(&variable1, strlen(variable1), coverage/2);
                free(variable1);
                return 4;
            }
             if ((sizes/1000) < 50) //should be large enough
            {
                char *variable1=malloc(sizeof(char)*25000);
                char *variable2=malloc(sizeof(char)*25000);
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
        }
    }
}

void mutate(char *source, size_t coverage, size_t length)
{
    FILE *fout=fopen("covermax.fasta", "a");
    char savedchar;
    for (int i=0; i<coverage; i++)
    {
        fprintf(fout, ">%d-%d-%d-mutate.fasta\n", rand(), rand(), i);
        int position=rand()%length;
        int base_option=rand()%4;
        savedchar=source[position];
        switch (base_option)
        {
            case 0:
            {
                source[position]='A';
            }
             case 1:
            {
                source[position]='C';
            }
             case 2:
            {
                source[position]='G';
            }
             case 3:
            {
                source[position]='T';
            }
        }
        fprintf(fout, "%s\n", source);
        source[position]=savedchar;
    }

}

#endif
