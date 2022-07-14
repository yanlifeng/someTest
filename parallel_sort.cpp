#include <cstdio>
#include <vector>
#include <cstring>
#include <iostream>
#include <omp.h>
#include <unistd.h>
using namespace std;
int main(int argc, char* argv[]){
    vector<string>files;
    for(int i = 1; i < argc; i++){
        files.push_back(string(argv[i]));
    }
    int N = files.size();
    string cmd = "fastq-sort -S 250000M --temporary-directory=/home/large/ylf_data/tmp_sort -i ";
#pragma omp parallel for num_threads(2)
    for(int i = 0; i < N; i++){
        string now_cmd = cmd + files[i] + " > " + files[i] + ".sorted";
        printf("%s\n", now_cmd.c_str());
        system(now_cmd.c_str());
        sleep(10);
        string md5_cmd = "md5sum " + files[i] + ".sorted";
        system(md5_cmd.c_str());
    }
    return 0;
}
