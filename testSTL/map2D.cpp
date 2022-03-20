#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <random>
#include <sys/time.h>
#define ll long long
using namespace std;
//map<int,map<int,ll>>mp;
//map<ll,ll>mpp;
unordered_map<int,unordered_map<int,ll>>mp;
unordered_map<ll,ll>mpp;
struct node{
    int x,y,z;
};
double GetTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (double) tv.tv_sec + (double) tv.tv_usec / 1000000;
}
int n=5000000;
int main(){
    vector<node>G;
    for(int i=0;i<n;i++){
        G.push_back({rand()%n,rand()%n,rand()*rand()}); 
    }
    double t0=GetTime();
    int px,py;
    for(int i=0;i<n;i++){
        int x=G[i].x;
        int y=G[i].y;
        int z=G[i].z;
        if(mp[x].count(y)){
            mp[x][y]+=z;
        }else{
            mp[x][y]=z;
        }
        px=x;
        py=y;
    }
    printf("ans %d\n",mp[px][py]);
    printf("cost1 %lf\n",GetTime()-t0);
    ll pu;
    t0=GetTime();
    for(int i=0;i<n;i++){
        int x=G[i].x;
        int y=G[i].y;
        int z=G[i].z;
        ll u=(x<<20ll)+y;
        if(mpp.count(u)){
            mpp[u]+=z;
        }else{
            mpp[u]=z;
        }
        pu=u;
    }
    printf("ans %d\n",mpp[pu]);
    printf("cost1 %lf\n",GetTime()-t0);
    return 0;
}
