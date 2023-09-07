#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <pthread.h>
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

    // Detach shared memory
    shmdt(hasil);

    return 0;
}
