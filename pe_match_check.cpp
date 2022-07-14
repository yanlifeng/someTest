#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sys/time.h>
#include <vector>
#include <cassert>
using namespace std;
struct node{
    string name;
    string seqs;
    string srand;
    string qual;
};

inline double GetTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (double) tv.tv_sec + (double) tv.tv_usec / 1000000;
}


int main(int argc, char* argv[]){
    double t0 = GetTime();
    string file_name1 = string(argv[1]);
    string file_name2 = string(argv[2]);
    ifstream iiff1;
    iiff1.open(file_name1);
    ifstream iiff2;
    iiff2.open(file_name2);
    string name1, name2;
    string seqs1, seqs2;
    string srand1, srand2;
    string qual1, qual2;
    ofstream off;
    off.open("process2.log");
    int cnt = 0;
    while(getline(iiff1, name1)){
        getline(iiff1, seqs1);
        getline(iiff1, srand1);
        getline(iiff1, qual1);

        getline(iiff2, name2);
        getline(iiff2, seqs2);
        getline(iiff2, srand2);
        getline(iiff2, qual2);

        name1 = name1.substr(0, name1.find("length"));
        name2 = name2.substr(0, name2.find("length"));
        srand1 = srand1.substr(0, srand1.find("length"));
        srand2 = srand2.substr(0, srand2.find("length"));
        assert(name1 == name2);
        assert(srand1 == srand2);
        //assert(seqs1.length() == seqs2.length());
        //assert(qual1.length() == qual2.length());
        cnt++;

        if(cnt % 100000 == 0)off << "write " << cnt << endl;
    }
    iiff1.close();
    iiff2.close();
    off.close();
    return 0;


}
