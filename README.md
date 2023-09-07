 # <b><i>sisop-praktikum-modul-03-2023-IF184402-UO4</i></b>

## Group UO4

This is our attempt on the 3nd Practicum of Operation System 
consisting the members as following:

| Name                        | NRP        |
|-----------------------------|------------|
|Hanifi Abrar Setiawan        | 5025211066 |
|Faraihan Rafi Adityawarman   | 5025211074 |
|Vija Wildan Gita Prabawa     | 5025211261 |

## <b>1st Problem</b>

Lord Maharaja Baginda El Capitano Harry Maguire, S.Or., S.Kom yang dulunya seorang pemain bola, sekarang sudah pensiun dini karena sering blunder dan merugikan timnya. Ia memutuskan berhenti dan beralih profesi menjadi seorang programmer. Layaknya bola di permainan sepak bola yang perlu dikelola, pada dunia programming pun perlu adanya pengelolaan memori. Maguire berpikir bahwa semakin kecil file akan semakin mudah untuk dikelola dan transfer file-nya juga semakin cepat dan mudah. Dia lantas menemukan Algoritma Huffman untuk proses kompresi lossless. Dengan kepintaran yang pas-pasan dan berbekal modul Sisop, dia berpikir membuat program untuk mengkompres sebuah file. Namun, dia tidak mampu mengerjakannya sendirian, maka bantulah Maguire membuat program tersebut! 
(Wajib menerapkan konsep pipes dan fork seperti yang dijelaskan di modul Sisop. Gunakan 2 pipes dengan diagram seperti di modul 3).

### Answer and Explanation

```c
#define MAX_TREE_HT 100
```


```c
// To Check the frequency of each character.
typedef struct CharacterReq {
    char character;
    int amount;
} CharacterReq;

// To save the huffman tree inside a struct
typedef struct HuffmanMemory {
    char s;
    char huffman[20];
    int length;
} HuffmanMemory;

// A Huffman tree node
struct MinHeapNode {
 
    // One of the input characters
    char data;
 
    // Frequency of the character
    unsigned freq;
 
    // Left and right child of this node
    struct MinHeapNode *left, *right;
};
 
// A Min Heap:  Collection of
// min-heap (or Huffman tree) nodes
struct MinHeap {
 
    // Current size of min heap
    unsigned size;
 
    // capacity of min heap
    unsigned capacity;
 
    // Array of minheap node pointers
    struct MinHeapNode** array;
};
 
// A utility function allocate a new
// min heap node with given character
// and frequency of the character
struct MinHeapNode* newNode(char data, unsigned freq)
{
    struct MinHeapNode* temp = (struct MinHeapNode*)malloc(
        sizeof(struct MinHeapNode));
 
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;
 
    return temp;
}
 
// A utility function to create
// a min heap of given capacity
struct MinHeap* createMinHeap(unsigned capacity)
 
{
 
    struct MinHeap* minHeap
        = (struct MinHeap*)malloc(sizeof(struct MinHeap));
 
    // current size is 0
    minHeap->size = 0;
 
    minHeap->capacity = capacity;
 
    minHeap->array = (struct MinHeapNode**)malloc(
        minHeap->capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}
```

To define the struct used so it could be easier to use later on.

```c
char kal[30][15];
int count = -1;
// A utility function to print an array of size n
void printArr(int arr[], int n, int count)
{
    int i;
    for (i = 0; i < n; i++){
        sprintf(&kal[count][i + 3], "%d", arr[i]);
    }
}
 
// Prints huffman codes from the root of Huffman Tree.
// It uses arr[] to store codes
void printCodes(struct MinHeapNode* root, int arr[],
                int top)
 
{   
 
    // Assign 0 to left edge and recur
    if (root->left) {
 
        arr[top] = 0;
        printCodes(root->left, arr, top + 1);
    }
 
    // Assign 1 to right edge and recur
    if (root->right) {
 
        arr[top] = 1;
        printCodes(root->right, arr, top + 1);
    }

    char str[5];
 
    // If this is a leaf node, then
    // it contains one of the input
    // characters, print the character
    // and its code from arr[]
    if (isLeaf(root)) {
        
        count++;

        sprintf(str, "%c: ", root->data);
        sprintf(kal[count], "%s", str);

        printArr(arr, top, count);

    }
}

// The main function that builds a
// Huffman Tree and print codes by traversing
// the built Huffman Tree
void HuffmanCodes(char data[], int freq[], int size)
 
{
    // Construct Huffman Tree
    struct MinHeapNode* root = buildHuffmanTree(data, freq, size);
 
    // Print Huffman codes using
    // the Huffman tree built above
    int arr[MAX_TREE_HT], top = 0;
 
    printCodes(root, arr, top);
}
```

generate and print the Huffman codes for a given set of characters and their frequencies. The `printArr()` function is a utility function that takes an integer array and converts it to a string by appending each element to a character array. The `printCodes()` function recursively traverses the Huffman tree and generates the binary code for each leaf node. Finally, the `main()` function reads the input data, constructs the Huffman tree, and calls `printCodes()` to generate and print the binary codes for each input symbol.

```c
void count_char(int fdParentChild, const char* inputFileName) {
    CharacterReq CharReq[256];
    size_t struct_size = sizeof(CharacterReq);
    char asciiChar;

    // Open file
    FILE *file;
    file = fopen(inputFileName, "r");

    if(file == NULL) {
        printf("no file with the name file.txt\n");
    }

    char alphabetChar[] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 
        'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 
        'U', 'V', 'W', 'X', 'Y', 'Z'
    };

    for(int i = 0; i < 26; i++){
        CharReq[i].character = alphabetChar[i];
        CharReq[i].amount = 0;
    }

    while((asciiChar = fgetc(file)) != EOF){
        if((asciiChar >= 65 && asciiChar <= 90) || (asciiChar >= 97 && asciiChar <= 122)){
            asciiChar = toupper(asciiChar);
            for(int i = 0;i < 26; i++){
                if(asciiChar == CharReq[i].character){
                    CharReq[i].amount++;
                    break;
                }
            }

        }
    }

    // Write into pipe
    write(fdParentChild, CharReq, struct_size * 26);
}
```

reads an input file character by character and counts the number of occurrences of each letter (both uppercase and lowercase) in the file. This information is stored in an array of structures, where each structure represents a letter and the number of times it appears in the file. The function then writes this array of structures to a pipe so that it can be read by another process. Overall, the function demonstrates how to count the occurrences of letters in a file and communicate this information between processes using a pipe.

```c
int main(int argc, char* argv[]){
    ...
}
```

main function

```c
    const char* inputFileName = "file.txt";
    const char* outputFileName = "compressed.txt";

    int pipefd[2];
    int pipefd2[2];
    pid_t pid;

    HuffmanMemory HuffmanMem[30];

    if(pipe(pipefd) == -1 || pipe(pipefd2) == -1){
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if(pid == -1){
        perror("fork");
        exit(EXIT_FAILURE);
    }
```

initializing inputFileName, outputFileName, pipe, pid, and forking the pid.

```c
if(pid == 0){ 
        // Child process

        CharacterReq h2[26];
        // Close the unused write end of the pipe
        close(pipefd[1]);

        read(pipefd[0], h2, sizeof(h2));
        close(pipefd[0]);

        char alphabetChar[26];
        int frequency[26];
        int j = 0;
        for (int i = 0; i < 26; i++) {
            if (h2[i].amount > 0) {
                alphabetChar[j] = h2[i].character;
                frequency[j] = h2[i].amount;
                j++;
            }
        }
        HuffmanCodes(alphabetChar, frequency, j);
        
        for (int i = 0; i < 26; i++) {

            if (write(pipefd2[1], kal[i], strlen(kal[i])) < 0) {
                perror("Error writing to pipe");
                exit(EXIT_FAILURE);
            }

            if (write(pipefd2[1], "\n", 1) < 0) {
                perror("Error writing to pipe");
                exit(EXIT_FAILURE);
            }

            usleep(1000);
        }

        close(pipefd2[1]);
    }
```


The child process. It first reads from a pipe that was previously written to by the parent process, which contains an array of structures representing the frequency of each letter in an input file. It then extracts the non-zero frequencies from the array and passes them to the HuffmanCodes function along with the corresponding letters.

The resulting Huffman codes are then written to another pipe using write, along with newline characters. usleep is used to pause briefly between each write operation, which can help prevent issues with the receiving process reading data out of order. Finally, the write end of the pipe is closed before the child process exits.

```c
{
        // Parent process
        
        // Close the unused write end of the pipe
        close(pipefd[0]);

        count_char(pipefd[1], inputFileName);
        close(pipefd[1]);

        char buffer[1024];
        int bytes_read;
        char data[50][50];
        char newdata[50][50];

        int row = 0;
        int x = 23;
        while (x--) {
            bytes_read = read(pipefd2[0], buffer, sizeof(buffer));

            // saving buffer to 2-dimensional array
            memcpy(data[row], buffer, bytes_read);
            data[row][bytes_read] = '\0'; // Menambahkan nul-terminator

            // search substring on every row and copy to `newdata` array
            char* substr = strstr(data[row], ": ");

            if(substr != NULL) {
                substr += 2;
                strncpy(newdata[row], substr, 50 - 1);
                newdata[row][50 - 1] = '\0'; // adding nul-terminator

                // Removing \n on newdata[row]
                char* newline_pos = strchr(newdata[row], '\n');
                if (newline_pos != NULL) {
                    *newline_pos = '\0';
                }

                // Saving to struct
                HuffmanMem[row].s = data[row][0];
                strncpy(HuffmanMem[row].huffman, newdata[row], sizeof(HuffmanMem[row].huffman) - 1);
                HuffmanMem[row].huffman[sizeof(HuffmanMem[row].huffman) - 1] = '\0';
            }

            HuffmanMem[row].length = strlen(HuffmanMem[row].huffman);
            row++;
        }

        if (bytes_read == -1) {
            perror("Error reading from pipe");
            exit(EXIT_FAILURE);
        }

        close(pipefd2[0]);

        for(int i = 0; i < 23; i++){
            printf("%s", data[i]);
        }

        FILE *file;
        file = fopen(inputFileName, "r");

        FILE *outputFile = fopen(outputFileName, "w");
        if (outputFile == NULL) {
            printf("Error opening %s\n", outputFileName);
            exit(1);
        }

        if(file == NULL) {
            printf("%s can't be open", inputFileName);
        }

        char ch;
        while((ch = fgetc(file)) != EOF){

            if((ch >= 65 && ch <= 90) || (ch >= 97 && ch <= 122)){

                ch = toupper(ch);
                for(int i = 0; i < 26; i++){
                    if(ch == HuffmanMem[i].s){
                            
                        fprintf(outputFile, "%s ", HuffmanMem[i].huffman);
                    }
                }
            }
        }

        file = fopen(inputFileName, "r");

        int bit_before = 0, bit_after = 0;
        while((ch = fgetc(file)) != EOF){

            if((ch >= 65 && ch <= 90) || (ch >= 97 && ch <= 122)){
                bit_before += 8;

                ch = toupper(ch);

                for(int i = 0; i < 26; i++){

                    if(ch == HuffmanMem[i].s){
                        bit_after += HuffmanMem[i].length;

                        break;
                    }
                }
            }
            else continue;
        }

        printf("\nBefore: %d\n", bit_before);
        printf("After: %d\n", bit_after);

    }
```

This code is the parent process that implements Huffman coding to compress text files. It reads characters from a text file using fgetc(), then converts them to uppercase and looks up their corresponding Huffman codes in a struct array. It then writes the Huffman codes to an output file using fprintf(). Finally, it calculates the number of bits before and after the compression process and prints them to the console.

In addition, the code reads from a second pipe (pipefd2) to receive the Huffman codes from the child process. It saves the Huffman codes in a struct array (HuffmanMem) and uses them to compress the text file.

Before reading the Huffman codes from the pipe, the code reads data from the pipe into a 2-dimensional array (data). It then searches for a substring (: ) in each row of the array, and if found, saves the Huffman code to another 2-dimensional array (newdata) and the character and its Huffman code to the struct array (HuffmanMem). The length of each Huffman code is also saved to the struct array.

## <b>2nd Problem</b>

Fajar sedang sad karena tidak lulus dalam mata kuliah Aljabar Linear. Selain itu, dia tambah sedih lagi karena bertemu matematika di kuliah Sistem Operasi seperti kalian 🥲. Karena dia tidak bisa ngoding dan tidak bisa menghitung, jadi Fajar memohon jokian kalian. Tugas Fajar adalah sebagai berikut.

#### Problem A 
Membuat program C dengan nama kalian.c, yang berisi program untuk melakukan perkalian matriks. Ukuran matriks pertama adalah 4×2 dan matriks kedua 2×5. Isi dari matriks didefinisikan di dalam code. Matriks nantinya akan berisi angka random dengan rentang pada matriks pertama adalah 1-5 (inklusif), dan rentang pada matriks kedua adalah 1-4 (inklusif). Tampilkan matriks hasil perkalian tadi ke layar.

```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define ROW 4
#define COL 5

int main() {
    key_t key = 4678; 
    int shmid;
    long long int (*hasil)[COL]; // Pointer ke matriks hasil
    int i, j, k;

    // Buat shared memory segment
    if ((shmid = shmget(key, sizeof(long long int[ROW][COL]), IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    // Hubungkan ke shared memory segment
    if ((hasil = shmat(shmid, NULL, 0)) == (long long int (*)[COL]) -1) {
        perror("shmat");
        exit(1);
    }

    // Isi matriks pertama dengan angka random
    long long int matriks1[ROW][2];
    for (i = 0; i < ROW; i++) {
        for (j = 0; j < 2; j++) {
            matriks1[i][j] = rand() % 5 + 1;
        }
    }

    // Isi matriks kedua dengan angka random
    long long int matriks2[2][COL];
    for (i = 0; i < 2; i++) {
        for (j = 0; j < COL; j++) {
            matriks2[i][j] = rand() % 4 + 1;
        }
    }

    // Perkalian matriks
    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COL; j++) {
            long long int sum = 0;
            for (k = 0; k < 2; k++) {
                sum += matriks1[i][k] * matriks2[k][j];
            }
            hasil[i][j] = sum;
        }
    }

    printf("Matriks hasil:\n");
    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COL; j++) {
            printf("%lld ", hasil[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    printf("Matriks 1:\n");
    for (i = 0; i < ROW; i++) {
        for (j = 0; j < 2; j++) {
            printf("%lld ", matriks1[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    printf("Matriks 2:\n");
    for (i = 0; i < 2; i++) {
        for (j = 0; j < COL; j++) {
            printf("%lld ", matriks2[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    // Detach shared memory
    shmdt(hasil);

    return 0;
}

```
#### Explanation
- The program includes several header files needed for input/output operations (stdio.h), memory allocation     (stdlib.h), and shared memory operations (sys/ipc.h, sys/shm.h).

- Two constants are defined using the #define directive: ROW is set to 4, and COL is set to 5. These constants define the dimensions of the matrices used in the program.

- The main() function begins.

- A key value of 4678 is assigned to the variable key. This key will be used to identify the shared memory segment.

- The variable shmid is declared to store the identifier of the shared memory segment.

- The variable hasil is declared as a pointer to a two-dimensional array of type long long int. This pointer will point to the shared memory segment.

- Three loop control variables i, j, and k are declared.

- The program attempts to create a shared memory segment using the shmget() function. The size of the shared memory segment is calculated using the sizeof operator with the dimensions of the hasil array. If the creation fails, an error message is printed, and the program exits.

- The program attaches to the shared memory segment using the shmat() function. If the attachment fails, an error message is printed, and the program exits.

- Two matrices, matriks1 and matriks2, are declared as two-dimensional arrays and initialized with random values using the rand() function. The values are generated within specific ranges.

- The program performs matrix multiplication on matriks1 and matriks2 and stores the result in the hasil matrix. This is done using three nested for loops: the outermost loop iterates over the rows of matriks1, the middle loop iterates over the columns of matriks2, and the innermost loop performs the multiplication and accumulation for each element of the resulting matrix.

- After the matrix multiplication is complete, the program prints the contents of the hasil matrix, matriks1, and matriks2 using nested for loops and the printf() function.

- The shared memory segment is detached using the shmdt() function.

- Finally, the program returns 0, indicating successful execution.

#### Problem b 
Buatlah program C kedua dengan nama cinta.c. Program ini akan mengambil variabel hasil perkalian matriks dari program kalian.c (program sebelumnya). Tampilkan hasil matriks tersebut ke layar. 

```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <pthread.h>
#include <sys/time.h>

#define ROW 4
#define COL 5

int main() {
    key_t key = 4678;
    int shmid;
    long long int (*hasil)[COL];
    int i, j;

    if ((shmid = shmget(key, sizeof(int[ROW][COL]), 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    // Hubungkan ke shared memory segment
    if ((hasil = shmat(shmid, NULL, 0)) == (long long int (*)[COL]) -1) {
        perror("shmat");
        exit(1);
    }

    // Tampilkan matriks hasil perkalian
    printf("Matriks hasil:\n");
    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COL; j++) {
            printf("%lld ", hasil[i][j]);
        }
        printf("\n");
    }
    printf("\n");


    // Detach shared memory
    shmdt(hasil);

    return 0;
}

```
#### Explanation
- The program includes several header files required for input/output operations (stdio.h), memory allocation (stdlib.h), shared memory operations (sys/shm.h), thread management (pthread.h), and time-related operations (sys/time.h).

- Two constants are defined using the #define directive: ROW is set to 4, and COL is set to 5. These constants represent the dimensions of the matrices used in the program.

- The main() function begins.

- A key value of 4678 is assigned to the variable key. This key is used to identify the shared memory segment.

- The variable shmid is declared to store the identifier of the shared memory segment.

- The variable hasil is declared as a pointer to a two-dimensional array of type long long int. This pointer will point to the shared memory segment.

- Two loop control variables, i and j, are declared.

- The program attempts to obtain the identifier of the shared memory segment using the shmget() function. The size of the shared memory segment is calculated using the sizeof operator with the dimensions of the array int[ROW][COL]. If the retrieval fails, an error message is printed, and the program exits.

- The program attaches to the shared memory segment using the shmat() function. If the attachment fails, an error message is printed, and the program exits.

- The program then proceeds to print the contents of the shared memory segment, which represents the result of a matrix multiplication operation. The printf() function is used in nested for loops to iterate over the rows and columns of the hasil matrix and print its elements.

- After printing the matrix, the shared memory segment is detached using the shmdt() function.

- Finally, the program returns 0, indicating successful execution.

#### Problem C 
Setelah ditampilkan, berikutnya untuk setiap angka dari matriks tersebut, carilah nilai faktorialnya. Tampilkan hasilnya ke layar dengan format seperti matriks.

```c

long long int faktorial(int n) {
    if (n == 0) {
        return 1;
    } else {
        return n * faktorial(n - 1);
    }
}

void *hitung_faktorial(void *arg) {
    long long int (*hasil)[COL] = arg;
    int i, j;

    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COL; j++) {
            hasil[i][j] = faktorial(hasil[i][j]);
        }
    }

    pthread_exit(NULL);
}


    // Mengukur waktu eksekusi program
struct timeval start, end;
gettimeofday(&start, NULL);

    // Hitung faktorial menggunakan thread
pthread_t tid;
pthread_attr_t attr;

pthread_attr_init(&attr);
pthread_create(&tid, &attr, hitung_faktorial, hasil);
pthread_join(tid, NULL);


    // Tampilkan matriks hasil faktorial
printf("Matriks hasil faktorial:\n");
    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COL; j++) {
            printf("%lld ", hasil[i][j]);
        }
        printf("\n");
}
printf("\n");

    // Menghitung waktu eksekusi program
gettimeofday(&end, NULL);
double start_time = start.tv_sec + (start.tv_usec / 1000000.0);
double end_time = end.tv_sec + (end.tv_usec / 1000000.0);
double cpu_time_used = end_time - start_time;

    // Tampilkan waktu eksekusi program
printf("Waktu yang dibutuhkan: %.6f detik\n", cpu_time_used);
printf("\n");

```

#### Explanation
- The code defines a function faktorial() that calculates the factorial of a given integer n. If n is 0, it returns 1. Otherwise, it recursively calls itself with n-1 and multiplies the result by n.

- The code defines a function hitung_faktorial() that takes a void pointer arg as an argument. It interprets arg as a pointer to a two-dimensional array of type long long int and performs factorial calculation on each element of the array. It uses the faktorial() function to calculate the factorial of each element and stores the result back in the array.

- The code initializes a structure start of type struct timeval and another structure end of the same type to measure the execution time of the program. The gettimeofday() function is used to obtain the current time and store it in start.

- The code creates a new thread using the pthread_create() function. It passes the thread ID tid, thread attributes attr, the hitung_faktorial function as the thread routine, and the hasil array as an argument to the thread routine.

- The code waits for the created thread to terminate using the pthread_join() function. This ensures that the main thread waits for the completion of the factorial calculation in the thread before proceeding.

- After the factorial calculation is completed, the code prints the resulting factorial matrix using nested for loops and the printf() function.

- The code measures the execution time of the program by obtaining the current time and storing it in end. It calculates the start time and end time in seconds, and then subtracts the start time from the end time to get the total CPU time used.

- The code prints the execution time of the program using the printf() function.

#### Problem D 
Buatlah program C ketiga dengan nama sisop.c. Pada program ini, lakukan apa yang telah kamu lakukan seperti pada cinta.c namun tanpa menggunakan thread dan multithreading. Buat dan modifikasi sedemikian rupa sehingga kamu bisa menunjukkan perbedaan atau perbandingan (hasil dan performa) antara program dengan multithread dengan yang tidak. 
Dokumentasikan dan sampaikan saat demo dan laporan resmi.

```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/time.h>

#define ROW 4
#define COL 5

long long int faktorial(int n) {
    if (n == 0) {
        return 1;
    } else {
        return n * faktorial(n - 1);
    }
}

void hitung_faktorial(long long int (*hasil)[COL]) {
    int i, j;

    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COL; j++) {
            hasil[i][j] = faktorial(hasil[i][j]);
        }
    }
}

int main() {
    key_t key = 4678;
    int shmid;
    long long int (*hasil)[COL];
    int i, j;

    if ((shmid = shmget(key, sizeof(int[ROW][COL]), 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    // Hubungkan ke shared memory segment
    if ((hasil = shmat(shmid, NULL, 0)) == (long long int (*)[COL]) -1) {
        perror("shmat");
        exit(1);
    }

    // Tampilkan matriks hasil perkalian
    printf("Matriks hasil:\n");
    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COL; j++) {
            printf("%lld ", hasil[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    // Mengukur waktu eksekusi program
    struct timeval start, end;
    gettimeofday(&start, NULL);

    // Hitung faktorial tanpa multithreading
    hitung_faktorial(hasil);

    // Tampilkan matriks hasil faktorial
    printf("Matriks hasil faktorial:\n");
    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COL; j++) {
            printf("%lld ", hasil[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    // Mengukur waktu eksekusi program
    gettimeofday(&end, NULL);
    double start_time = start.tv_sec + (start.tv_usec / 1000000.0);
    double end_time = end.tv_sec + (end.tv_usec / 1000000.0);
    double cpu_time_used = end_time - start_time;

    // Tampilkan waktu eksekusi program
    printf("Waktu yang dibutuhkan: %.6f detik\n", cpu_time_used);
    printf("\n");

    // Detach shared memory
    shmdt(hasil);

    return 0;
}

```
#### Explanation
- The code includes necessary header files for input/output operations (stdio.h), memory allocation (stdlib.h), shared memory operations (sys/shm.h), and time-related operations (sys/time.h).

- Two constants are defined using the #define directive: ROW is set to 4, and COL is set to 5. These constants represent the dimensions of the matrices used in the program.

- The code defines a function faktorial() that calculates the factorial of a given integer n. It uses a recursive approach: if n is 0, it returns 1; otherwise, it multiplies n with the factorial of n-1.

- The code defines a function hitung_faktorial() that takes a pointer to a two-dimensional array hasil of type long long int as an argument. It iterates over the elements of the array and replaces each element with its factorial calculated using the faktorial() function.

- The main() function begins.

- A key value of 4678 is assigned to the variable key. This key is used to identify the shared memory segment.

- The variable shmid is declared to store the identifier of the shared memory segment.

- The variable hasil is declared as a pointer to a two-dimensional array of type long long int. This pointer will point to the shared memory segment.

- Two loop control variables, i and j, are declared.

- The program attempts to obtain the identifier of the shared memory segment using the shmget() function. The size of the shared memory segment is calculated using the sizeof operator with the dimensions of the array int[ROW][COL]. If the retrieval fails, an error message is printed, and the program exits.

- The program attaches to the shared memory segment using the shmat() function. If the attachment fails, an error message is printed, and the program exits.

- The program then proceeds to print the contents of the shared memory segment, which represents the result of a matrix multiplication operation. The printf() function is used in nested for loops to iterate over the rows and columns of the hasil matrix and print its elements.

- The program measures the start time of the program's execution using the gettimeofday() function.

- The program calls the hitung_faktorial() function, passing the hasil matrix as an argument, to calculate the factorial of each element in the matrix.

- The program prints the resulting factorial matrix using nested for loops and the printf() function.

- The program measures the end time of the program's execution using the gettimeofday() function and calculates the CPU time used.

- The program prints the execution time of the program using the printf() function.

- The shared memory segment is detached using the shmdt() function.

- Finally, the program returns 0, indicating successful execution.
## <b>3rd Problem</b>

Elshe saat ini ingin membangun usaha sistem untuk melakukan stream lagu. Namun, Elshe tidak paham harus mulai dari mana.
Bantulah Elshe untuk membuat sistem stream (receiver) stream.c dengan user (multiple sender dengan identifier) user.c menggunakan message queue (wajib). Dalam hal ini, user hanya dapat mengirimkan perintah berupa STRING ke sistem dan semua aktivitas sesuai perintah akan dikerjakan oleh sistem.

#### Starter

```c
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
```

#### Explanation

These are the libraries that I will use for both `stream.c` and `user.c`

#### A

Bantulah Elshe untuk membuat sistem stream (receiver) stream.c dengan user (multiple sender dengan identifier) user.c menggunakan message queue (wajib). Dalam hal ini, user hanya dapat mengirimkan perintah berupa STRING ke sistem dan semua aktivitas sesuai perintah akan dikerjakan oleh sistem.

#### Answer

```c
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
```

#### Explanation

For the first problem we needed to make `stream.c` and `user.c` where `user.c` needed to have message queue. Here I'm showing the `user.c` file since the A problem can be answered mostly with this file

#### B

User pertama kali akan mengirimkan perintah DECRYPT kemudian sistem stream akan melakukan decrypt/decode/konversi pada file song-playlist.json (dapat diunduh manual saja melalui link berikut) sesuai metodenya dan meng-output-kannya menjadi playlist.txt diurutkan menurut alfabet.

#### Answer

```c
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
```

#### Explanation

- The `DECRYPT` function reads data from a JSON file, decrypts song information, and writes the decrypted data to a text file.
- It opens the JSON file, reads its contents, and stores them in `json_data`.
- The function iterates through the JSON data, extracting method and song values.
- Depending on the method value, it performs different decryption operations: `rot13ToASCII`, `base64ToASCII`, or `hexToASCII`.
- The decrypted results are printed and written to the "playlist.txt" file.
- The function closes the file and terminates.

#### C

Selain itu, user dapat mengirimkan perintah LIST, kemudian sistem stream akan menampilkan daftar lagu yang telah di-decrypt

#### Answer

```c
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
```

#### Explanation

- The function `LIST` reads a file named "playlist.txt" that contains song names.
- It opens the file using the `fopen` function and checks if it was opened successfully.
- It declares several variables: `line` to store a line from the file, lines as a 2D array to store all lines, `uppercaseLines`, `lowercaseLines`, and `numberLines` as arrays of pointers to store lines categorized by their first character, and `numUppercaseLines`, `numLowercaseLines`, and `numNumberLines` to keep track of the number of lines in each category.
- It uses a loop to read lines from the file using `fgets` and categorizes each line into `uppercaseLines`, `lowercaseLines`, or `numberLines` based on the first character of the line.
- The lines are copied into the respective arrays using `strcpy`, and the counters for each category are incremented accordingly.
- It opens a new file named "sortingSong.txt" using `fopen` in write mode.
- The function uses `qsort` to sort the lines in each category: `uppercaseLines`, `lowercaseLines`, and `numberLines`, using a comparison function called `compare`.
- It loops through `numberLines` and writes each line to the "sortingSong.txt" file using `fputs`. It also prints the line using `printf`.
- It does the same for `lowercaseLines` and `uppercaseLines`.

#### D

User juga dapat mengirimkan perintah PLAY <SONG>

#### Answer

```c
void PLAY(const int x, const char* words){

    FILE * songCheck;
    char lineCheck[150];
    char matchedLines[100][150];
    long numMatched = 0;

    songCheck = fopen("playlist.txt", "r");

    while(fgets(lineCheck, 150, songCheck)){

        char* tmp = strdup(lineCheck);

        for(int i = 0; i < strlen(tmp); i++){
            tmp[i] = tolower(tmp[i]);
        }

        char* wordLower = strdup(words);

        for(int i = 0; i < strlen(wordLower); i++){
            wordLower[i] = tolower(wordLower[i]);
        }

        if(strstr(tmp, wordLower)){
            
            strcpy(matchedLines[numMatched], lineCheck);
            numMatched++;
        }
    }

    fclose(songCheck);

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
```

#### Explanation

- The code defines a function named `PLAY` that takes two parameters: an integer x and a character pointer words.
- Inside the function, a file pointer `songCheck` and two character arrays, `lineCheck` and `matchedLines`, are declared.
- The variable `numMatched` is initialized to 0.
- The file "playlist.txt" is opened in read mode using `fopen`.
- A while loop is used to read each line from the file using `fgets` and store it in the `lineCheck` array.
- The `lineCheck` array is then converted to lowercase characters using a for loop and the `tolower` function.
- The words array is also converted to lowercase characters using a similar for loop.
- If the words array is found within the `lineCheck` array (case-insensitive search) using strstr, the current line is copied to the `matchedLines` array and `numMatched` is incremented.
- After reading all the lines from the file, the file is closed using `fclose`.
- Depending on the value of `numMatched`, different messages are printed to the console:
    - If `numMatched` is 1, it prints "USER x PLAYING 'matchedLines[0]'".
    - If `numMatched` is greater than 1, it prints "THERE ARE numMatched SONGS CONTAINING 'words':" followed by the matched lines.
    - If `numMatched` is 0, it prints "THERE IS NO SONG CONTAINING 'words'".


#### E

User juga dapat menambahkan lagu ke dalam playlist 

#### Answer

```c
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
```

#### Explanation

- The `ADD` function takes two parameters: an integer x and a pointer to a character `newSong`.
- It opens a file named "playlist.txt" in read mode using `fopen` and assigns the file pointer to the `add_song` variable.
- It declares a character array `line` with a size of 150 characters.
- It initializes a flag variable to 0.
- It enters a loop that reads lines from the file using `fgets`, with a maximum length of 150 characters per line.
- Within the loop, it checks if the `newSong` string is found within the line using `strstr`.
    - If a match is found, it sets the flag to 1 and breaks out of the loop.
- It closes the file using `fclose`.
- If the flag is still 0 (i.e., the song was not found in the playlist), it opens the file in append mode using `fopen` and assigns the file pointer back to `add_song`.
- It writes the `newSong` string to the file using `fputs`.
- It closes the file again using `fclose`.
- If the flag is 0, it prints a message indicating that the user (x) has added the song (`newSong`) to the playlist using `printf`.
- If the flag is 1, it prints a message stating that the song is already on the playlist using `puts`.


#### F

Karena Elshe hanya memiliki resource yang kecil, untuk saat ini Elshe hanya dapat memiliki dua user. Gunakan semaphore (wajib) untuk membatasi user yang mengakses playlist.
Output-kan "STREAM SYSTEM OVERLOAD" pada sistem ketika user ketiga mengirim perintah apapun.

#### Answer

```c
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

```

#### Explanation

- The code is checking conditions based on the values of `pid1`, `pid2`, and `data.id`.
- If `pid1` is 0 and `data.id` is not equal to `pid2`, or `data.id` is equal to `pid1`, the following actions are performed:
    - Assign `data.id` to `pid1`.
    - If the first three characters of data.msg_text are "end", print a message indicating the process with `data.id` has ended. Otherwise, print a message indicating the process with `data.id` is running.
    - If the message is "end", reset `pid1` to 0; otherwise, set `flag` to 1.
    - Release a semaphore using `sem_post`.
- If the above condition is not met and `pid2` is 0 and `data.id` is not equal to `pid1`, or `data.id` is equal to `pid2`, the following actions are performed:
    - Assign `data.id` to `pid2`.
    - If the first three characters of data.msg_text are "end", print a message indicating the process with `data.id` has ended. Otherwise, print a message indicating the process with `data.id` is running.
    - If the message is "end", reset `pid2` to 0; otherwise, set `flag` to 1.
    - Release a semaphore using `sem_post`.
- If none of the above conditions are met, print a message indicating that the process with `data.id` is unacceptable and display "STREAM SYSTEM OVERLOAD".
- Set `flag` to 0 and release a semaphore using `sem_post`.


#### G

Apabila perintahnya tidak dapat dipahami oleh sistem, sistem akan menampilkan "UNKNOWN COMMAND".

#### Answer

```c
else {
            data.id = getpid();
            data.msg_type = 1;
            
            strcpy(data.msg_text, "UNKNOWN COMMAND");
            msgsnd(msg_id, (void*) &data, 500, 0);
        }
```

#### Explanation

- Here if the user inputted any command that aren't included, it will print "UNKNOWN COMMAND"


## <b>4th Problem</b>

Suatu hari, Amin, seorang mahasiswa Informatika mendapati suatu file bernama hehe.zip. Di dalam file .zip tersebut, terdapat sebuah folder bernama files dan file .txt bernama extensions.txt dan max.txt. Setelah melamun beberapa saat, Amin mendapatkan ide untuk merepotkan kalian dengan file tersebut! 

#### Starter

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <ctype.h>
#include <direct.h>
#include <unistd.h>
```

#### Explanation

This part of code will be used to import the library that used in both `unzip.c` and `categorize.c`

#### A

Download dan unzip file tersebut dalam kode c bernama unzip.c.

#### Answer

```c
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
```

#### Explanation


- The code includes two header files: `<stdio.h>` and `<stdlib.h>`. These are standard C library header files that provide functions for input/output operations and memory allocation, respectively.

- The `main()` function is the entry point of the program, where the execution starts.

- The `command` array is declared with a size of 500 characters, which will be used to store a command to be executed later.

- The `sprintf()` function is used to format and store a command in the `command` array. The command is constructed to download a file using the `wget` command-line utility. It downloads a file from the specified URL and saves it as "hehe.zip" in the current directory.

  - The `system()` function is called with the `command` array as an argument. It executes the command stored in the array. The return value of `system()` is stored in the `status` variable.

- The code checks if the `status` variable is -1, indicating an error in executing the command. If so, it prints "Failed to download."

- Another `command2` array is declared to store a command for extracting the downloaded ZIP file.

- Similar to before, `sprintf()` is used to format and store the extraction command in the `command2` array. It will extract the contents of "hehe.zip" in the current directory.

- The `system()` function is called again with the `command2` array as an argument to extract the ZIP file.

- If the extraction command fails (i.e., `status` is -1), it prints "Failed to extract" and returns 1 to indicate an error.


#### B

Selanjutnya, buatlah program categorize.c untuk mengumpulkan (move / copy) file sesuai extension-nya. Extension yang ingin dikumpulkan terdapat dalam file extensions.txt. Buatlah folder categorized dan di dalamnya setiap extension akan dibuatkan folder dengan nama sesuai nama extension-nya dengan nama folder semua lowercase. Akan tetapi, file bisa saja tidak semua lowercase. File lain dengan extension selain yang terdapat dalam .txt files tersebut akan dimasukkan ke folder other.
Pada file max.txt, terdapat angka yang merupakan isi maksimum dari folder tiap extension kecuali folder other. Sehingga, jika penuh, buatlah folder baru dengan format extension (2), extension (3), dan seterusnya.

#### Answer

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <ctype.h>
#include <unistd.h>

#define MAX_EXTENSION_LENGTH 10
#define MAX_EXTENSIONS 256

int numExtensions = 0;
char extensions[MAX_EXTENSIONS][MAX_EXTENSION_LENGTH];

void create_directory(const char *path) {
    mkdir(path, 0755);
}

void process_file(const char *filename, const char *extension, const char *subdirectory) {
    // Convert extension to lowercase
    char lowercase_extension[MAX_EXTENSION_LENGTH];
    for (int i = 0; i < MAX_EXTENSION_LENGTH - 1 && extension[i] != '\0'; i++) {
        lowercase_extension[i] = tolower(extension[i]);
    }
    lowercase_extension[MAX_EXTENSION_LENGTH - 1] = '\0';

    // Check if the extension is in the list
    int found = 0;
    for (int i = 0; i < numExtensions; i++) {
        if (strcmp(lowercase_extension, extensions[i]) == 0) {
            found = 1;
            break;
        }
    }

    // Move the file to the appropriate directory
    char sourcePath[512];
    if (subdirectory != NULL) {
        snprintf(sourcePath, sizeof(sourcePath), "files/%s/%s", subdirectory, filename);
    } else {
        snprintf(sourcePath, sizeof(sourcePath), "files/%s", filename);
    }

    char destPath[512];
    if (found) {
        snprintf(destPath, sizeof(destPath), "categorized/%s/%s", lowercase_extension, filename);
    } else {
        snprintf(destPath, sizeof(destPath), "categorized/other/%s", filename);
    }

    if (rename(sourcePath, destPath) != 0) {
        printf("Failed to move file: %s\n", filename);
    }
}

void process_directory(const char *dirPath) {
    DIR *dir;
    struct dirent *entry;
    dir = opendir(dirPath);
    if (dir == NULL) {
        printf("Failed to open directory: %s\n", dirPath);
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            char *filename = entry->d_name;

            char *dot = strrchr(filename, '.');
            if (dot != NULL) {
                char extension[MAX_EXTENSION_LENGTH];
                strcpy(extension, dot + 1);
                process_file(filename, extension, strrchr(dirPath, '/') + 1);
            }
        } else if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                char subdirPath[512];
                snprintf(subdirPath, sizeof(subdirPath), "%s/%s", dirPath, entry->d_name);
                process_directory(subdirPath);
            }
        }
    }

    closedir(dir);
}

int main() {
    // Read extensions from file
    FILE *file = fopen("extensions.txt", "r");
    if (file == NULL) {
        printf("Failed to open extensions.txt file.\n");
        return 1;
    }

    // Read extensions and validate the maximum number of extensions
    char extension[MAX_EXTENSION_LENGTH];
    while (fscanf(file, "%s", extension) == 1) {
        if (numExtensions >= MAX_EXTENSIONS) {
            printf("Maximum number of extensions reached.\n");
            break;
        }
        strcpy(extensions[numExtensions], extension);
        numExtensions++;
    }

    fclose(file);

    // Create 'categorized' directory if it doesn't exist
    create_directory("categorized");

    // Create directories for each extension
    for (int i = 0; i < numExtensions; i++) {
        // Convert extension to lowercase
        for (int j = 0; j < MAX_EXTENSION_LENGTH - 1 && extensions[i][j] != '\0'; j++) {
            extensions[i][j] = tolower(extensions[i][j]);
        }

        char path[MAX_EXTENSION_LENGTH + 12];
        snprintf(path, sizeof(path), "categorized/%s", extensions[i]);
        create_directory(path);
    }

    // Create 'other' directory if it doesn't exist
    create_directory("categorized/other");

    // Process files in the 'files' directory and its subdirectories
    process_directory("files");

    return 0;
}
```

#### Explanation

- The code includes necessary header files for input/output operations, string manipulation, directory operations, and platform-specific functions.

- It defines constants for the maximum length of file extensions and the maximum number of extensions to be categorized.

- It declares a global variable `numExtensions` to keep track of the number of extensions, and a 2D character array `extensions` to store the extensions.

- The function `create_directory` is defined to create a directory.

- The function `process_file` is defined to process a file by checking its extension and moving it to the appropriate directory based on the extension.

- The `process_directory` function is defined to recursively process a directory and its subdirectories, iterating over the files and directories within it.

- The `main` function serves as the entry point of the program.

- The program attempts to open the file `extensions.txt` in read mode to read the list of extensions to be categorized.

- It reads the extensions from the file and stores them in the `extensions` array until either the end of the file is reached or the maximum number of extensions is reached.

- The `categorized` directory is created (if it doesn't exist) using the `create_directory` function.

- Directories are created for each extension in the `extensions` array, including a separate directory for `other` files.

- The `process_directory` function is called to start processing `files` in the files directory and its subdirectories.

- For each file encountered, its extension is extracted, and the `process_file` function is called to move the file to the appropriate directory based on the extension.
