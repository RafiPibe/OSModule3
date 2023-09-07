#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

struct msg_buf{

    int id;
    long msg_type;
    char msg_text[500];
};

int main(){

    struct msg_buf data;
    int msg_id;

    key_t key;
    key = 12;

    msg_id = msgget(key, 0666|IPC_CREAT);

    if(msg_id == -1){
        puts("Error creating queue\n");
        exit(0);
    }  

    int running = 1;
    char buffer_msg[200];

    while (running) {
        printf("Enter your command (Type \"Help\" for commands list): ");

        fgets(buffer_msg, 200, stdin);

        buffer_msg[strcspn(buffer_msg, "\n")] = '\0';

        if(strstr(buffer_msg, "DECRYPT") || strstr(buffer_msg, "LIST") || strstr(buffer_msg, "PLAY") || strstr(buffer_msg, "ADD")){
            data.msg_type = 1;

            data.id = getpid();

            strcpy(data.msg_text, buffer_msg);

            if(msgsnd(msg_id, (void*) &data, 500, 0) == -1) puts("Message not sent");

        } else if(strstr(buffer_msg, "quit")) {
            data.id = getpid();
            data.msg_type = 1;

            strcpy(data.msg_text, "quit");
            msgsnd(msg_id, (void*) &data, 500, 0);

            exit(EXIT_FAILURE);

        } else if (strcmp(buffer_msg, "Help") == 0) {
            printf("\u2022 Type \"PLAY \"Song_Title\"\" to play the song you want \n");
            printf("\u2022 Type \"LIST\" to show all the song inside the playlist \n");
            printf("\u2022 Type \"ADD \"Song_Title\"\" to add any song you want \n");
            printf("\u2022 Type \"DECRYPT\" to decrypt the playlist file \n");
            printf("\u2022 Type \"quit\" to quit the program \n");

        } else {
            data.id = getpid();
            data.msg_type = 1;
            
            strcpy(data.msg_text, "UNKNOWN COMMAND");
            msgsnd(msg_id, (void*) &data, 500, 0);
        }

    }
    return 0;
}
