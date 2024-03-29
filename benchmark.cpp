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

inline double GetTime() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (double) tv.tv_sec + (double) tv.tv_usec / 1000000;
}

std::vector<uint64_t> A;
std::vector<uint64_t> B;

#define mod  1999999973
#define MAXN 2000000000

//#define mod   9999999967ll
//#define MAXN 10000000000ll

#define MAXM 600000000
int *head;
int *pre;
uint64_t *val;
uint64_t *pos;
int *pos_int;
int num;
#define bits 28

std::vector<uint64_t>randstrobe_start_indices;

void BuildDatabaseGG() {
	double t0 = GetTime();

	randstrobe_start_indices.reserve((1u << bits) + 1);
	uint64_t prev_hash = A.empty() ? 0 : A[0];

	for (uint64_t position = 0; position < A.size(); ++position) {
		const uint64_t cur_hash = A[position];
		if (cur_hash == prev_hash) {
			continue;
		}

		const unsigned int cur_hash_N = cur_hash >> (64 - bits);
		while (randstrobe_start_indices.size() <= cur_hash_N) {
			randstrobe_start_indices.push_back(position);
		}
		prev_hash = cur_hash;
	}
	while (randstrobe_start_indices.size() < ((1u << bits) + 1)) {
		randstrobe_start_indices.push_back(A.size());
	}
	printf("GG init %lf\n", GetTime() - t0);
	printf("GG arrary size %d\n", randstrobe_start_indices.size());

}
inline size_t find_GG(uint64_t key) {
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

void QueryDatabaseGG() {
    double t0 = GetTime();
    uint64_t sum = 0;
    for(int i = 0; i < B.size(); i++) {
        auto res = find_GG(B[i]);
        if(res != -1) sum += res;
    }
    printf("GG query %lf s\n", GetTime() - t0);
    printf("GG query %llu\n", sum);
}

void BuildDatabaseMy() {
	double t0 = GetTime();
	head = new int[MAXN];
	pre = new int[MAXM];
	val = new uint64_t[MAXM];
	pos = new uint64_t[MAXM];
	num = 0;
	for(size_t i = 0; i < MAXN; i++) head[i] = -1;
	for(int i = 0; i < A.size(); i++) {
		size_t ha = A[i] % mod;
		bool find = 0;
		for(int j = head[ha]; j != -1; j = pre[j]) {
			if(A[i] == val[j]) {
				//pos[j] = i;
				find = 1;
				break;
			}
		}
		if(find == 0) {
			val[num] = A[i];
			pos[num] = i;
			pre[num] = head[ha];
			head[ha] = num++;
		}
	}
	printf("My hashtable init %lf s\n", GetTime() - t0);
}

void QueryDatabaseMy() {
    int cntt[100] = {0};
	uint64_t sum = 0;
	double t0 = GetTime();
	for(int i = 0; i < B.size(); i++) {
		size_t ha = B[i] % mod;
		bool find = 0;
        int cnt = 0;
        int index = 0;
		for(int j = head[ha]; j != -1; j = pre[j]) {
            cnt++;
			if(B[i] == val[j]) {
				find = 1;
				sum += pos[j];
				break;
			}
		}
        //if(cnt) index = static_cast<int>(std::log10(cnt));
        index = cnt;
        cntt[index]++;
	}
    for(int i = 0; i < 100; i++) {
        //printf("== %lld %d\n", (long long)(std::pow(10, i)), cntt[i]);
        printf("== %d %d\n", i, cntt[i]);
    }
	printf("my hashtable query %lf s\n", GetTime() - t0);
	printf("my hashtable query sum %llu\n", sum);
}


void BuildDatabaseMyBaBa() {
	double t0 = GetTime();
	head = new int[MAXN];
	pre = new int[MAXM];
	val = new uint64_t[MAXM];
	pos_int = new int[MAXM];
	num = 0;
	for(int i = 0; i < MAXN; i++) head[i] = -1;
	for(int i = 0; i < A.size(); i++) {
		int ha = A[i] % mod;
		bool find = 0;
		for(int j = head[ha]; j != -1; j = pre[j]) {
			if(A[i] == val[j]) {
				//pos_int[j] = i;
				find = 1;
				break;
			}
		}
		if(find == 0) {
			val[num] = A[i];
			pos_int[num] = i;
			pre[num] = head[ha];
			head[ha] = num++;
		}
	}
	printf("My baba hashtable init %lf s\n", GetTime() - t0);
}

void QueryDatabaseMyBaBa() {
	uint64_t sum = 0;
	double t0 = GetTime();
	for(int i = 0; i < B.size(); i++) {
		int ha = B[i] % mod;
		bool find = 0;
		for(int j = head[ha]; j != -1; j = pre[j]) {
			if(B[i] == val[j]) {
				find = 1;
				sum += pos_int[j];
				break;
			}
		}
	}
	printf("my baba hashtable query %lf s\n", GetTime() - t0);
	printf("my baba hashtable query sum %llu\n", sum);
}


void BuildDatabaseMap(std::map<uint64_t, uint64_t>& database) {
	double t0 = GetTime();
	for(int i = 0; i < A.size(); i++) {
		database[A[i]] = i;
	}
	printf("stl map init %lf s\n", GetTime() - t0);
}


void BuildDatabaseUnorderedMap(std::unordered_map<uint64_t, uint64_t>& database) {
	double t0 = GetTime();
	for(int i = 0; i < A.size(); i++) {
		database[A[i]] = i;
	}
	printf("stl unordered_map init %lf s\n", GetTime() - t0);
}

void BuildDatabaseRHUnorderedMap(robin_hood::unordered_map<uint64_t, uint64_t>& database) {
	double t0 = GetTime();
	for(int i = 0; i < A.size(); i++) {
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
	for(int i = 0; i < B.size(); i++) {
		if (database.find(B[i]) != database.end()) {
			sum += database[B[i]];
		}
	}
	printf("robin_hood unordered_map query %lf s\n", GetTime() - t0);
	printf("robin_hood unordered_map query sum %llu\n", sum);
}

int main(int argc, char* argv[]) {
	std::string databaseFilename = std::string(argv[1]);  // Replace with your actual file name
	std::string queryFilename = std::string(argv[2]); // Replace with your actual file name
	printf("==\n%s\n%s\n", databaseFilename.c_str(), queryFilename.c_str());

	std::ifstream file1(databaseFilename);
	uint64_t number;
	uint64_t number2 = 0;
	double t0 = GetTime();
    std::set<uint64_t>S;
	while (file1 >> number) {
		A.push_back(number);
        S.insert(number);
        if(number < number2) {
            printf("GG no order %llu %llu\n", number, number2);
        }
        number2 = number;
	}
	printf("read map data %lf s\n", GetTime() - t0);
	printf("map size %llu\n", A.size());
	printf("set size %llu\n", S.size());

	std::ifstream file2(queryFilename);
	t0 = GetTime();
	while (file2 >> number) {
		B.push_back(number);
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


	BuildDatabaseMy();
	QueryDatabaseMy();


	//BuildDatabaseMyBaBa();
	//QueryDatabaseMyBaBa();

    BuildDatabaseGG();
    QueryDatabaseGG();


	return 0;
}

