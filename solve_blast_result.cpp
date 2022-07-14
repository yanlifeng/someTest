#include <cstdio>
#include <fstream>
#include <iostream>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <unistd.h>
#include <unordered_map>
#include <vector>
#include <cassert>
using namespace std;


vector<string> split(const string& str, const string& delim) {
    vector<string> res;
    if("" == str) return res;
    //先将要切割的字符串从string类型转换为char*类型
    char * strs = new char[str.length() + 1] ; //不要忘了
    strcpy(strs, str.c_str()); 

    char * d = new char[delim.length() + 1];
    strcpy(d, delim.c_str());

    char *p = strtok(strs, d);
    while(p) {
        string s = p; //分割得到的字符串转换为string类型
        res.push_back(s); //存入结果数组
        p = strtok(NULL, d);
    }

    return res;
}
int main(int argc, char* argv[]){
    if(argc < 4){
        printf("error : para less than 3. eg : ./run badList nz2tidFile groundTruth\n");
        return 0;
    }
    string badList = string(argv[1]);
    string nz2tidFile = string(argv[2]);
    string groundTruth = string(argv[3]);
    
    ifstream ifs0(groundTruth);
    if(!ifs0){
        cerr << "error open: " << groundTruth << endl;
        exit(1);
    }
    unordered_map<string, int> accession_taxid_map;
    unordered_map<string, string> accession_organismName_map;
    string line;
    getline(ifs0, line);//for the header line
    while(getline(ifs0, line)){
        stringstream ss;
        string accession, organismName(""), tmpStr;
        int taxid;
        ss << line;
        ss >> accession >> taxid;
        while(ss >> tmpStr){
            organismName += tmpStr + ' ';
        }
        organismName.substr(0, organismName.length()-1);
        accession_taxid_map.insert({accession, taxid});
        accession_organismName_map.insert({accession, organismName});
    }
    ifs0.close();


    ifstream iff;
    iff.open(nz2tidFile);
    string nz_name, gc_name;
    int cnt;
    unordered_map<string, int>mp_nz2tid;

    while(iff >> nz_name >> gc_name >> cnt){
        nz_name = nz_name.substr(1, nz_name.length() - 1);
        if(mp_nz2tid.count(nz_name)){
            printf("%s GG duplicate nz name %s\n", nz_name.c_str());
            return 0;
        }else{
            if(accession_taxid_map.count(gc_name) == 0){
                printf("GG mp has no gc name %s\n", gc_name.c_str());
                return 0;
            }
            mp_nz2tid[nz_name] = accession_taxid_map[gc_name];
        }
    }
    printf("read nz2tid file done\n");
    ifstream iiff;
    ofstream ooff;
    iiff.open(badList);
    ooff.open("tot.analyze.res");
    string name1, name2;
    int label1, label2;
    while(iiff >> name1 >> label1 >> name2 >> label2){
        string res_path = name1 + "_" + name2 + ".res";
        if(access(res_path.c_str(), 0) == -1){
            printf("no file %s\n", res_path.c_str());
            return 0;
        }
        string done_path = res_path + "/done";
        ifstream fin(done_path);
        if(!fin){
            printf("%s blast search is't done\n", res_path.c_str());
            return 0;
        }


        //if(mp_gc2cnt.count(name1) == 0 || mp_gc2cnt.count(name2) == 0){
        //    printf("%s %s not in nz2gcf file\n", name1.c_str(), name2.c_str());
        //    continue;
        //}

        ofstream ooff2;
        ifstream iiff2;

        string in_tmp = res_path + "/" + name1 + ".blast.res";
        string out_tmp = res_path + "/" + name1 + ".analyze.res";

        ooff2.open(out_tmp);
        iiff2.open(in_tmp);


        printf("now process %s\n", in_tmp.c_str());
        string line;
        string now_query;
        unordered_map<int,int>mp_tids;
        int seqs_cnt = 0;
        while(getline(iiff2, line)){
            if(line.find("Query=") != string::npos){
                now_query = split(line, " ")[1];
                seqs_cnt++;
            }
            if(line.find("Sequences producing significant alignments:") == string::npos)
                continue;
            getline(iiff2, line);
            assert(line.length() == 0);
            unordered_map<int,double>mp_tmp;
            for(int i = 0; i < 5; i++){
                getline(iiff2, line);
                if(line.length() == 0)break;
                auto res = split(line, " ");
                string now_nz = res[0];
                if(mp_nz2tid.count(now_nz) == 0){
                    printf("mp_nz2tid has no nz %s\n", now_nz.c_str());
                    continue;
                }
                int tid = mp_nz2tid[now_nz];
                double scor = stod(res[res.size() - 2]);
                double e_scor = stod(res[res.size() - 1]);
                if(mp_tmp.count(tid)){
                    mp_tmp[tid] += scor;
                }else {
                    mp_tmp[tid] = scor;
                }
            }
            vector<pair<double, int>>vec_tmp;
            for(auto item : mp_tmp){
                vec_tmp.push_back(make_pair(-item.second, item.first));
            }
            sort(vec_tmp.begin(), vec_tmp.end());
            if(vec_tmp.size()){
                int good_tid = vec_tmp[0].second;
                ooff2 << now_query << " : best match tid in blast is " << good_tid << endl;
                if(mp_tids.count(good_tid)){
                    mp_tids[good_tid]++;
                }else{
                    mp_tids[good_tid]=1;
                }
            }
        }
        ooff2.close();
        iiff2.close();
        vector<pair<int, int>>vec_tids;
        for(auto item :mp_tids){
            vec_tids.push_back(make_pair(-item.second, item.first));
        }
        sort(vec_tids.begin(), vec_tids.end());
        if(vec_tids.size())ooff << name1 << "'s tid origin is " << label1 << ", now seems to be " << vec_tids[0].second << " because " << -vec_tids[0].first << "/" << seqs_cnt << " seqs in blast is " << vec_tids[0].second << endl;

        printf("process %s done\n", in_tmp.c_str());
        if(vec_tids.size())
            printf("%s best tid is %d\n", name1.c_str(), vec_tids[0].second);





        
        in_tmp = res_path + "/" + name2 + ".blast.res";
        out_tmp = res_path + "/" + name2 + ".analyze.res";
        ooff2.open(out_tmp);
        iiff2.open(in_tmp);

        printf("now process %s\n", in_tmp.c_str());
        mp_tids.clear();
        seqs_cnt = 0;
        while(getline(iiff2, line)){
            if(line.find("Query=") != string::npos){
                now_query = split(line, " ")[1];
                seqs_cnt++;
            }
            if(line.find("Sequences producing significant alignments:") == string::npos)
                continue;
            getline(iiff2, line);
            assert(line.length() == 0);
            unordered_map<int,double>mp_tmp;
            for(int i = 0; i < 5; i++){
                getline(iiff2, line);
                if(line.length() == 0)break;
                auto res = split(line, " ");
                string now_nz = res[0];
                if(mp_nz2tid.count(now_nz) == 0){
                    printf("mp_nz2tid has no nz %s\n", now_nz.c_str());
                    continue;
                }
                int tid = mp_nz2tid[now_nz];
                double scor = stod(res[res.size() - 2]);
                double e_scor = stod(res[res.size() - 1]);
                if(mp_tmp.count(tid)){
                    mp_tmp[tid] += scor;
                }else {
                    mp_tmp[tid] = scor;
                }
            }
            vector<pair<double, int>>vec_tmp;
            for(auto item : mp_tmp){
                vec_tmp.push_back(make_pair(-item.second, item.first));
            }
            sort(vec_tmp.begin(), vec_tmp.end());
            if(vec_tmp.size()){
                int good_tid = vec_tmp[0].second;
                ooff2 << now_query << " : best match tid in blast is " << good_tid << endl;
                if(mp_tids.count(good_tid)){
                    mp_tids[good_tid]++;
                }else{
                    mp_tids[good_tid]=1;
                }
            }
        }
        ooff2.close();
        iiff2.close();
        vec_tids.clear();
        for(auto item :mp_tids){
            vec_tids.push_back(make_pair(-item.second, item.first));
        }
        sort(vec_tids.begin(), vec_tids.end());
        if(vec_tids.size())ooff << name2 << "'s tid origin is " << label2 << ", now seems to be " << vec_tids[0].second << " because " << -vec_tids[0].first << "/" << seqs_cnt << " seqs in blast is " << vec_tids[0].second << endl;
        ooff << "===================" << endl;
        printf("process %s done\n", in_tmp.c_str());
        if(vec_tids.size())
            printf("%s best tid is %d\n", name2.c_str(), vec_tids[0].second);





    }
    iiff.close();
    return 0;
}
