#include <iostream>
#include <ctime>
#include <set>
#include <map>
#include <hash_set>
#include <hash_map>
#include <unordered_set>
#include <unordered_map>
 
using namespace std;
 
const int NUM = 5000000;
 
void TestSet()
{
	LARGE_INTEGER f;
	QueryPerformanceFrequency(&f);
 
	int *data = new int[NUM];
	for (int i = 0; i < NUM; i++)
		data[i] = rand() % NUM;
 
	set<int> int_set;
	hash_set<int> int_hash_set;
	unordered_set<int> int_unordered_set;
 
	LARGE_INTEGER c[10];
 
	//插入
 
	QueryPerformanceCounter(&c[0]);
 
	for (int i = 0; i < NUM; i++)
		int_set.insert(data[i]);
 
	QueryPerformanceCounter(&c[1]);
 
	for (int i = 0; i < NUM; i++)
		int_hash_set.insert(data[i]);
 
	QueryPerformanceCounter(&c[2]);
 
	for (int i = 0; i < NUM; i++)
		int_unordered_set.insert(data[i]);
 
	//查找
 
	QueryPerformanceCounter(&c[3]);
 
	for (int i = 0; i < NUM; i++)
		auto ite = int_set.find(data[i]);
 
	QueryPerformanceCounter(&c[4]);
 
	for (int i = 0; i < NUM; i++)
		auto ite = int_hash_set.find(data[i]);
 
	QueryPerformanceCounter(&c[5]);
 
	for (int i = 0; i < NUM; i++)
		auto ite = int_unordered_set.find(data[i]);
 
	//删除
 
	QueryPerformanceCounter(&c[6]);
 
	for (int i = 0; i < NUM; i++)
		int_set.erase(data[i]);
 
	QueryPerformanceCounter(&c[7]);
 
	for (int i = 0; i < NUM; i++)
		int_hash_set.erase(data[i]);
 
	QueryPerformanceCounter(&c[8]);
 
	for (int i = 0; i < NUM; i++)
		int_unordered_set.erase(data[i]);
 
	QueryPerformanceCounter(&c[9]);
 
	double ms[9];
	for (int i = 0; i <= 8; i++)
	{
		ms[i] = 1000.0*(c[i + 1].QuadPart - c[i].QuadPart) / f.QuadPart;
	}
 
 
	cout << "set插入 " << NUM << " 条数据耗时: " << ms[0] << " ms" << endl;
	cout << "hash_set插入 " << NUM << " 条数据耗时: " << ms[1] << " ms" << endl;
	cout << "unordered_set插入 " << NUM << " 条数据耗时: " << ms[2] << " ms" << endl;
	cout << endl;
 
	cout << "set查找 " << NUM << " 条数据耗时: " << ms[3] << " ms" << endl;
	cout << "hash_set查找 " << NUM << " 条数据耗时: " << ms[4] << " ms" << endl;
	cout << "unordered_set查找 " << NUM << " 条数据耗时: " << ms[5] << " ms" << endl;
	cout << endl;
 
	cout << "set删除 " << NUM << " 条数据耗时: " << ms[6] << " ms" << endl;
	cout << "hash_set删除 " << NUM << " 条数据耗时: " << ms[7] << " ms" << endl;
	cout << "unordered_set删除 " << NUM << " 条数据耗时: " << ms[8] << " ms" << endl;
	cout << endl;
 
	delete[]data;
}
 
void TestMap()
{
	LARGE_INTEGER f;
	QueryPerformanceFrequency(&f);
 
	int *data = new int[NUM];
	for (int i = 0; i < NUM; i++)
		data[i] = rand() % NUM;
 
	map<int, int> int_map;
	hash_map<int, int> int_hash_map;
	unordered_map<int, int> int_unordered_map;
 
	LARGE_INTEGER c[10];
 
	//插入
 
	QueryPerformanceCounter(&c[0]);
 
	for (int i = 0; i < NUM; i++)
		int_map.insert(make_pair(data[i], data[NUM - i]));
 
	QueryPerformanceCounter(&c[1]);
 
	for (int i = 0; i < NUM; i++)
		int_hash_map.insert(make_pair(data[i], data[NUM - i]));
 
	QueryPerformanceCounter(&c[2]);
 
	for (int i = 0; i < NUM; i++)
		int_unordered_map.insert(make_pair(data[i], data[NUM - i]));
 
	//查找
 
	QueryPerformanceCounter(&c[3]);
 
	for (int i = 0; i < NUM; i++)
		auto ite = int_map.find(data[i]);
 
	QueryPerformanceCounter(&c[4]);
 
	for (int i = 0; i < NUM; i++)
		auto ite = int_hash_map.find(data[i]);
 
	QueryPerformanceCounter(&c[5]);
 
	for (int i = 0; i < NUM; i++)
		auto ite = int_unordered_map.find(data[i]);
 
	//删除
 
	QueryPerformanceCounter(&c[6]);
 
	for (int i = 0; i < NUM; i++)
		int_map.erase(data[i]);
 
	QueryPerformanceCounter(&c[7]);
 
	for (int i = 0; i < NUM; i++)
		int_hash_map.erase(data[i]);
 
	QueryPerformanceCounter(&c[8]);
 
	for (int i = 0; i < NUM; i++)
		int_unordered_map.erase(data[i]);
 
	QueryPerformanceCounter(&c[9]);
 
	double ms[9];
	for (int i = 0; i <= 8; i++)
	{
		ms[i] = 1000.0*(c[i + 1].QuadPart - c[i].QuadPart) / f.QuadPart;
	}
 
 
	cout << "map插入 " << NUM << " 条数据耗时: " << ms[0] << " ms" << endl;
	cout << "hash_map插入 " << NUM << " 条数据耗时: " << ms[1] << " ms" << endl;
	cout << "unordered_map插入 " << NUM << " 条数据耗时: " << ms[2] << " ms" << endl;
	cout << endl;
 
	cout << "map查找 " << NUM << " 条数据耗时: " << ms[3] << " ms" << endl;
	cout << "hash_map查找 " << NUM << " 条数据耗时: " << ms[4] << " ms" << endl;
	cout << "unordered_map查找 " << NUM << " 条数据耗时: " << ms[5] << " ms" << endl;
	cout << endl;
 
	cout << "map删除 " << NUM << " 条数据耗时: " << ms[6] << " ms" << endl;
	cout << "hash_map删除 " << NUM << " 条数据耗时: " << ms[7] << " ms" << endl;
	cout << "unordered_map删除 " << NUM << " 条数据耗时: " << ms[8] << " ms" << endl;
	cout << endl;
 
	delete[]data;
}
 
int main()
{
	TestSet();
	cout << endl;
	cout << endl;
	TestMap();
}
