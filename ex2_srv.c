// אופיר שטרוסברג
// לידור דנון
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
int timer = 60;
void handler5(){
        signal(5, handler5);
        printf("\nend of stage: client conected to server\n");
        int result;
        pid_t pid;
        int status;
        int toclientfd;
        char *line1 = NULL;
        char *line2 = NULL;
        char *line3 = NULL;
        char *line4 = NULL;
        size_t  len1 = 0;
        size_t  len2 = 0;
        size_t  len3 = 0;
        size_t  len4 = 0;
        int flagError = 0;
        char stringToWrite[700] = {};
        if((pid = fork()) == 0){
                FILE *clientStream = fopen("to_srv.txt", "r");
                if (clientStream == NULL)
                {
                        printf("ERROR_FROM_EX2\n");
                        flagError = 1;
                }
                if (getline(&line1, &len1, clientStream) <= 0)
                {
                        printf("ERROR_FROM_EX2\n");
                        free(line1);
                        fclose(clientStream);
                        flagError = 1;
                }
                if (getline(&line2, &len2, clientStream) <= 0)
                {
                        printf("ERROR_FROM_EX2\n");
                        free(line1);
                        free(line2);
                        fclose(clientStream);
                        flagError = 1;
                }
                if (getline(&line3, &len3, clientStream) <= 0)
                {
                        printf("ERROR_FROM_EX2\n");
                        free(line1);
                        free(line2);
                        free(line3);
                        fclose(clientStream);
                        flagError = 1;
                }
                if (getline(&line4, &len4, clientStream) <= 0)
                {
                        printf("ERROR_FROM_EX2\n");
                        free(line1);
                        free(line2);
                        free(line3);
                        free(line4);
                        fclose(clientStream);
                        flagError = 1;
                }
                fclose(clientStream);
                line1[strcspn(line1, "\n")] = 0;
                line2[strcspn(line2, "\n")] = 0;
                line3[strcspn(line3, "\n")] = 0;
                line4[strcspn(line4, "\n")] = 0;
                //need atoi for all what we read
                char fileName2[100] = {};
                sprintf(fileName2, "toClient%s.txt", line1);
                toclientfd = open(fileName2, O_CREAT | O_WRONLY | O_TRUNC, 0666);
                int firstNum = atoi(line2);
                int op = atoi(line3);
                int secondNum = atoi(line4);
                if (secondNum == 0 && op == 4){
                        sprintf(stringToWrite, "cannot devide by zero");
                        write(toclientfd, &stringToWrite, strlen(stringToWrite));
                        close(toclientfd);
                        flagError = 1;
                        if(kill(atoi(line1), 5)==-1){
                                printf("ERROR_FROM_EX2\n");
                                flagError = 1;
                        }
                        else{
                            printf("end of stage: server finished computing the solution\n");    
                        }
                        
                }
                else{
                        if(op == 1){
                                result = firstNum + secondNum;
                        }
                        else if(op == 2){
                                result = firstNum - secondNum;
                        }
                        else if(op == 3){
                                result = firstNum * secondNum;
                        }
                        else if(op == 4){
                                result = firstNum / secondNum;
                        }
                        else{
                                sprintf(stringToWrite, "invalid operation");
                                write(toclientfd, &stringToWrite, strlen(stringToWrite));
                                close(toclientfd);
                                flagError = 1;
                                if(kill(atoi(line1), 5)==-1){
                                        printf("ERROR_FROM_EX2\n");
                                        flagError = 1;
                                }
                                else{
                                        printf("end of stage: server finished computing the solution\n");
                                }
                        }
                }
        if(flagError == 0){
                sprintf(stringToWrite, "%d", result);
                // printf("string to write %s\n", stringToWrite);
                write(toclientfd, &stringToWrite, strlen(stringToWrite));
                close(toclientfd);
                if(kill(atoi(line1), 5)==-1){
                        printf("ERROR_FROM_EX2\n");
                        flagError = 1;
                }
                else{
                        printf("end of stage: server finished computing the solution\n");
                }
        }
        remove("to_srv.txt");
        timer = 60;
        raise(9);
        } else {
        wait(&status);
        }
}
int main(){
        signal(5, handler5);
        while(timer > 0){
                sleep(1);
                timer--;
        }
        exit(0);
}
