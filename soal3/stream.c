#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdint.h>
#include <ctype.h>

#define SHM_KEY 1234
#define SEM_NAME "/mySemaphore"

typedef struct {
    pid_t pid1;
    pid_t pid2;
} pid_shared_t;

char* hexToASCII(const char* hex) {
    size_t len = strlen(hex);
    if (len % 2 != 0) return NULL;

    size_t output_len = len / 2;
    char* output = malloc(output_len + 1);
    if (output == NULL) return NULL;

    for (size_t i = 0; i < output_len; i++) {
        char byte[3] = {hex[i * 2], hex[i * 2 + 1], '\0'};
        output[i] = (char)strtol(byte, NULL, 16);
    }
    output[output_len] = '\0';

    return output;
}

static char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P','Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};
static char *decoding_table = NULL;
static int mod_table[] = {0, 2, 1};
 
void build_decoding_table() {
 
    decoding_table = malloc(256);
 
    for (int i = 0; i < 64; i++)
        decoding_table[(unsigned char) encoding_table[i]] = i;
}

unsigned char *base64ToASCII(const char *data,
                             size_t input_length,
                             size_t *output_length) {
 
    if (decoding_table == NULL) build_decoding_table();
 
    if (input_length % 4 != 0) return NULL;
 
    *output_length = input_length / 4 * 3;
    if (data[input_length - 1] == '=') (*output_length)--;
    if (data[input_length - 2] == '=') (*output_length)--;
 
    unsigned char *decoded_data = malloc(*output_length);
    unsigned char *hasil_decode = malloc(*output_length);

    if (decoded_data == NULL) return NULL;
 
    for (int i = 0, j = 0; i < input_length;) {
 
        uint32_t sextet_a = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_b = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_c = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_d = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
 
        uint32_t triple = (sextet_a << 3 * 6)
        + (sextet_b << 2 * 6)
        + (sextet_c << 1 * 6)
        + (sextet_d << 0 * 6);
 
        if (j < *output_length) decoded_data[j++] = (triple >> 2 * 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> 1 * 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> 0 * 8) & 0xFF;
    }

    for (int i = 0; i < *output_length; i++){

        if(decoded_data[i] != 0 && decoded_data[i] != 127){
            hasil_decode[i] = decoded_data[i];
        }
        else break;
    }
 
    return hasil_decode;
}
 
char *rot13ToASCII(char *src){

    if(src == NULL) return NULL;

    char* result = malloc(strlen(src) + 1);

    if(result != NULL){
        strcpy(result, src);

        char* current_char = result;

        while(*current_char != '\0'){

            if((*current_char >= 97 && *current_char <= 122) || (*current_char >= 65 && *current_char <= 90)){

                if(*current_char > 109 || (*current_char > 77 && *current_char < 91)){

                    *current_char -= 13;
                }
                else{
                    *current_char += 13;
                }
            }
            current_char++;
        }
    }

    return result;
}

void DECRYPT(){

    char* json_data;
    char *filename = "song-playlist.json";
    FILE *file = fopen(filename, "r");

    if(file == NULL){
        puts("Error: File can't be opened or doesn't exist");
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    json_data = (char*) malloc(file_size);

    fread(json_data, file_size, 1, file);

    fclose(file);

    char *json_start = strchr(json_data, '[');
    char *json_end = strchr(json_data, ']');

    char *method_start = strstr(json_start, "\"method\"");

    FILE *fp = fopen("playlist.txt", "w");

    while(method_start && method_start < json_end) {

        char *method_value_start = strchr(method_start, ':');
        char *method_value_end = strchr(method_value_start, ',');
        int method_value_len = method_value_end - method_value_start - 3;

        char method[method_value_len + 1];
        strncpy(method, method_value_start + 3, method_value_len);
        method[method_value_len] = '\0';

        strtok(method, "\"");

        printf("Method: %s\n", method);

        char *song_start = strstr(method_value_end, "\"song\"");
        char *song_value_start = strchr(song_start, ':');
        char *song_value_end = strchr(song_value_start, '}');
        long song_value_len = song_value_end - song_value_start - 3;

        char song[song_value_len + 1];
        strncpy(song, song_value_start + 3, song_value_len);
        song[song_value_len] = '\0';

        strtok(song, "\"");

        printf("Song: %s\n", song);
        
        if(strcmp(method, "rot13ToASCII") == 0){
            char *result = rot13ToASCII(song);
            printf("Result: %s\n",result);
            fprintf(fp, "%s\n", result);

        } if(strcmp(method, "base64ToASCII") == 0) {
            char* hasil_decode = base64ToASCII(song, song_value_len, &song_value_len);
            printf("Result: %s\n", hasil_decode);
            fprintf(fp, "%s\n", hasil_decode);

        } if(strcmp(method, "hex") == 0) {
            char* hexString = hexToASCII(song);
            printf("Result: %s\n", hexString);
            fprintf(fp, "%s\n", hexString);
        }
         
        method_start = strstr(song_value_end, "\"method\"");
    }

    fclose(fp);
}

int compare(const void* a, const void* b) {
    char* str1 = *(char**)a;
    char* str2 = *(char**)b;

    return strcmp(str1, str2);
}

void LIST() {

    FILE *file_song = fopen("playlist.txt", "r");
    if (file_song == NULL) {
        puts("Error: File can't be opened or doesn't exist");
    }

    char line[105];
    char lines[1005][105];
    char* uppercaseLines[1005];
    char* lowercaseLines[1005];
    char* numberLines[1005];
    int numUppercaseLines = 0;
    int numLowercaseLines = 0;
    int numNumberLines = 0;

    for(int i = 0; i < 1005 && fgets(line, 105, file_song); i++) {
        
        if (line[0] >= 65 && line[0] <= 90) {
            uppercaseLines[numUppercaseLines] = lines[i];
            strcpy(uppercaseLines[numUppercaseLines], line);
            numUppercaseLines++;
        } else if (line[0] >= 97 && line[0] <= 122) {
            lowercaseLines[numLowercaseLines] = lines[i];
            strcpy(lowercaseLines[numLowercaseLines], line);
            numLowercaseLines++;
        } else {
            numberLines[numNumberLines] = lines[i];
            strcpy(numberLines[numNumberLines], line);
            numNumberLines++;
        }
    }

    FILE *sort_file = fopen("sortingSong.txt", "w");

    qsort(uppercaseLines, numUppercaseLines, sizeof(char*), compare);
    qsort(lowercaseLines, numLowercaseLines, sizeof(char*), compare);
    qsort(numberLines, numNumberLines, sizeof(char*), compare);

    for(int i = 0; i < numNumberLines; i++){
        fputs(numberLines[i], sort_file);
        printf("%s\n", numberLines[i]);
    }

    for(int i = 0; i < numLowercaseLines; i++){
        fputs(lowercaseLines[i], sort_file);
        printf("%s\n", lowercaseLines[i]);
    }

    for(int i = 0; i < numUppercaseLines; i++){
        fputs(uppercaseLines[i], sort_file);
        printf("%s\n", uppercaseLines[i]);
    }

    fclose(sort_file);

}

void PLAY(const int x, const char* words){

    FILE * cek_song;
    char line_cek[150];
    char matchedLines[100][150];
    long numMatched = 0;

    cek_song = fopen("playlist.txt", "r");

    while(fgets(line_cek, 150, cek_song)){

        char* tmp = strdup(line_cek);

        for(int i = 0; i < strlen(tmp); i++){
            tmp[i] = tolower(tmp[i]);
        }

        char* wordLower = strdup(words);

        for(int i = 0; i < strlen(wordLower); i++){
            wordLower[i] = tolower(wordLower[i]);
        }

        if(strstr(tmp, wordLower)){
            
            strcpy(matchedLines[numMatched], line_cek);
            numMatched++;
        }
    }

    fclose(cek_song);

    if(numMatched == 1) printf("USER %d PLAYING \"%s\"\n", x, matchedLines[0]);
    else if(numMatched > 1){
        printf("THERE ARE %ld SONG CONTAINING \"%s\":", numMatched, words);
        puts("");
        
        for(long i = 0; i < numMatched; i++){
            printf("%ld. %s", i + 1, matchedLines[i]);
        }
    }
    else printf("THERE IS NO SONG CONTAINING \"%s\"\n", words);

}

void ADD(const int x, const char* newSong){

    FILE *add_song;
    add_song = fopen("playlist.txt", "r");

    char line[150];
    int flag = 0;
    while(fgets(line, 150, add_song)){

        if(strstr(line, newSong)){
            flag = 1;
            break;
        }
    }

    fclose(add_song);

    if(flag == 0){
        add_song = fopen("playlist.txt", "a");

        fputs(newSong, add_song);

        fclose(add_song);

        printf("USER %d ADD %s\n", x, newSong);
    }

    else puts("SONG ALREADY ON PLAYLIST");
}

struct msg_buf{
    int id;
    long msg_type;
    char msg_text[BUFSIZ];
};

int main(){
    int shem_id = shmget(SHM_KEY, sizeof(pid_shared_t), IPC_CREAT | 0666);
    if (shem_id == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    pid_shared_t *pid_data = shmat(shem_id, NULL, 0);
    if (pid_data == (void *) -1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    sem_t semaphore;
    if (sem_init(&semaphore, 1, 1) == -1) {
        perror("sem_init");
        exit(EXIT_FAILURE);
    }

    int running = 1;
    struct msg_buf data;

    long int msg_rcv = 0;

    key_t key = 12;

    int msg_id;
    msg_id = msgget(key, 0666|IPC_CREAT);

    int flag = 0;

    while(running){

        msgrcv(msg_id, (void*) &data, BUFSIZ, msg_rcv, 0);

        sem_wait(&semaphore);

        if ((pid_data->pid1 == 0 && data.id != pid_data->pid2) || data.id == pid_data->pid1) {
            pid_data->pid1 = data.id;

            if(strncmp(data.msg_text, "end", 3) == 0){
                printf("Process with PID %d is end.\n", data.id);

                printf("Data received: %s\n", data.msg_text);

                pid_data->pid1 = 0;

                flag = 0;

                sem_post(&semaphore);

            }
            
            else{
                printf("Process with PID %d is running.\n", data.id);
                flag = 1;
                sem_post(&semaphore);
            } 
        } 

        else if ((pid_data->pid2 == 0 && data.id != pid_data->pid1) || data.id == pid_data->pid2) {
            pid_data->pid2 = data.id;
            
            if(strncmp(data.msg_text, "end", 3) == 0){

                printf("Process with PID %d is end.\n", data.id);
                printf("Data received: %s\n", data.msg_text);

                pid_data->pid2 = 0;

                flag = 0;

                sem_post(&semaphore);

            }

            else {
                printf("Process with PID %d is running.\n", data.id);
                flag = 1;
                sem_post(&semaphore);
            }
        } 

        else{

            printf("Process with PID %d is unacceptable.\n", data.id);
            puts("STREAM SYSTEM OVERLOAD");
            flag = 0;
            sem_post(&semaphore);
        }

        sem_wait(&semaphore);

        if(flag == 1){
            
            printf("Data received: %s with id: %d\n", data.msg_text, data.id);

            if(strncmp(data.msg_text, "DECRYPT", 7) == 0){

                DECRYPT();
                puts("");
            }

            else if(strncmp(data.msg_text, "LIST", 4) == 0){

                LIST();
                puts("");
            }

            else if(strncmp(data.msg_text, "PLAY", 4) == 0){

                char *ptr1 = strchr(data.msg_text, '\"');
                char *ptr2;

                char lagu_input[200] = {0};
                int user_id = -1;

                if(ptr1 != NULL){

                    ptr1++;
                    ptr2 = strchr(ptr1, '\"');

                    if(ptr2 != NULL) strncpy(lagu_input, ptr1, ptr2 - ptr1);
                }

                PLAY(data.id, lagu_input);
            }
            

            else if(strncmp(data.msg_text, "ADD", 3) == 0){

                char *ptr1 = strstr(data.msg_text, "ADD ");
                if (ptr1 != NULL) {
                    ptr1 += 4;
                    char *ptr2 = strchr(ptr1, '\0');

                    while (*ptr1 == ' ') ptr1++;

                    while (*(ptr2 - 1) == ' ') ptr2--;

                    size_t len = ptr2 - ptr1;

                    char lagu_input[200];
                    strncpy(lagu_input, ptr1, len);
                    lagu_input[len] = '\0';

                    ADD(data.id, lagu_input);
                }
            }
        }
        sem_post(&semaphore);

        puts("");

    }
    sem_destroy(&semaphore);
    shmdt(pid_data);
    shmctl(shem_id, IPC_RMID, NULL);
    
    msgctl(msg_id, IPC_RMID, 0);

    return 0;
}