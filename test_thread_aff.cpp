#include <iostream>
#include <thread>
#include <vector>
#include <pthread.h>

const int MATRIX_SIZE = (1 << 10) * 64; // 定义矩阵大小

int n_thread = 32;

// 简单的矩阵乘法
void matrixMultiply(int startRow, int endRow, const std::vector<std::vector<int>>& matA,
                    const std::vector<std::vector<int>>& matB, std::vector<std::vector<int>>& result) {
    for (int i = startRow; i < endRow; ++i) {
        for (int j = 0; j < MATRIX_SIZE; ++j) {
            for (int k = 0; k < MATRIX_SIZE; ++k) {
                result[i][j] += matA[i][k] * matB[k][j];
            }
        }
    }
}

// 线程函数
void threadFunction(int threadNum, const std::vector<std::vector<int>>& matA,
                    const std::vector<std::vector<int>>& matB, std::vector<std::vector<int>>& result) {
    // 设置线程的 CPU 亲和性
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(threadNum, &cpuset);

    pthread_t current_thread = pthread_self();
    if (pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset) != 0) {
        std::cerr << "Error setting thread affinity" << std::endl;
    }

    // 计算每个线程处理的行数
    int rowsPerThread = MATRIX_SIZE / n_thread;
    int startRow = threadNum * rowsPerThread;
    int endRow = startRow + rowsPerThread;
    //printf("thread %d :[%d, %d]\n", threadNum, startRow, endRow);

    // 执行矩阵乘法
    matrixMultiply(startRow, endRow, matA, matB, result);
}

int main() {
    // 初始化矩阵 A 和 B
    std::vector<std::vector<int>> matA(MATRIX_SIZE, std::vector<int>(MATRIX_SIZE, 1));
    std::vector<std::vector<int>> matB(MATRIX_SIZE, std::vector<int>(MATRIX_SIZE, 2));
    std::vector<std::vector<int>> result(MATRIX_SIZE, std::vector<int>(MATRIX_SIZE, 0));

    std::vector<std::thread> threads;
    int numThreads = n_thread; // 获取硬件支持的线程数

    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(threadFunction, i, std::ref(matA), std::ref(matB), std::ref(result));
    }

    for (auto& t : threads) {
        t.join();
    }

    // 打印结果的一部分，以验证
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            std::cout << result[i][j] << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}

