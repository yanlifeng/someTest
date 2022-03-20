#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>
#include <set>
#include <hash_set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <random>
#include <sys/time.h>
#define ll long long
using namespace std;
struct node{
    int x;
    ll y;
    bool operator < (node A)const{
        if(y==A.y)return x<A.x;
        return y<A.y;
    }
};
double GetTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (double) tv.tv_sec + (double) tv.tv_usec / 1000000;
}
int n=5000000;
//unordered_set<node>S;
unordered_set<ll>Ss;
int main(){
    vector<node>G;
    for(int i=0;i<n;i++){
        G.push_back({rand()%n,rand()*rand()}); 
    }
    vector<node>Upg;
    for(int i=0;i<n;i++){
        Upg.push_back({rand()%n,rand()*rand()});
    }
    double t0=GetTime();
    /*
    for(int i=0;i<n;i++){
        int x=G[i].x;
        ll y=G[i].y;
        S.insert({x,y});
    }
    for(int i=0;i<n;i++){
       int x=Upg[i].x;
       ll y=Upg[i].y;
       ll y1=G[i].y;
       if(S.find({x,y1})!=S.end()){
           S.erase({x,y1});
           S.insert({x,y});
       }else {
           S.insert({x,y});
       }
    }
    printf("cost1 %lf\n",GetTime()-t0);
    
    */
    t0=GetTime();
    for(int i=0;i<n;i++){
        int x=G[i].x;
        ll y=G[i].y;
        ll z=(y<<20ll)+x;
        Ss.insert(z);
    }
    for(int i=0;i<n;i++){
       int x=Upg[i].x;
       ll y=Upg[i].y;
       ll z=(y<<20ll)+x;
       ll y1=G[i].y;
       ll z1=(y1<<20ll)+x;
       if(Ss.find(z1)!=Ss.end()){
           Ss.erase(z1);
           Ss.insert(z);
       }else {
           Ss.insert(z);
       }
    }
    printf("cost2 %lf\n",GetTime()-t0);
    return 0;
}
