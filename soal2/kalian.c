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
