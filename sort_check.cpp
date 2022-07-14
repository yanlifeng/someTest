#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sys/time.h>
#include <vector>
#include <cassert>

#include "/home/user_home/ylf/someGit/parallel-sort/parasort.h" // parasort
using namespace std;
struct node{
    string name;
    string seqs;
    string srand;
    string qual;
    bool operator < (const node A) const{
        return name < A.name;
    }
};
int cmp(node A, node B){
    return A.name < B.name;
}

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
    vector<node>lst1;
    ifstream iiff2;
    iiff2.open(file_name2);
    vector<node>lst2;

    ofstream off;
    off.open("process3.log");
    string name1, seqs1, srand1, qual1;
    while(getline(iiff1, name1)){
        getline(iiff1, seqs1);
        getline(iiff1, srand1);
        getline(iiff1, qual1);
        lst1.push_back({name1, seqs1, srand1, qual1});
        if(lst1.size() % 100000 == 0)off << "read1 " << lst1.size() << endl;
    }
    printf("read1 cost %f\n", GetTime() - t0);
    t0 = GetTime();
    parasort(lst1.size(), &(lst1[0]), 8);
    //sort(lst1.begin(), lst1.end());
    printf("sort1 done\n");
    printf("sort1 cost %f\n", GetTime() - t0);


    
    string name2, seqs2, srand2, qual2;
    while(getline(iiff2, name2)){
        getline(iiff2, seqs2);
        getline(iiff2, srand2);
        getline(iiff2, qual2);
        lst2.push_back({name2, seqs2, srand2, qual2});
        if(lst2.size() % 100000 == 0)off << "read2 " << lst2.size() << endl;
    }
    printf("read2 cost %f\n", GetTime() - t0);
    t0 = GetTime();
    sort(lst2.begin(), lst2.end());
    printf("sort2 done\n");
    printf("sort2 cost %f\n", GetTime() - t0);


    t0 = GetTime();
    assert(lst1.size() == lst2.size());
    for(int i = 0; i < lst1.size(); i++){
        assert(lst1[i].name == lst2[i].name);
        assert(lst1[i].seqs == lst2[i].seqs);
        assert(lst1[i].srand == lst2[i].srand);
        assert(lst1[i].qual == lst2[i].qual);
    }
    iiff1.close();
    iiff2.close();
    off.close();
    printf("check cost %f\n", GetTime() - t0);
    return 0;


}
