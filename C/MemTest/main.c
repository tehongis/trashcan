#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
    int x;
    int y;
    struct node *left;
    struct node *right;
};

int *readdata(char *filename) {

    // try to open file
    FILE *fptr;
    fptr = fopen(filename,"r");

    if(fptr == NULL) {
        printf("File open error!");
        exit(1);
    }


    // check filesize, reserve memory and zerofill-it
    fseek(fptr, 0, SEEK_END);
    long int fsize = ftell(fptr);
    int *memptr = (int*)malloc(fsize+1);
 
    if (memptr == NULL) {
        printf("Memory not allocated.\n");
        exit(0);
    }
    memset(memptr, 0, fsize+1);   

    // read filedata to allocated memory
    fseek(fptr, 0, 0);
    fread(memptr,1,fsize,fptr);

    // close file and return allocated memory
    fclose(fptr);
    return memptr;
}


int main(int argc,char **argv) {
/*
    for (int c = 0 ; c < argc; c++) {
        printf("%d : %s\n",c,argv[c]);
    }
*/

    char *filedata = (char *) readdata("random2.txt");

    int datasize = strlen(filedata);
    printf("Mempointer: %p\n",filedata);
    printf("Filesize: %d\n",datasize);

    struct Node *base_node = NULL;
    struct Node *current_node = base_node;

    char *token;
    while ((token = strsep(&filedata, "\n"))) {
        char **restrict instate = NULL;
        int numberA = atoi( strtok_r(token, "\t", &instate) );
        int numberB = atoi( strtok_r(NULL, "\t", &instate) );

        struct Node *new_node = malloc(sizeof(struct Node));
        if (base_node = NULL) {
            base_node = new_node;
        }
        
        new_node->x = numberA;
        new_node->y = numberB;

        current_node = new_node;

        //printf("%d %d\n",numberA,numberB);
    }

    free(filedata);

    free(base_node);

    return 0;
}
