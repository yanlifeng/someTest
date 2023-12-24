#!/bin/bash

# 设置输入输出文件
input_file="hashes.bin"
output_file="out.bin"

# 设置线程数列表
thread_list="1 2 4 8 16 32"

# 设置运行次数
num_runs=3

# 遍历线程数
for num_threads in $thread_list; do
    # 设置环境变量 OMP_NUM_THREADS
    export NUM_THREADS=$num_threads

    # 执行性能测试
    for ((i=1; i<=$num_runs; i++)); do
        echo "Running benchmark with $num_threads threads - Run $i"
        ./11 $input_file $output_file $1
    done

    # 输出空行以分隔不同线程数的结果
    echo ""
done

# 恢复 OMP_NUM_THREADS 到默认值（如果需要的话）
unset NUM_THREADS

