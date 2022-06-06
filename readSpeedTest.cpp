#include<cstdio>
#include <iostream>
#include <sys/time.h>
using namespace std;
#define N (4*(1<<20))

inline double GetTime() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (double) tv.tv_sec + (double) tv.tv_usec / 1000000;
}
int main(){
	char *a=new char[N];
	int size = sizeof(char);
	FILE *fp;
	if( (fp=fopen("testw", "r")) == NULL ){  
		puts("Fail to open file!");
		exit(0);
	}
	long long res=0;
	double tt=0;
	for(int i=0;i<2500;i++){
		double t0=GetTime();
		res+=fread(a, size, N, fp);
		tt+=GetTime()-t0;
	}
	printf("%lld\n",res/N);
	printf("%.4f\n",tt);
	fclose(fp);
	return 0;
}
