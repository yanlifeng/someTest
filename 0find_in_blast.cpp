#include <cstdio>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include <vector>
using namespace std;
int main(int argc, char* argv[]){
    if(argc < 4){
        printf("Para error, at least 3 paras, which means badList genomePath dbPath\n");
        return 0;
    }
    string badList = string(argv[1]);
    string genomePath = string(argv[2]);
    string dbPath = string(argv[3]);
    printf("bad list is %s\n", badList.c_str());
    printf("genome path is %s\n", genomePath.c_str());
    printf("blast database path is %s\n", dbPath.c_str());
    ifstream iff;
    iff.open(genomePath);
    vector<string>tot_paths;
    string path;
    while(iff >> path){
        tot_paths.push_back(path);
    }
    iff.close();
    ifstream iiff;
    iiff.open(badList);
    string name1, name2;
    int label1, label2;
    while(iiff >> name1 >> label1 >> name2 >> label2){
        string res_path = name1 + "_" + name2 + ".res";
        int is_done = 0;
        if(access(res_path.c_str(), 0) == -1){
            string cmd1 = "mkdir " + res_path;
            system(cmd1.c_str());
        }else{
            string done_path = res_path + "/done";
            ifstream fin(done_path);
            if(!fin){
                is_done = 0;
            }else{
                is_done = 1;
            }
        }
        if(is_done){
            printf("%s is done, continue\n", res_path.c_str());
            continue;
        }
        string gemo_path1;
        int find_cnt1 = 0;
        for(auto item : tot_paths){
            if(item.find(name1) != string::npos){
                gemo_path1 = item;
                find_cnt1++;
            }
        }
        if(find_cnt1 > 1)printf("GG %s match %d path\n", name1.c_str(), find_cnt1);
        string gemo_path2;
        int find_cnt2 = 0;
        for(auto item : tot_paths){
            if(item.find(name2) != string::npos){
                gemo_path2 = item;
                find_cnt2++;
            }
        }
        if(find_cnt2 > 1)printf("GG %s match %d path\n", name2.c_str(), find_cnt2);


        string cmd2 = "time blastn -num_threads 48 -query "
            + gemo_path1 + " -db " + dbPath + " -out " + res_path + "/"
            + name1 + ".blast.res" + " -max_target_seqs 5";
        printf("%s\n", cmd2.c_str());
        system(cmd2.c_str());


        cmd2 = "time blastn -num_threads 48 -query "
            + gemo_path2 + " -db " + dbPath + " -out " + res_path + "/"
            + name2 + ".blast.res" + " -max_target_seqs 5";
        printf("%s\n", cmd2.c_str());
        system(cmd2.c_str());

        string cmd3 = "cp done " + res_path + "/";
        printf("%s\n", cmd3.c_str());
        system(cmd3.c_str());
        printf("============================\n\n");
    }
    return 0;

}
