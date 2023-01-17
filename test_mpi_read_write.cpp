#include <cstdio>
#include <iostream>
#include <sys/time.h>
#include <thread>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "mpi.h"
using namespace std;
#define N (1 * (1 << 22))
int comm_size = 0;
int my_rank = -1;
inline double GetTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (double) tv.tv_sec + (double) tv.tv_usec / 1000000;
}
void f1(int my) {
    char *a = new char[N * 100];
    int size = sizeof(char);
    FILE *fp;
    string in_file = "testr";
    in_file += to_string(0);
    int base = my * 65 * N;
    if((fp = fopen(in_file.c_str(), "r")) == NULL ){  
        puts("Fail to open file!");
        exit(0);
    }
    fseek(fp, base, SEEK_SET);
    long long res = 0;
    double tt = 0;
    for(int i = 0; i < 65; i++){
        double t0 = GetTime();
        res += fread(a + N * i, size, N, fp);
        tt += GetTime() - t0;
    }
    printf("read %d %lld\n", my, res / N);
    printf("read %d %.4f\n", my, tt);
    fclose(fp);
}
void f2(int my) {
    char *a = new char[N * 100];
    for(int i = 0; i < N * 65; i++) {
        a[i] = 'a' + my;
    }
    int size = sizeof(char);
    FILE *fp;
    string out_file = "testw";
    out_file += to_string(0);
    int base = my * 65 * N;
    if(my == 0) {
        int fd = open(out_file.c_str(), O_CREAT | O_TRUNC | O_RDWR | O_EXCL, 0644);
        ftruncate(fd, size * 6 * 65 * N);
        fp = fdopen(fd, "w");
    } else {
        int found = 0;
        do {
            if(-1 == access(out_file.c_str(), F_OK)) {
                if(ENOENT == errno) {
                    printf("waiting file be created...\n");
                    usleep(100);
                } else {
                    printf("file open GG");
                    exit(0);
                }
            } else {
                fp = fopen(out_file.c_str(), "r+b");   
                found = 1;
            }
        } while(found == 0);
    }
    //if((fp = fopen(out_file.c_str(), "w")) == NULL ){  
    //    puts("Fail to open file!");
    //    exit(0);
    //}
    long long res = 0;
    double tt = 0;
    for(int i = 0; i < 65; i++){
        double t0 = GetTime();
        fseek(fp, base + i * N, SEEK_SET);
        int now_res = fwrite(a + N * i, size, N, fp);
        res += now_res;
        if(now_res != N) {
            printf("GG %d %d %c\n", my, now_res, *(a + N * i));
        }
        tt += GetTime() - t0;
    }
    printf("write %d %lld\n", my, res / N);
    printf("write %d %.4f\n", my, tt);
    fclose(fp);
}
int main(int argc, char *argv[]){
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    double total_time_start =  MPI_Wtime();
    //thread t1(f1, my_rank);
    thread t2(f2, my_rank);
    //t1.join();
    t2.join();
    double total_time_end =  MPI_Wtime();
    //printf("Time: total time cost: %.4lfs\n", total_time_end - total_time_start);

    MPI_Finalize();
    return 0;
}
