#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cstring>

int main(int argc, char* argv[]) {
    // 打开文本文件
    std::string infile(argv[1]);
    std::string outfile(argv[2]);
    printf("%s\n%s\n", infile.c_str(), outfile.c_str());

    std::ifstream input_file(infile);
    if (!input_file.is_open()) {
        std::cerr << "Failed to open file for reading." << std::endl;
        return 1;
    }

    // 读取 uint64_t 数据
    std::vector<uint64_t> data;
    uint64_t value;
    while (input_file >> value) {
        data.push_back(value);
    }

    // 打开二进制文件用于写入
    std::ofstream output_file(outfile, std::ios::binary);
    if (!output_file.is_open()) {
        std::cerr << "Failed to open file for writing." << std::endl;
        return 1;
    }
    printf("size %d\n", data.size());

    // 写入数据的数量
    uint64_t data_count = data.size();
    output_file.write(reinterpret_cast<const char*>(&data_count), sizeof(data_count));

    // 写入数据
    for (uint64_t num : data) {
        output_file.write(reinterpret_cast<const char*>(&num), sizeof(num));
    }

    return 0;
}

