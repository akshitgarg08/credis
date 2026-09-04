#include <iostream>
#include <vector>
#include <string>

#include "../include/RedisCommandHandler.h" 


void printVector(const std::string& testName, const std::vector<std::string>& vec) {
    std::cout << testName << " -> [";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << "\"" << vec[i] << "\"";
        if (i < vec.size() - 1) std::cout << ", ";
    }
    std::cout << "]\n";
}

int main() {
    std::cout << "--- Running Parser Tests ---\n\n";


    std::string inlineCmd = "SET mykey myvalue";
    printVector("1. Inline Command", parseRespCommand(inlineCmd));

    std::string messyInline = "   GET    mykey   ";
    printVector("2. Messy Inline  ", parseRespCommand(messyInline));

    std::string respCmd = "*3\r\n$3\r\nSET\r\n$5\r\nmykey\r\n$7\r\nmyvalue\r\n";
    printVector("3. Valid RESP    ", parseRespCommand(respCmd));

    std::string emptyResp = "*0\r\n";
    printVector("4. Empty RESP    ", parseRespCommand(emptyResp));

    std::string emptyStr = "";
    printVector("5. Empty String  ", parseRespCommand(emptyStr));

    return 0;
}