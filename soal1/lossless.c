#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <ctype.h>
#include <string.h>

#define MAX_TREE_HT 100


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
 
// A utility function to
// swap two min heap nodes
void swapMinHeapNode(struct MinHeapNode** a,
                     struct MinHeapNode** b)
 
{
 
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}
 
// The standard minHeapify function.
void minHeapify(struct MinHeap* minHeap, int idx)
 
{
 
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
 
    if (left < minHeap->size
        && minHeap->array[left]->freq
               < minHeap->array[smallest]->freq)
        smallest = left;
 
    if (right < minHeap->size
        && minHeap->array[right]->freq
               < minHeap->array[smallest]->freq)
        smallest = right;
 
    if (smallest != idx) {
        swapMinHeapNode(&minHeap->array[smallest],
                        &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}
 
// A utility function to check
// if size of heap is 1 or not
int isSizeOne(struct MinHeap* minHeap)
{
 
    return (minHeap->size == 1);
}
 
// A standard function to extract
// minimum value node from heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap)
 
{
 
    struct MinHeapNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
 
    --minHeap->size;
    minHeapify(minHeap, 0);
 
    return temp;
}
 
// A utility function to insert
// a new node to Min Heap
void insertMinHeap(struct MinHeap* minHeap,
                   struct MinHeapNode* minHeapNode)
 
{
 
    ++minHeap->size;
    int i = minHeap->size - 1;
 
    while (i
           && minHeapNode->freq
                  < minHeap->array[(i - 1) / 2]->freq) {
 
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
 
    minHeap->array[i] = minHeapNode;
}
 
// A standard function to build min heap
void buildMinHeap(struct MinHeap* minHeap)
 
{
 
    int n = minHeap->size - 1;
    int i;
 
    for (i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

 
// Utility function to check if this node is leaf
int isLeaf(struct MinHeapNode* root)
 
{
 
    return !(root->left) && !(root->right);
}
 
// Creates a min heap of capacity
// equal to size and inserts all character of
// data[] in min heap. Initially size of
// min heap is equal to capacity
struct MinHeap* createAndBuildMinHeap(char data[],
                                      int freq[], int size)
 
{
 
    struct MinHeap* minHeap = createMinHeap(size);
 
    for (int i = 0; i < size; ++i)
        minHeap->array[i] = newNode(data[i], freq[i]);
 
    minHeap->size = size;
    buildMinHeap(minHeap);
 
    return minHeap;
}
 
// The main function that builds Huffman tree
struct MinHeapNode* buildHuffmanTree(char data[],
                                     int freq[], int size)
 
{
    struct MinHeapNode *left, *right, *top;
 
    // Step 1: Create a min heap of capacity
    // equal to size.  Initially, there are
    // modes equal to size.
    struct MinHeap* minHeap
        = createAndBuildMinHeap(data, freq, size);
 
    // Iterate while size of heap doesn't become 1
    while (!isSizeOne(minHeap)) {
 
        // Step 2: Extract the two minimum
        // freq alphabetChar from min heap
        left = extractMin(minHeap);
        right = extractMin(minHeap);
 
        // Step 3:  Create a new internal
        // node with frequency equal to the
        // sum of the two nodes frequencies.
        // Make the two extracted node as
        // left and right children of this new node.
        // Add this node to the min heap
        // '$' is a special value for internal nodes, not
        // used
        top = newNode('$', left->freq + right->freq);
 
        top->left = left;
        top->right = right;
 
        insertMinHeap(minHeap, top);
    }
 
    // Step 4: The remaining node is the
    // root node and the tree is complete.
    return extractMin(minHeap);
}

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

int main(int argc, char* argv[]){

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
    } else {
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
    return 0;
}
