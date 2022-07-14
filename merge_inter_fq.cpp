#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
using namespace std;
int main(int argc, char* argv[]){
    string in1 = string(argv[1]);
    string in2 = string(argv[2]);
    string out = string(argv[3]);
    ifstream iiff1;
    ifstream iiff2;
    ofstream ooff;
    iiff1.open(in1);
    iiff2.open(in2);
    ooff.open(out);
    string name1, seqs1, srand1, qua1;
    string name2, seqs2, srand2, qua2;
    int cnt = 0;
    while(getline(iiff1, name1)){
        cnt++;
        if(cnt % 100000 == 0)printf("cnt %d\n", cnt);
        getline(iiff1, seqs1);
        getline(iiff1, srand1);
        getline(iiff1, qua1);

        getline(iiff2, name2);
        getline(iiff2, seqs2);
        getline(iiff2, srand2);
        getline(iiff2, qua2);

        ooff << name1 << endl << seqs1 << endl << srand1 << endl << qua1 << endl;
        ooff << name2 << endl << seqs2 << endl << srand2 << endl << qua2 << endl;
    }
    iiff1.close();
    iiff2.close();
    ooff.close();
    return 0;
}
