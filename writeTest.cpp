#include <aio.h>
#include <stdio.h>

#include <iostream>
#include <cstdio>
#include <fstream>
#include <sys/time.h>
#include <random>
#include <thread>
#include <atomic>
#include "src/concurrentqueue.h"
#include <unistd.h> /* for syscall() */
#define ll long long
using namespace std;

double GetTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (double) tv.tv_sec + (double) tv.tv_usec / 1000000;
}
moodycamel::ConcurrentQueue<pair<char*,ll>> Q(128);
atomic_int producerDone;
atomic_llong offest;
fstream out_stream;
void ProducerTask(){
    double cost1=0;
    double now=GetTime();
    FILE *inFile=NULL;
    inFile=fopen("/home/sssd/ylf_data/SRR7963242_1.fastq","rb");
    ll N=200ll*1024*1024*1024;
    ll M=1024ll*1024*1024;
    while(N>0){
        M=min(M,N);
        char *data=new char[M];
        double noww=GetTime();
        ll res=fread(data,1,M,inFile);
        cost1+=GetTime()-noww;
        while(Q.try_enqueue({data,res})==0){
            usleep(100);
        }
        if(res!=M)break;
        N-=res;
    }
    producerDone=1;
    printf("producer cost %.3f\n",GetTime()-now);
    printf("producer cost1 %.3f\n",cost1);
    return;
}
struct aiocb createIoRequest(int fd,  
        off_t offset, 
        volatile void * content, 
        size_t length){
    // create and initialize the aiocb structure.
    // If we don't init to 0, we have undefined behavior.
    // E.g. through sigevent op.aio_sigevent there could be 
    //      a callback function being set, that the program
    //      tries to call - which will then fail.
    struct aiocb ret = {0};
    {
        ret.aio_fildes = fd;
        ret.aio_offset = offset;
        ret.aio_buf = content;
        ret.aio_nbytes = length;            
    }
    return ret;
}



void ComsumerTask(int id){

    string filename="/home/sssd/ylf_data/test.fq";
    FILE * file = fopen(filename.c_str(),"w");
    int fd = fileno(file);
    printf("now thread id is %d\n",id);
    double st=GetTime();
    double cost1=0;
    pair<char*,ll >now;
    bool over=0;
    const struct aiocb * aiolist[10010];
    int now_id=0;
    while(1){
        while(Q.try_dequeue(now)==0){
            if(producerDone){
                over=1;
                break;
            }
            usleep(100);
        }
        if(over)break;
        char *data=now.first;
        ll M=now.second;
        double noww=GetTime();
        struct aiocb op  = createIoRequest(fd,offest,data,M);
        offest+=M;
        int ret = aio_write(&op);

        printf("aio_write 1: %d\n",ret);
        aiolist[now_id++]=&op;
        /*

        // wait until everything is done
        {
        const struct aiocb * aiolist[1];
        aiolist[0] = &op;

        int ret = aio_suspend(aiolist,1,NULL);
        printf("aio_suspend: %d\n",ret);

        // report possible errors
        {
        ret = aio_error(&op);
        printf("errno 1: %d\n",ret);
        }
        }
        */

        //aioWrite(fd,now.first,now.second);
        //out_stream.write(now.first,now.second);
        cost1+=GetTime()-noww;
        //delete now.first;
    }
    printf("consumer cost %.3f\n",GetTime()-st);
    printf("consumer cost1 %.3f\n",cost1);
    st=GetTime();
    int ret = aio_suspend(aiolist,now_id,NULL);
    printf("aio_suspend: %d\n",ret);

    // report possible errors
    {
        for(int i=0;i<now_id;i++){
            ret = aio_error(aiolist[i]);
            printf("errno 1: %d\n",ret);
        }
    }


    fclose(file);
    //out_stream.close();
    printf("close cost %.3f\n",GetTime()-st);
}
int main(){
    double now=GetTime();
    producerDone=0;
    offest=0;
    string filename="/home/sssd/ylf_data/test.fq";
    //filename+=to_string(id);
    out_stream=fstream(filename, std::ios::out | std::ios::binary);

    thread producer(ProducerTask);
    int x1=1;
    int x2=2;
    thread consumer1(ComsumerTask,x1);
    thread consumer2(ComsumerTask,x2);
    producer.join();
    consumer1.join();
    consumer2.join();
    printf("all done cost %.3f\n",GetTime()-now);
    return 0;
}
