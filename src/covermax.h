// covermax.h
#ifndef COVERMAX
#define COVERMAX
#define normal_size 4000
#define large_size 15000
#define xl_size 60000 //this should be the max possible minion read length

// function defs
char *get_first_true_read (file *fp); //take a fasta file and generate more reads from it
char *get_read(file *fp);



char *get_first_true_read (file *fp)
{
    uint8_t bytes_read=0;
    char *buffer=malloc(sizeof(char)*256); //create a buffer to hold a line of 256 chars
    fgets(buffer, 255, fp); //discard the header line
    char *read_l_normal, read_l_large, read_l_xlarge; //three char pointers to possible containers for our reads
    read_l_normal=malloc(sizeof(char)*normal_size); //hold up to 3999 bases
    bytes_read=fgets(buffer, 255, fp); //count the bytes read
    if (buffer[0]=='>' ||buffer[0]==';') //account for comments
    {
        bytes_read=0;
        bytes_read=fgets(buffer, 255, fp); //read in the next line
    }
    while (strstr(buffer, '>') == NULL)
    {
        strtok(buffer, '\n');
        strcat(read_l_normal, read_l_normal);
        bytes_read+=fgets(buffer, 255, fp);
        if (bytes_read >= normal_size-255) //if the number of bytes read out of the file exceeds our buffer size
        {
            strtok(buffer, '\n');
            read_l_large=malloc(sizeof(char)*large_size); //dynamic memory assignment
            strcat(read_l_large, read_l_normal);
            free(read_l_normal);
            strcat(read_l_large, buffer);
            bytes_read+=fgets(buffer, 255, fp);
            while (strstr(buffer, '>') == NULL)     //continue for the larger read buffer
            {
                strtok(buffer, '\n');
                strcat(read_l_large, buffer);
                bytes_read+=fgets(buffer, 255, fp);
                if (bytes_read >= large_size-255)
                {
                    strtok(buffer, '\n');
                    read_l_xlarge=malloc(sizeof(char)*xl_size);
                    strcat(read_l_xlarge, read_l_large);
                    free(read_l_large);
                    strcat(read_l_xlarge, buffer);
                    bytes_read+=fgets(buffer, 255, fp);
                    while (strstr(buffer, '>') == NULL) //continue for the even larger buffer
                    {
                        strtok(buffer, '\n');
                        strcat(read_l_xlarge, buffer);
                        bytes_read+=fgets(buffer, 255, fp);
                    }
                    if (strstr(buffer, '>' != NULL)) {return read_l_xlarge;}
                }
                if (strstr(buffer, '>' != NULL)) {return read_l_large;}
            }
             if (strstr(buffer, '>' != NULL)) {return read_l_large;}
        }
        if (strstr(buffer, '>' != NULL)) {return read_l_normal;}
    }
    //file *outfile=fopen();
}



#endif
