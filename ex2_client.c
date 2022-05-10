// אופיר שטרוסברג
// לידור דנון
#include <sys/random.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

void handler5(){
    printf("end of stage: server returned the solution\n");
    char fileName[100] = {};
    sprintf(fileName, "toClient%d.txt", getpid());
    int fdToClient = open(fileName, O_RDONLY);
    int readBytes;
    char ch;
    if(fdToClient == -1){
        printf("ERROR_FROM_EX2\n");
        remove(fileName);
        exit(-1);
    }
    printf("sol = ");
    do{
       readBytes = read(fdToClient,&ch,1);
       if(readBytes == 1){
       printf("%c", ch);
       }
    }while(readBytes == 1);
    printf("\n");
    printf("end of stage: client finished printing the solution\n");
    close(fdToClient);
    remove(fileName);
    exit(0);
}
int main(int argc, char * argv[]){
    signal(5, handler5);
    if(argc!=5){
        printf("ERROR_FROM_EX2\n");
        exit(-1);
    }
    int i;
    unsigned int randNum;
    int toServerfd;
    for(i=0;i<10;i++){
        toServerfd = open("to_srv.txt", O_CREAT | O_WRONLY | O_TRUNC, 0666);
        if(toServerfd == -1){
            getrandom(&randNum,sizeof(unsigned int), GRND_NONBLOCK);
            randNum = randNum%5 + 1;
            sleep(randNum);
        }
        else{
            char buffer[700] = {};
            sprintf(buffer,"%d\n%s\n%s\n%s\n" , getpid(),argv[2],argv[3],argv[4]);
            write(toServerfd, &buffer, strlen(buffer));
            close(toServerfd);
            if(kill(atoi(argv[1]),5)==-1){
                printf("ERROR_FROM_EX2\n");
                remove("to_srv.txt");
                exit(-1);
            }
            sleep(10);
            exit(0);
        }
    }
    printf("ERROR_FROM_EX2\n");
    remove("to_srv.txt");
    exit(-1);
}
