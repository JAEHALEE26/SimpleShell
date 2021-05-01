//
//  main.c
//  projectOh_01
//
//  Created by Jae ha Lee on 2021/03/21.
//

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/fcntl.h>
#include <errno.h>

#define MAX_LINE 80 /* The maximum length command */
#define    BUFFER_SIZE    25

void split(char cmdline[], char* args[]);

int main(void)
{

    char *args[MAX_LINE/2 + 1]; /* command line arguments */
    int should_run = 1; /* flag to determine when to exit program */
    char cmdline[MAX_LINE];
    char cmdline2[MAX_LINE];
    char cmdline3[MAX_LINE];
    int fdr, i;
    char *pch;
    char *command1[4];
    char *command2[5];
    char *text1 = NULL;
    char *text2 = NULL;
    char *text3 = NULL;
    
    while (should_run) {
        
        // processing user_input
        printf("osh>");
        fflush(stdout);
        fgets(cmdline, MAX_LINE, stdin);
        cmdline[strlen(cmdline)-1] = '\0';
        strcpy(cmdline2, cmdline);
        strcpy(cmdline3, cmdline);
        
        
        split(cmdline, args);
        
        
        if(strcmp(args[0], "exit") == 0)
        {
            printf("종료합니다\n");
            break;
        }
        
        if((strchr(cmdline2,'&') != NULL)){
            int status, amppid;
            int isbg = 1;
            
            if((strchr(cmdline2,'>') != NULL)){
                text1 = strtok(cmdline2, "&");
                text2 = strtok(cmdline2, ">");
                text3 = strtok(NULL, ">");
                
                i=0;
                pch = strtok(text1, " ");
                while (pch != NULL && i<3)
                {
                    command1[i] = pch;
                    pch = strtok (NULL, " ");
                    i++;
                }
                command1[i]=(char*)0;
                
                i=0;
                pch = strtok(text3, " ");
                while (pch != NULL && i<3)
                {
                    command2[i] = pch;
                    pch = strtok (NULL, " ");
                    i++;
                }
                command2[i]=(char*)0;
                

                if((amppid=fork()) < 0)
                    perror("fork failed");
                else if (amppid > 0){
                    printf("[%d]\n", getpid());
                    waitpid(amppid, &status, WNOHANG);
                    pid_t waitPid = wait(&status);

                    if(waitPid == -1) {
                        printf("에러 넘버 : %d \n",errno);
                        perror("wait 함수 오류 반환");
                    }
                    else {
                        if(WIFEXITED(status)) {
                            printf("\ndone    %s\n", cmdline3);
                        }
                        else if(WIFSIGNALED(status)) {
                            printf("wait : 자식 프로세스 비정상 종료 %d\n",WTERMSIG(status));
                        }
                    }
                }

                else {
                    fdr = open(command2[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    if(fdr==-1) {
                        perror("파일 새로생성 오류1");
                        exit(1);
                        break;
                    }
                    if( dup2(fdr, 1) == -1){
                        perror("fdr dup error");
                        break;
                    }
                    close(fdr);
                    execvp(command1[0], command1);
                    printf("command not found \n");
                    exit(0);
                    break;
                }
                
            }
            
            else if((strchr(cmdline2,'<') != NULL)){
                text1 = strtok(cmdline2, "&");
                text2 = strtok(cmdline2, "<");
                text3 = strtok(NULL, "<");
                
                i=0;
                pch = strtok(text1, " ");
                while (pch != NULL && i<3)
                {
                    command1[i] = pch;
                    pch = strtok (NULL, " ");
                    i++;
                }
                command1[i]=(char*)0;
                
                i=0;
                pch = strtok(text3, " ");
                while (pch != NULL && i<3)
                {
                    command2[i] = pch;
                    pch = strtok (NULL, " ");
                    i++;
                }
                command2[i]=(char*)0;
                
                if((amppid=fork()) < 0)
                    perror("fork failed");
                else if (amppid > 0){
                    printf("[%d]\n", getpid());
                    waitpid(amppid, &status, WNOHANG);
                    pid_t waitPid = wait(&status);

                    if(waitPid == -1) {
                        printf("에러 넘버 : %d \n",errno);
                        perror("wait 함수 오류 반환");
                    }
                    else {
                        if(WIFEXITED(status)) {
                            printf("\ndone    %s\n", cmdline3);
                        }
                        else if(WIFSIGNALED(status)) {
                            printf("wait : 자식 프로세스 비정상 종료 %d\n",WTERMSIG(status));
                        }
                    }
                }
                else {
                    fdr = open(command2[0], O_RDONLY);
                    if(fdr==-1) {
                        perror("파일 새로생성 오류2");
                        exit(1);
                        break;
                        
                    }
                    if(dup2(fdr, 0) == -1){
                        perror("fdr dup error");
                    }
                    close(fdr);
                    execvp(command1[0], command1);
                    printf("command not found \n");
                    exit(0);
                    break;
                }
                
            }
            
            else if((strchr(cmdline2,'|') != NULL)){
                int fd[2];
                text1 = strtok(cmdline2, "&");
                text2 = strtok(cmdline2, "|");
                text3 = strtok(NULL, "|");
                
                i=0;
                pch = strtok(text1, " ");
                while (pch != NULL && i<3)
                {
                    command1[i] = pch;
                    pch = strtok (NULL, " ");
                    i++;
                }
                command1[i]=(char*)0;
                
                i=0;
                pch = strtok(text3, " ");
                while (pch != NULL && i<3)
                {
                    command2[i] = pch;
                    pch = strtok (NULL, " ");
                    i++;
                }
                command2[i]=(char*)0;
                
                if(pipe(fd) == -1){
                    printf("fail to call pipe()\n");
                    exit(1);

                }
                
                if((amppid=fork()) < 0)
                    perror("fork failed");
                else if (amppid > 0){
                    printf("[%d]\n", getpid());
                    waitpid(amppid, &status, WNOHANG);
                    pid_t waitPid = wait(&status);

                    if(waitPid == -1) {
                        printf("에러 넘버 : %d \n",errno);
                        perror("wait 함수 오류 반환");
                    }
                    else {
                        if(WIFEXITED(status)) {
                            printf("\ndone    %s\n", cmdline3);
                        }
                        else if(WIFSIGNALED(status)) {
                            printf("wait : 자식 프로세스 비정상 종료 %d\n",WTERMSIG(status));
                        }
                    }
                    
                }
                else {
                    if(close(1)==-1) perror("close1");
                        dup(fd[1]);
                        if(close(fd[0]) == -1 || close(fd[1]) == -1)perror("close2");
                        execvp(command1[0], command1);
                        printf("command not found \n");
                        exit(0);
                }
                
                if((amppid=fork()) < 0)
                    perror("fork failed");
                else if (amppid > 0){
                    if(isbg == 0)
                        amppid = wait(&status);
                    else{
                        waitpid(amppid, &status, WNOHANG);
                    }
                }
                else {
                    if(close(0) == -1) perror("close3");
                    dup(fd[0]);
                    if(close(fd[0]) == -1 || close(fd[1]) == -1) perror("close4");
                    execvp(command2[0], command2);
                    printf("command not found \n");
                    exit(0);
                }
                
            }
            
            else{
        
                text1 = strtok(cmdline2, "&");
                text2 = strtok(NULL, "&");
            

                i=0;
                pch = strtok(text1, " ");
                while (pch != NULL && i<3)

                {
                    command1[i] = pch;
                    pch = strtok (NULL, " ");
                    i++;
                }
                command1[i]=(char*)0;
            
                i=0;
                pch = strtok(text2, " ");
                while (pch != NULL && i<3)
                {
                    command2[i] = pch;
                    pch = strtok (NULL, " ");
                    i++;
                }
                command2[i]=(char*)0;
            
                if((amppid=fork()) < 0)
                    perror("fork failed");

                else if (amppid > 0){
                    printf("[%d]\n", getpid());
                    waitpid(amppid, &status, WNOHANG);
                    pid_t waitPid = wait(&status);

                    if(waitPid == -1) {
                        printf("에러 넘버 : %d \n",errno);
                        perror("wait 함수 오류 반환");
                    }
                    else {
                        if(WIFEXITED(status)) {
                            printf("\ndone    %s\n", cmdline3);
                        }
                        else if(WIFSIGNALED(status)) {
                            printf("wait : 자식 프로세스 비정상 종료 %d\n",WTERMSIG(status));
                        }
                    }
                }

                else {
                    execvp(command1[0], command1);
                    
                }
            }
        }
                  
        

        else if((strchr(cmdline2,'|') != NULL)){
            int fd[2];
        
            text1 = strtok(cmdline2, "|");
            text2 = strtok(NULL, "|");
            
            if(strcmp(args[0], "exit") == 0)
            {

                printf("종료합니다\n");
                break;
            }

            i=0;
            pch = strtok(text1, " ");
            while (pch != NULL && i<3)
            {
                command1[i] = pch;
                pch = strtok (NULL, " ");
                i++;
            }
            command1[i]=(char*)0;
            
            i=0;
            pch = strtok(text2, " ");
            while (pch != NULL && i<3)
            {
                command2[i] = pch;
                pch = strtok (NULL, " ");
                i++;
            }
            command2[i]=(char*)0;
            
            if(pipe(fd) == -1){
                printf("fail to call pipe()\n");
                exit(1);

            }
               
            switch(fork())
            {
                case -1 : perror("fork error"); break;
                case 0 :
                    if(close(1)==-1) perror("close1");
                    dup(fd[1]);
                    if(close(fd[0]) == -1 || close(fd[1]) == -1)perror("close2");
                    execvp(command1[0], command1);
                    printf("command not found \n");
                    exit(0);
                    
            }
               
            switch(fork())
            {
                case -1 : perror ("fork"); break;
                case 0 :
                    if(close(0) == -1) perror("close3");
                            dup(fd[0]);
                    if(close(fd[0]) == -1 || close(fd[1]) == -1) perror("close4");
                    execvp(command2[0], command2);
                    printf("command not found \n");
                    exit(0);
                
            }
            
        }
        
        else if((strchr(cmdline2,'>') != NULL)){
            text1 = strtok(cmdline2, ">");
            text2 = strtok(NULL, ">");
            
            
            if(strcmp(args[0], "exit") == 0)
            {
                printf("종료합니다\n");
                break;
            }

            i=0;
            pch = strtok(text1, " ");
            while (pch != NULL && i<3)
            {
                command1[i] = pch;
                pch = strtok (NULL, " ");
                i++;
            }
            command1[i]=(char*)0;
            
            i=0;
            pch = strtok(text2, " ");
            while (pch != NULL && i<3)
            {
                command2[i] = pch;
                pch = strtok (NULL, " ");
                i++;
            }
            command2[i]=(char*)0;

            switch(fork())
            {
                case -1 : perror ("fork"); break;
                case 0 :
                    fdr = open(command2[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    if(fdr==-1) {
                        perror("파일 새로생성 오류1");
                        exit(1);
                        break;
                        
                    }
                    if( dup2(fdr, 1) == -1){
                        perror("fdr dup error");
                        break;
                    }
                    close(fdr);
                    execvp(command1[0], command1);
                    printf("command not found \n");
                    exit(0);
                    break;
                default :
                    wait(NULL);
                    break;
            }
        }
        
        else if((strchr(cmdline2,'<') != NULL)){
            text1 = strtok(cmdline2, "<");
            text2 = strtok(NULL, "<");
            
            i=0;
            pch = strtok(text1, " ");
            while (pch != NULL && i<3)
            {
                command1[i] = pch;
                pch = strtok (NULL, " ");
                i++;
            }
            command1[i]=(char*)0;
            
            i=0;
            pch = strtok(text2, " ");
            while (pch != NULL && i<3)
            {
                command2[i] = pch;
                pch = strtok (NULL, " ");
                i++;
            }
            command2[i]=(char*)0;

            switch(fork())
            {
                case -1 : perror ("fork"); break;
                case 0 :
                    fdr = open(command2[0], O_RDONLY);
                    if(fdr==-1) {
                        perror("파일 새로생성 오류2");
                        exit(1);
                        break;
                        
                    }
                    if(dup2(fdr, 0) == -1){
                        perror("fdr dup error");
                    }
                    close(fdr);
                    execvp(command1[0], command1);
                    printf("command not found \n");
                    exit(0);
                    break;
                default :
                    wait(NULL);
            }
        }
        
        
        else{
            cmdline2[strlen(cmdline2)]='\0';

            i=0;
            pch = strtok (cmdline2," ");
            while (pch != NULL && i<3)
            {
              command1[i]=pch;
                pch = strtok (NULL, " ");
             i++;
            }
            command1[i]=(char*)0;
            
            pid_t pid;
            pid = fork();
            if(pid < 0) {
                printf("fork Fail! \n");
                exit(1);
            }
            
            else if(pid == 0){
                execvp(command1[0],command1);
            }
            else {
                wait(NULL);
            }
        }
    }
}

void split(char cmdline[], char* args[]){
    int i = 0;
    char* token = strtok(cmdline, " ");
    while (token != NULL) {
        args[i] = token;
        i++;
        token = strtok(NULL, " ");
            }
}
