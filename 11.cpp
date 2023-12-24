#include <iostream>
#include <fstream>
#include <map>
#include <unordered_map>
#include <cstdint>
#include <string>
#include <set>
#include <vector>
#include <cmath>
#include <sys/time.h>
#include "robin_hood.h"
#include <omp.h>
#include <algorithm>
#include <thread>

inline double GetTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (double) tv.tv_sec + (double) tv.tv_usec / 1000000;
}

std::vector<uint64_t> A;
std::vector<uint64_t> B;

#define mod  1999999973
#define MAXN 2000000000
//#define MAXN ((1ll << 32) - 1)

//#define mod  3999999979 
//#define MAXN 4000000000

#define MAXM 600000000
int *head;
struct node{
    int pre, pos;
    uint64_t val;
};
node *E;
int num;
//#define bits 28
#define bits 31

struct ggnode {
    int hval, pos;
    uint64_t val;
};

ggnode *AA;

std::vector<uint64_t>randstrobe_start_indices;

inline uint32_t mod_hash(uint64_t key) {
    return key % mod;
}

uint64_t* hashtable;

inline void BloomFilter_push_mod(uint64_t key) {
    uint32_t a = key&0xffffffff;
    a = (a ^ 61) ^ (a >> 16);
    a = a + (a << 3);
    a = a ^ (a >> 4);
    a = a * 0x27d4eb2d;
    a = a ^ (a >> 15);
    a = (a>>6)&0x3fff;
    uint32_t mapkey = (key >> 32)|(a << 18);
    hashtable[mapkey>>6] |= (1ll<<(mapkey&0x3f));
}

inline bool BloomFilter_get_mod(uint64_t key) {
    uint32_t a = key&0xffffffff;
    a = (a ^ 61) ^ (a >> 16);
    a = a + (a << 3);
    a = a ^ (a >> 4);
    a = a * 0x27d4eb2d;
    a = a ^ (a >> 15);
    a = (a>>6)&0x3fff;
    uint32_t mapkey = (key >> 32)|(a << 18);
    return hashtable[mapkey>>6]&(1ll<<(mapkey&0x3f));
}

inline uint32_t mod_hash2(uint64_t key) {
    uint32_t a = key & 0xffffffff;
    a = (a ^ 61) ^ (a >> 16);
    a = a + (a << 3);
    a = a ^ (a >> 4);
    a = a * 0x27d4eb2d;
    a = a ^ (a >> 15);
    a = (a >> 6) & 0x3fff;
    uint32_t mapkey = (key >> 32)|(a << 18);
    return mapkey;
}

bool cmp(ggnode a, ggnode b) {
    if(a.hval == b.hval) {
        if(a.val == b.val) return a.pos < b.pos;
        return a.val < b.val;
    }
    return a.hval < b.hval;
}

void BuildDatabaseGGmy() {
    double t0 = GetTime();

    //auto last = std::unique(A.begin(), A.end());
    //A.erase(last, A.end());

    AA = new ggnode[A.size()];
    for(int i = 0; i < A.size(); i++) {
        AA[i] = {mod_hash(A[i]), i, A[i]};
    }
    printf("GG init1 my %lf\n", GetTime() - t0);

    t0 = GetTime();
    randstrobe_start_indices.reserve(MAXN + 1);
    printf("GG init2 my %lf\n", GetTime() - t0);

    t0 = GetTime();
    std::sort(AA, AA + A.size(), cmp);
    printf("GG init3 my %lf\n", GetTime() - t0);

    t0 = GetTime();
    for (int i = 0; i < A.size(); i++) {
        const unsigned int cur_hash_N = AA[i].hval;
        while (randstrobe_start_indices.size() <= cur_hash_N) {
            randstrobe_start_indices.push_back(i);
        }
    }
    while (randstrobe_start_indices.size() < MAXN + 1) {
        randstrobe_start_indices.push_back(A.size());
    }
    printf("GG init4 my %lf\n", GetTime() - t0);
    printf("GG arrary size my %d\n", randstrobe_start_indices.size());

}

inline int check_GGmy(uint64_t key) {
    constexpr int MAX_LINEAR_SEARCH = 4;
    const unsigned int top_N = key;
    uint64_t position_start = randstrobe_start_indices[top_N];
    uint64_t position_end = randstrobe_start_indices[top_N + 1];
    return position_end - position_start;
}


inline int find_GGmy(uint64_t key) {
    constexpr int MAX_LINEAR_SEARCH = 4;
    const unsigned int top_N = mod_hash(key);
    uint64_t position_start = randstrobe_start_indices[top_N];
    uint64_t position_end = randstrobe_start_indices[top_N + 1];
    if (position_start == position_end) {
        return -1;
    }

    for ( ; position_start < position_end; ++position_start) {
        if (AA[position_start].val == key) return AA[position_start].pos;
        //if (AA[position_start].val > key) return -1;
    }
    return -1;
}


void QueryDatabaseGGmy_thread() {
    double t0 = GetTime();

    // Get the value of the environment variable "NUM_THREADS"
    const char* numThreadsEnv = std::getenv("NUM_THREADS");

    // Set the number of threads based on the environment variable or use hardware concurrency
    int num_threads = (numThreadsEnv != nullptr) ? std::atoi(numThreadsEnv) : std::thread::hardware_concurrency();
    printf("thread num %d\n", num_threads);
    uint64_t sum[num_threads] = {0};


    // Container to hold thread objects
    std::vector<std::thread> threads;

    // Function to be executed by each thread
    auto thread_function = [&](int thread_id) {
        // Calculate the range of elements for this thread
        int chunk_size = B.size() / num_threads;
        int start = thread_id * chunk_size;
        int end = (thread_id == num_threads - 1) ? B.size() : (thread_id + 1) * chunk_size;

        // Process the elements assigned to this thread
        for (int i = start; i < end; ++i) {
            auto res = find_GGmy(B[i]);
            if (res != -1) {
                // Use a lock or atomic operation if updating a shared variable
                sum[thread_id] += res;
            }
        }
    };

    // Create threads and assign tasks
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(thread_function, i);
    }

    // Join threads to wait for their completion
    for (auto& thread : threads) {
        thread.join();
    }

    uint64_t summ = 0;
    for(int i = 0; i < num_threads; i++) summ += sum[i];


    printf("GG query cost %lf s\n", GetTime() - t0);
    printf("GG query sum %llu\n", summ);
}

void CheckDatabaseGGmy() {
    std::map<int, int> cntt;
    for(uint32_t i = 0; i < MAXN; i++) {
        auto res = check_GGmy(i);
        if(cntt.find(res) == cntt.end()) {
            cntt[res] = 0;
        }
        cntt[res]++;
    }
    for(auto item : cntt) printf("[info gg] : %d %d\n", item.first, item.second);
}


void BuildDatabaseGG() {
    double t0 = GetTime();

    randstrobe_start_indices.reserve((1u << bits) + 1);

    for (uint64_t position = 0; position < A.size(); ++position) {

        const unsigned int cur_hash_N = A[position] >> (64 - bits);
        while (randstrobe_start_indices.size() <= cur_hash_N) {
            randstrobe_start_indices.push_back(position);
        }
    }
    while (randstrobe_start_indices.size() < ((1u << bits) + 1)) {
        randstrobe_start_indices.push_back(A.size());
    }
    printf("GG init %lf\n", GetTime() - t0);
    printf("GG arrary size %d\n", randstrobe_start_indices.size());

}

inline int check_GG(uint64_t key) {
    constexpr int MAX_LINEAR_SEARCH = 4;
    const unsigned int top_N = key;
    uint64_t position_start = randstrobe_start_indices[top_N];
    uint64_t position_end = randstrobe_start_indices[top_N + 1];
    return position_end - position_start;
}


inline int find_GG(uint64_t key) {
    constexpr int MAX_LINEAR_SEARCH = 4;
    const unsigned int top_N = key >> (64 - bits);
    uint64_t position_start = randstrobe_start_indices[top_N];
    uint64_t position_end = randstrobe_start_indices[top_N + 1];
    if (position_start == position_end) {
        return -1;
    }

    if (position_end - position_start < MAX_LINEAR_SEARCH) {
        for ( ; position_start < position_end; ++position_start) {
            if (A[position_start] == key) return position_start;
            if (A[position_start] > key) return -1;
        }
        return -1;
    }

    auto pos = std::lower_bound(A.begin() + position_start, A.begin() + position_end, key);
    if (pos != A.end() && *pos == key) {
        return pos - A.begin();
    } else {
        return -1;
    }
}


void QueryDatabaseGG_thread() {
    double t0 = GetTime();

    // Get the value of the environment variable "NUM_THREADS"
    const char* numThreadsEnv = std::getenv("NUM_THREADS");

    // Set the number of threads based on the environment variable or use hardware concurrency
    int num_threads = (numThreadsEnv != nullptr) ? std::atoi(numThreadsEnv) : std::thread::hardware_concurrency();
    printf("thread num %d\n", num_threads);
    uint64_t sum[num_threads] = {0};


    // Container to hold thread objects
    std::vector<std::thread> threads;

    // Function to be executed by each thread
    auto thread_function = [&](int thread_id) {
        // Calculate the range of elements for this thread
        int chunk_size = B.size() / num_threads;
        int start = thread_id * chunk_size;
        int end = (thread_id == num_threads - 1) ? B.size() : (thread_id + 1) * chunk_size;

        // Process the elements assigned to this thread
        for (int i = start; i < end; ++i) {
            auto res = find_GG(B[i]);
            if (res != -1) {
                // Use a lock or atomic operation if updating a shared variable
                sum[thread_id] += res;
            }
        }
    };

    // Create threads and assign tasks
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(thread_function, i);
    }

    // Join threads to wait for their completion
    for (auto& thread : threads) {
        thread.join();
    }

    uint64_t summ = 0;
    for(int i = 0; i < num_threads; i++) summ += sum[i];


    printf("GG query cost %lf s\n", GetTime() - t0);
    printf("GG query sum %llu\n", summ);
}

void CheckDatabaseGG() {
    std::map<int, int> cntt;
    for(uint32_t i = 0; i < (1u << bits); i++) {
        auto res = check_GG(i);
        if(cntt.find(res) == cntt.end()) {
            cntt[res] = 0;
        }
        cntt[res]++;
    }
    for(auto item : cntt) printf("[info gg] : %d %d\n", item.first, item.second);
}

void QueryDatabaseGG() {
    double t0 = GetTime();
    uint64_t sum = 0;
    //#pragma omp parallel for schedule(dynamic) reduction(+:sum)
    for(int i = 0; i < B.size(); i++) {
        auto res = find_GG(B[i]);
        if(res != -1) sum += res;
    }
    printf("GG query cost %lf s\n", GetTime() - t0);
    printf("GG query sum %llu\n", sum);
}

const static uint32_t HashTableMax = 1ll<<26;

void BuildDatabaseMy() {

    double t0 = GetTime();
    head = new int[MAXN];

	hashtable = (uint64_t*)malloc(HashTableMax*sizeof(uint64_t));
    memset(hashtable,0,sizeof(uint64_t)*HashTableMax);

    E = new node[MAXM];
    num = 0;
    for(uint32_t i = 0; i < MAXN; i++) head[i] = -1;
    for(int i = 0; i < A.size(); i++) {
        //BloomFilter_push_mod(A[i]);
        uint32_t ha = mod_hash(A[i]);
        bool find = 0;
        for(int j = head[ha]; j != -1; j = E[j].pre) {
            if(A[i] == E[j].val) {
                find = 1;
                break;
            }
        }
        if(find == 0) {
            E[num].val = A[i];
            E[num].pos = i;
            E[num].pre = head[ha];
            head[ha] = num++;
        }
    }
    printf("My hashtable init %lf s\n", GetTime() - t0);
}

void QueryDatabaseMy_thread() {
    double t0 = GetTime();

    // Get the value of the environment variable "NUM_THREADS"
    const char* numThreadsEnv = std::getenv("NUM_THREADS");

    // Set the number of threads based on the environment variable or use hardware concurrency
    int num_threads = (numThreadsEnv != nullptr) ? std::atoi(numThreadsEnv) : std::thread::hardware_concurrency();
    printf("thread num %d\n", num_threads);
    uint64_t sum[num_threads] = {0};
    uint64_t not_find[num_threads] = {0};


    // Container to hold thread objects
    std::vector<std::thread> threads;

    // Function to be executed by each thread
    auto thread_function = [&](int thread_id) {
        // Calculate the range of elements for this thread
        int chunk_size = B.size() / num_threads;
        int start = thread_id * chunk_size;
        int end = (thread_id == num_threads - 1) ? B.size() : (thread_id + 1) * chunk_size;

        // Process the elements assigned to this thread
        for (int i = start; i < end; ++i) {
            //if(BloomFilter_get_mod(B[i]) == 0) continue;
            uint32_t ha = mod_hash(B[i]);
            //bool find = 0;
            for(int j = head[ha]; j != -1; j = E[j].pre) {
                if(B[i] == E[j].val) {
                    //find = 1;
                    sum[thread_id] += E[j].pos;
                    break;
                }
            }
            //if(find == 0) not_find[thread_id]++;
        }
    };

    // Create threads and assign tasks
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(thread_function, i);
    }

    // Join threads to wait for their completion
    for (auto& thread : threads) {
        thread.join();
    }

    uint64_t summ = 0;
    for(int i = 0; i < num_threads; i++) summ += sum[i];

    uint64_t not_num = 0;
    for(int i = 0; i < num_threads; i++) not_num += not_find[i];


    printf("My query cost %lf s\n", GetTime() - t0);
    printf("My query sum %llu\n", summ);
    printf("My query not num %llu\n", not_num);
}


void CheckDatabaseMy() {
    std::map<int, int> cntt;
    for(int i = 0; i < MAXN; i++) {
        int cnt = 0;
        for(int j = head[i]; j != -1; j = E[j].pre) {
            cnt++;
        }
        auto res = cnt;
        if(cntt.find(res) == cntt.end()) {
            cntt[res] = 0;
        }
        cntt[res]++;
    }
    for(auto item : cntt) printf("[info my] : %d %d\n", item.first, item.second);
}

void QueryDatabaseMy() {
    double t0 = GetTime();
    uint64_t sum = 0;
    //#pragma omp parallel for schedule(dynamic) reduction(+:sum)
    for(int i = 0; i < B.size(); i++) {
        uint32_t ha = mod_hash(B[i]);
        bool find = 0;
        for(int j = head[ha]; j != -1; j = E[j].pre) {
            if(B[i] == E[j].val) {
                find = 1;
                sum += E[j].pos;
                break;
            }
        }
    }
    printf("my hashtable query %lf s\n", GetTime() - t0);
    printf("my hashtable query sum %llu\n", sum);
}




void BuildDatabaseMap(std::map<uint64_t, uint64_t>& database) {
    double t0 = GetTime();
    for(int i = 0; i < A.size(); i++) {
        if(database.find(A[i]) == database.end())
            database[A[i]] = i;
    }
    printf("stl map init %lf s\n", GetTime() - t0);
}


void BuildDatabaseUnorderedMap(std::unordered_map<uint64_t, uint64_t>& database) {
    double t0 = GetTime();
    for(int i = 0; i < A.size(); i++) {
        if(database.find(A[i]) == database.end())
            database[A[i]] = i;
    }
    printf("stl unordered_map init %lf s\n", GetTime() - t0);
}

void BuildDatabaseRHUnorderedMap(robin_hood::unordered_map<uint64_t, uint64_t>& database) {
    double t0 = GetTime();
    for(int i = 0; i < A.size(); i++) {
        if(database.find(A[i]) == database.end())
            database[A[i]] = i;
    }
    printf("robin_hood unordered_map init %lf s\n", GetTime() - t0);
}

void QueryDatabaseMap(std::map<uint64_t, uint64_t>& database) {
    uint64_t sum = 0;
    double t0 = GetTime();
    for(int i = 0; i < B.size(); i++) {
        if (database.find(B[i]) != database.end()) {
            sum += database[B[i]];
        }
    }
    printf("stl map query %lf s\n", GetTime() - t0);
    printf("stl map query sum %llu\n", sum);
}

void QueryDatabaseUnorderedMap(std::unordered_map<uint64_t, uint64_t>& database) {
    uint64_t sum = 0;
    double t0 = GetTime();
    for(int i = 0; i < B.size(); i++) {
        if (database.find(B[i]) != database.end()) {
            sum += database[B[i]];
        }
    }
    printf("stl unordered_map query %lf s\n", GetTime() - t0);
    printf("stl unordered_map query sum %llu\n", sum);
}

void QueryDatabaseRHUnorderedMap(robin_hood::unordered_map<uint64_t, uint64_t>& database) {
    uint64_t sum = 0;
    double t0 = GetTime();
#pragma omp parallel for schedule(dynamic) reduction(+:sum)
    for(int i = 0; i < B.size(); i++) {
        if (database.find(B[i]) != database.end()) {
            sum += database[B[i]];
        }
    }
    printf("robin_hood unordered_map query %lf s\n", GetTime() - t0);
    printf("robin_hood unordered_map query sum %llu\n", sum);
}

int main(int argc, char* argv[]) {
    int flag = stoi(std::string(argv[3]));
    std::string databaseFilename = std::string(argv[1]);  // Replace with your actual file name
    std::string queryFilename = std::string(argv[2]); // Replace with your actual file name
    printf("==\n%s\n%s\n", databaseFilename.c_str(), queryFilename.c_str());

    double t0 = GetTime();
    std::ifstream input_file1(databaseFilename, std::ios::binary);
    if (!input_file1.is_open()) {
        std::cerr << "Failed to open file for reading." << std::endl;
        return 1;
    }

    uint64_t data_count1;
    input_file1.read(reinterpret_cast<char*>(&data_count1), sizeof(data_count1));

    A.resize(data_count1);
    input_file1.read(reinterpret_cast<char*>(A.data()), data_count1 * sizeof(uint64_t));


    //for(int i = 0; i < 10000; i++) {
    //    for(int j = 0; j < 10; j++) {
    //        data_count1++;
    //        A.push_back(1ll * mod + i * j);
    //    }
    //}

    if (!input_file1) {
        std::cerr << "Error occurred during file reading." << std::endl;
        return 1;
    }
    printf("read map data %lf s\n", GetTime() - t0);
    printf("map size %llu\n", A.size());


    t0 = GetTime();
    std::ifstream input_file2(queryFilename, std::ios::binary);
    if (!input_file2.is_open()) {
        std::cerr << "Failed to open file for reading." << std::endl;
        return 1;
    }

    uint64_t data_count2;
    input_file2.read(reinterpret_cast<char*>(&data_count2), sizeof(data_count2));

    B.resize(data_count2);
    input_file2.read(reinterpret_cast<char*>(B.data()), data_count2 * sizeof(uint64_t));

    if (!input_file2) {
        std::cerr << "Error occurred during file reading." << std::endl;
        return 1;
    }

    printf("read query data %lf s\n", GetTime() - t0);
    printf("query size %llu\n", B.size());

    //std::map<uint64_t, uint64_t> databaseMap;
    //BuildDatabaseMap(databaseMap);
    //QueryDatabaseMap(databaseMap);

    //std::unordered_map<uint64_t, uint64_t> databaseUnorderedMap;
    //BuildDatabaseUnorderedMap(databaseUnorderedMap);
    //QueryDatabaseUnorderedMap(databaseUnorderedMap);

    //robin_hood::unordered_map<uint64_t, uint64_t> databaseRHUnorderedMap;
    //BuildDatabaseRHUnorderedMap(databaseRHUnorderedMap);
    //QueryDatabaseRHUnorderedMap(databaseRHUnorderedMap);
    if(flag) {
        BuildDatabaseMy();
        //CheckDatabaseMy();
        QueryDatabaseMy_thread();
        //QueryDatabaseMy();
    } else {
        BuildDatabaseGGmy();
        //BuildDatabaseGG();
        //CheckDatabaseGG();
        //CheckDatabaseGGmy();
        QueryDatabaseGGmy_thread();
        //QueryDatabaseGG_thread();
        //QueryDatabaseGG();
    }


    //BuildDatabaseMyBaBa();
    //QueryDatabaseMyBaBa();

    return 0;
}

