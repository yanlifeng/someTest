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

struct LabNum{
    int label;
    int number;
};

struct GlobalLabelInfo{
    int clustId;
    int labelNumber;
};

struct PosNum{
    int startPos;
    int clustSize;
};

struct IdNum{
    int id;
    int number;
};

bool cmpLabNum(LabNum ln1, LabNum ln2){
    return ln1.number > ln2.number;
}

bool cmpIdNum(IdNum in1, IdNum in2){
    return in1.number > in2.number;
}

void printInfo(string pwd, string dependency, string example, vector<string> args, vector<string> descriptions);

int updateLabel(vector< vector<LabNum> > &labNumArr, unordered_map<int, GlobalLabelInfo> &globalMap, int clustId, int &badLabel, vector<int> &resLabelArr);

void calLabelFile(string groundTruth, string clustFile);

int main(int argc , char *argv[]){
    string groundTruth = string(argv[1]);
    string clustFile = string(argv[2]);
    calLabelFile(groundTruth, clustFile);

    return 0;
}

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
    vector<int> ourClust;
    vector<int> standardClust;
    unordered_map<string, int> standardMap;
    unordered_map<int, int> curMap;
    vector<vector<LabNum>> labNumArr;
    vector<PosNum> posArr;
    int startPos = 0;

    int numNotInGroundTruth = 0;
    ifstream ifs1(clustFile);
    if(!ifs1){
        cerr << "error open: " << clustFile << endl;
        exit(1);
    }
    while(getline(ifs1, line)){
        if(line[0] != '\t'){
            if(curMap.size() != 0){
                int clustSize = 0;
                vector<LabNum> curClustInfo;
                for(auto x : curMap){
                    LabNum ln;
                    ln.label = x.first;
                    ln.number = x.second;
                    curClustInfo.push_back(ln);
                    clustSize += x.second;
                }
                std::sort(curClustInfo.begin(), curClustInfo.end(), cmpLabNum);
                labNumArr.push_back(curClustInfo);
                PosNum pn;
                pn.startPos = startPos;
                pn.clustSize = clustSize;
                posArr.push_back(pn);
                startPos += clustSize;
                unordered_map<int, int>().swap(curMap);
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
                standardClust.push_back(curLabel);
                curMap.insert({curLabel, 0});
                curMap[curLabel]++;
            }
        }
    }
    if(curMap.size() != 0){
        int clustSize = 0;
        vector<LabNum> curClustInfo;
        for(auto x : curMap){
            LabNum ln;
            ln.label = x.first;
            ln.number = x.second;
            curClustInfo.push_back(ln);
            clustSize += x.second;
        }
        std::sort(curClustInfo.begin(), curClustInfo.end(), cmpLabNum);
        labNumArr.push_back(curClustInfo);
        PosNum pn;
        pn.startPos = startPos;
        pn.clustSize = clustSize;
        posArr.push_back(pn);
        startPos += clustSize;
        unordered_map<int, int>().swap(curMap);
    }
    printf("numNotInGroundTruth %d\n", numNotInGroundTruth);
    printf("%s has tot %d clusters\n", clustFile.c_str(), labNumArr.size());
    ofstream ooff;
    ooff.open("purity.res");
    for(auto cluster:labNumArr){
        vector<int>mx3;
        int totSize = 0;
        for(auto item:cluster){
            mx3.push_back(item.number);
            totSize += item.number;
        }
        if(cluster.size() == 1)continue;
        if(totSize < 100)continue;
        ooff << "this cluster has " << cluster.size() << " kind elements, " << totSize << " elements" << endl;
        sort(mx3.begin(), mx3.end(), greater<int>());
        for(auto item:mx3){
            ooff << item << "(" << 1.0 * item / totSize << ")  ";
        }
        ooff << endl;
    }
    ooff.close();
}

int updateLabel(vector< vector<LabNum> > &labNumArr, unordered_map<int, GlobalLabelInfo> &globalMap, int clustId, int &badLabel, vector<int> &resLabelArr)//return the new badLabel
{
    bool isBad = true;
    while(labNumArr[clustId].size() != 0 && isBad)
    {
        int curLabel = labNumArr[clustId][0].label;
        int curNumber = labNumArr[clustId][0].number;
        if(globalMap.find(curLabel) == globalMap.end())//new label 
        {
            GlobalLabelInfo glab;
            glab.clustId = clustId;
            glab.labelNumber = curNumber;
            globalMap.insert({curLabel, glab});
            resLabelArr[clustId] = curLabel;
            isBad = false;
        }
        else//label collison with previous cluster
        {
            int preClustId = globalMap[curLabel].clustId;
            int preNumber = globalMap[curLabel].labelNumber;
            if(curNumber > preNumber)//the previous cluster is defeated, need to update the previous cluster.
            {
                resLabelArr[clustId] = curLabel;
                isBad = false;
                globalMap[curLabel].clustId = clustId;
                globalMap[curLabel].labelNumber = curNumber;
                badLabel = updateLabel(labNumArr, globalMap, preClustId, badLabel, resLabelArr);
            }
            else//current cluster can not defeat the previous cluster, just erase the biggest label to try new biggest label
            {}
        }

        labNumArr[clustId].erase(labNumArr[clustId].begin());//erase the biggest label in this cluster
    }//end while
    if(isBad)
    {
        resLabelArr[clustId] = badLabel;
        badLabel--;//update the newBadLabel
    }
    return badLabel;
}


void printInfo(string pwd, string dependency, string example, vector<string> args, vector<string> descriptions){
    assert(args.size() == descriptions.size());
    cerr << endl;
    cerr << "example: " << example << endl;
    cerr << endl;
    cerr << "source file path: " << pwd << endl;
    cerr << endl;
    cerr << "dependency: " << dependency << endl;
    cerr << endl;
    cerr << "run as: ";
    for(int i = 0; i < args.size(); i++){
        cerr << args[i] << ' ';
    }
    cerr << endl;
    for(int i = 0; i < args.size(); i++){
        fprintf(stderr, "\tThe %d paramter(%s) is %s\n", i, args[i].c_str(), descriptions[i].c_str());
    }
}
