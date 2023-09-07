#include <stdio.h>
#include <stdlib.h>

int main() {
    char command[500];
    sprintf(command, "wget \"https://drive.google.com/uc?id=1rsR6jTBss1dJh2jdEKKeUyTtTRi_0fqp&export=download\" -O hehe.zip");

    int status = system(command);
    if(status == -1) {
        printf("Failed to download.\n");
    }

    char command2[100];
    sprintf(command2, "unzip hehe.zip");

    status = system(command2);
    if(status == -1) {
        printf("Failed to extract\n");
        return 1;
    }

    return 0;
}