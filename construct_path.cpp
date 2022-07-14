#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
using namespace std;
int main(int argc, char* argv[]){
    if(argc < 4){
        printf("error : para less than 4. eg : ./run file_path base_path out_path\n");
        return 0;
    }
    string file_path = string(argv[1]);
    string base_path = string(argv[2]);
    string out_path = string(argv[3]);
    ifstream iiff;
    iiff.open(file_path);
    ofstream ooff;
    ooff.open(out_path);
    string line;
    while(getline(iiff, line)){
        ooff << base_path + "/" + line <<endl;
    }
    return 0;

}
