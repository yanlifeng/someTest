#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sys/time.h>
#include <vector>
#include <execution>

#include <pstl/execution>
#include <pstl/algorithm>

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
    string file_name = string(argv[1]);
    ifstream iiff;
    iiff.open(file_name);
    vector<node>lst;
    ofstream off;
    off.open("process.log");
    string name, seqs, srand, qual;
    while(getline(iiff, name)){
        getline(iiff, seqs);
        getline(iiff, srand);
        getline(iiff, qual);
        lst.push_back({name, seqs, srand, qual});
        if(lst.size() % 100000 == 0)off << "read " << lst.size() << endl;
    }
    unordered_map<string, int>
    vector<pair<int64_t, int>>ids;
    for(int i = 0; i < lst.size(); i++){
        ids.push_back(make_pair());
    }
    printf("read cost %f\n", GetTime() - t0);
    t0 = GetTime();
    sort(execution::par, lst.begin(), lst.end());
    //parasort(lst.size(), lstt, 8);
    printf("sort done\n");
    printf("sort cost %f\n", GetTime() - t0);
    t0 = GetTime();
    ofstream ooff;
    ooff.open(file_name + ".sorted");
    int cnt = 0;
    for(int i = 0; i < lst.size(); i++){
        auto item = lstt[i];
        ooff << item.name << endl << item.seqs << endl << item.srand << endl << item.qual << endl;
        cnt++;
        if(cnt % 100000 == 0)off << "write " << cnt << endl;
    }
    off.close();
    ooff.close();
    iiff.close();
    printf("write cost %f\n", GetTime() - t0);
    return 0;


}
