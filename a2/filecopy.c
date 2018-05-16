#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main() {
char input_filename[50];
char output_filename[50];
    printf("Please enter input file name: ");
    gets(input_filename);

    int infd;
    infd = open(input_filename,O_RDWR);
    if(infd < 0){
        perror("Error ");
        return -1;
    }
    printf("Please enter output file name: ");
    gets(output_filename);
    int outfd;
    outfd = open(output_filename,O_CREAT|O_EXCL|O_WRONLY,0777);
    if(outfd <0){
        if(errno == EEXIST) {
            printf("File already exists, overwrite? enter 1 for yes,0 for no\n");
            int choice;
            scanf("%d", &choice);
            if (choice == 1) {
                printf("Overwriting file...\n");
                outfd = open(output_filename,O_RDWR);
            }
            else{
                printf("Exiting program\n");
                return -1;
            }
        } else{
            perror("Error ");
            return -1;
        }


    }

    char buffer[1];
    int numRead;
    int numWrite;
    while((numRead = read(infd,&buffer,1)) > 0){
        numWrite = write(outfd,&buffer,numRead);
        if(numRead != numWrite){
            perror("Error ");
            return -1;
        }
    }
    if(numRead <0) {
        perror("Error ");
        return -1;
    }

    int close_in = close(infd);
    if(close_in <0){
        perror("Error ");
        return -1;
    }
    int close_out = close(outfd);
    if(close_out <0){
        perror("Error ");
        return -1;
    }
    printf("File copied succesfully!\n");
    return 0;
}
