#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <unordered_map>
#include <set>
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
int main(){
    ifstream iff;
    iff.open("ANI_report_prokaryotes.txt");
    string line;
    getline(iff, line);
    unordered_map<string, string>mp_gcf2gca;
    while(getline(iff, line)){
        auto res = split(line, "\t");
        string gca_name = res[0];
        string gcf_name = res[1];
        mp_gcf2gca[gcf_name] = gca_name;
    }
    iff.close();
    ofstream ooff;
    ooff.open("million_seqName_accession.map");
    ifstream iiff;
    iiff.open("bacteria_seqName_accession.map");
    string nz_name, gc_name, cnt;
    set<string>gcs;
    while(iiff >> nz_name >> gc_name >> cnt){
        if(mp_gcf2gca.count(gc_name) == 0){
            printf("no %s in mp\n", gc_name.c_str());
            gcs.insert(gc_name);
        }
        else ooff << nz_name << " " << mp_gcf2gca[gc_name] << " " << cnt <<endl;
    }
    for(auto item : gcs){
        printf("%s\n", item.c_str());
    }
    printf("cnt2 is %d\n", gcs.size());
    iiff.close();
    ooff.close();
    return 0;
}
