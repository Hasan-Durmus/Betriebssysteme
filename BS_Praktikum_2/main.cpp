#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#define MAX_INPUT 1024  // Maximale Länge der Eingabezeile
#define MAX_ARGS 100    // Maximale Anzahl der Argumente in einem Befehl
#define MAX_BACKGROUND_PROCESS 50 // Maximale Anzahl an Hintergrundprozesse;
#define MAX_BACKGROUND_PID 50

pid_t pid_list[MAX_BACKGROUND_PID];
int pid_count = 0;

void read_input(char *user_input){

    printf("Best Shell ever: ");

    if(fgets(user_input,MAX_INPUT,stdin) == NULL){
        perror("There is no Programm with that name \n");
        exit(EXIT_FAILURE);
    }
    size_t input_length = strlen(user_input);
    if(input_length > 0 && user_input[input_length -1] == '\n'){
        user_input[input_length - 1] = '\0';

    }
}

void add_pid(pid_t background_pid){
    if(pid_count < MAX_BACKGROUND_PID)
        pid_list[pid_count++]  =   background_pid;
    else
        printf("ERROR: Can not create more background process\n");
    printf("pid_count : %d", pid_count);
}

void remove_pid(pid_t pid){
    for(int i = 0; i < pid_count; i++){
        if(pid_list[i] == pid){
            for(int j = i; j < pid_count - 1; j++){
                pid_list[j] = pid_list[j + 1];
            }
            pid_count --;
            break;
        }
    }
}

void check_backround_process(){
    int status;
    pid_t buffer_pid;

    for(int i = 0; i < pid_count; i++){
        buffer_pid = waitpid(pid_list[i], &status, 1);
        if(buffer_pid > 0){
            printf("pid %d finished \n",buffer_pid);
            if(WIFEXITED(status))
                printf("Exited with status %d.\n",WIFEXITED(status));
            else if(WIFSIGNALED(status))
                printf("Signal exited with status %d \n",WTERMSIG(status));
            remove_pid(buffer_pid);

        }
        else if (buffer_pid < 0 && errno != ECHILD){
            perror("Error: waitpid");
        }
    }
}
void extract_arguments(char *user_input, char *args[], bool *background){
    char *extract = strtok(user_input," ");
    int i = 0;
    while(extract != NULL){

        args[i++] = extract;
        extract = strtok(NULL," ");
    }

    args[i] = NULL;

    if(strcmp(args[i-1], "&") == 0 && i > 0){
        *background = 1;
        args[i-1] = NULL;

    }


}

int main(){
    char user_input[MAX_INPUT];
    char *args[MAX_ARGS];
    bool background_flag;
    char background_list[MAX_BACKGROUND_PROCESS];

    while(1){

    read_input(user_input);

        if(strcmp(user_input,"exit") == 0){
            printf("Do you want to exit Shell ? (y/n):");
            char answer;
            scanf("%c",&answer);
            if(answer =='y' || answer =='Y'){
                printf("Exiting Shell \n");
                break;
            }
            else{
                continue;
            }


        }
        extract_arguments(user_input, args, &background_flag);

        pid_t new_process = fork();

        if(new_process < 0){
            perror("Couldnt creat new process");
            continue;
        }

        if(new_process == 0){
            if(execvp(args[0],args) < 0){
                perror("Couldnt find programm");
                exit(EXIT_FAILURE);
            }
        }
        else{

            int status;

            switch(background_flag){
                case 0:{
                    printf("Started process with PID : %d \n", new_process);
                    waitpid(new_process,&status,0);
                    if(new_process > 0){
                        printf("Process closed. PID : %d \n", new_process);
                        if(WIFEXITED(status))
                            printf("Exited with status %d.\n",WIFEXITED(status));
                        else if(WIFSIGNALED(status))
                            printf("Signal exited with status %d \n",WIFSIGNALED(status));

                    }
                    background_flag = 0;
                    break;
                }
                case 1:{
                    printf("Started background process with PID : %d \n", new_process);
                    add_pid(new_process);
                    check_backround_process();
                    background_flag = 0;
                    break;
                }
            }
                printf("pidcount = %d \n", pid_count);
        }




    }

}
