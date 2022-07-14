#include <iostream>
#include <stdlib.h>
#include <string>
#include <cassert>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstdio>
#include <algorithm>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <sys/sysinfo.h>
#include <omp.h>


using namespace std;

void calLabelFile(string groundTruth, string clustFile, string treeFile){
    //--------for groundTruth--------------
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

    //--------for cluster file--------------------------
    vector<pair<string,int>>curCluster;
    vector<vector<pair<string,int>>> clusters;

    int numNotInGroundTruth = 0;
    ifstream ifs1(clustFile);
    if(!ifs1){
        cerr << "error open: " << clustFile << endl;
        exit(1);
    }
    while(getline(ifs1, line)){
        if(line[0] != '\t'){
            if(curCluster.size() != 0){
                vector<pair<string,int>> tmpCluster;
                for(auto item : curCluster){
                    tmpCluster.push_back(item);
                }
                clusters.push_back(tmpCluster);
                curCluster.clear();
            }
        }
        else{
            stringstream ss;
            ss << line;
            int curId, genomeId;
            string genomeSize, fileName, genomeName;
            ss >> curId >> genomeId >> genomeSize >> fileName >> genomeName;
            int startIndex = fileName.find_last_of('/');
            int endIndex = fileName.find_first_of('_', startIndex+5);
            if(endIndex == -1){
                endIndex = fileName.find('.', startIndex+5);
                printf("GG fileName\n");
            }	
            string key = fileName.substr(startIndex+1, endIndex-startIndex-1);
            if(accession_taxid_map.find(key) == accession_taxid_map.end()){
                numNotInGroundTruth++;
                continue;
            }
            else{
                int curLabel = accession_taxid_map[key];
                curCluster.push_back(make_pair(key,curLabel));
            }
        }
    }
    if(curCluster.size() != 0){
        vector<pair<string,int>> tmpCluster;
        for(auto item : curCluster){
            tmpCluster.push_back(item);
        }
        clusters.push_back(tmpCluster);
    }
    ifs1.close();



    unordered_map<int,int>fid_mp;
    unordered_map<int,string>level_mp;

    ifstream iiff;
    iiff.open(treeFile);
    while(getline(iiff,line)){
        int gid,fid;
        string level="";
        string ttmp;
        stringstream ss;
        ss << line;
        ss >> gid >> ttmp >> fid >> ttmp;
        while(1){
            ss >> ttmp;
            if(ttmp == "|")break;
            level += ttmp;
        }
        fid_mp[gid] = fid;
        level_mp[gid] = level;
    }
    iiff.close();

    printf("numNotInGroundTruth %d\n", numNotInGroundTruth);
    printf("%s has tot %d clusters\n", clustFile.c_str(), clusters.size());
    ofstream ooff;
    ooff.open("purity.res");
    ofstream ooff2;
    ooff2.open("list.res");
    for(auto cluster:clusters){
        int nnn = cluster.size();
        for(int i = 0; i < nnn; i++){
            string ass = cluster[i].first;
            int label = cluster[i].second;
            for(int j = i + 1; j < nnn; j++){
                string ass2 = cluster[j].first;
                int label2 = cluster[j].second;
                if(label != label2){
                    ooff2 << ass << " " << label << " " << ass2 << " " << label2 << endl;
                    vector<int>fids;
                    vector<int>fids2;
                    int now_id = label;
                    for(int k = 0; k < 3; k++){
                        now_id = fid_mp[now_id];
                        fids.push_back(now_id);
                    }
                    now_id = label2;
                    for(int k = 0; k < 3; k++){
                        now_id = fid_mp[now_id];
                        fids2.push_back(now_id);
                    }

                    ooff << ass << " " << label << " ";
                    for(auto item : fids)ooff << item << "," << level_mp[item] << " ";
                    ooff << endl;

                    ooff << ass2 << " " << label2 << " ";
                    for(auto item : fids2)ooff << item << "," << level_mp[item] << " ";
                    ooff << endl;
                    ooff << "============================" <<endl;
                }
            }
        } 
    }
    ooff.close();
    ooff2.close();
}

int main(int argc , char *argv[]){
    string groundTruth = string(argv[1]);
    string clustFile = string(argv[2]);
    string treeFile = string(argv[3]);
    calLabelFile(groundTruth, clustFile, treeFile);

    return 0;
}

