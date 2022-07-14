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

void calLabelFile(string groundTruth, string clustFile){
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
    printf("numNotInGroundTruth %d\n", numNotInGroundTruth);
    printf("%s has tot %d clusters\n", clustFile.c_str(), clusters.size());
    ofstream ooff;
    ooff.open("purity.res");
    for(auto cluster:clusters){
        map<int,vector<string>>mp;
        for(auto item : cluster){
            string ass = item.first;
            int label = item.second;
            if(mp.count(label) == 0){
                vector<string>tmp;
                mp[label] = tmp;
            }
            mp[label].push_back(ass);
        }
        int totSize = cluster.size();
        if(mp.size() <= 1)continue;
        ooff << "this cluster has " << totSize << " elements, total " << mp.size() << " kinds" <<endl;
        for(auto item : mp){
            vector<string> names = item.second;
            ooff << names.size() <<  " (" << 1.0 * names.size() / totSize << ") :";
        }
        ooff << endl << "good ones : " << endl;
        for(auto item : mp){
            int label = item.first;
            vector<string> names = item.second;
            //if(names.size() >= totSize * 0.6){
            ooff << names.size() <<  " (" << 1.0 * names.size() / totSize << ") ";
            ooff << names[0] << " : " << accession_organismName_map[names[0]] << " : " << accession_taxid_map[names[0]];
            ooff << endl;
            //}
        }
        ooff << endl << "bad ones : " << endl;
        for(auto item : mp){
            int label = item.first;
            vector<string> names = item.second;
            if(names.size() <= totSize * 0.001){
                ooff << names.size() <<  " (" << 1.0 * names.size() / totSize << ") ";
                for(auto name:names)ooff << name << " : " << accession_organismName_map[name] << " : " << accession_taxid_map[names[0]] << "   |    |    ";
                ooff << endl;
            }
        }
        ooff << "==========================" << endl;
    }
    ooff.close();
}

int main(int argc , char *argv[]){
    string groundTruth = string(argv[1]);
    string clustFile = string(argv[2]);
    calLabelFile(groundTruth, clustFile);

    return 0;
}

