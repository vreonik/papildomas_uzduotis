#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::ifstream in("tekstas.txt");
    if (!in) return 1;

    std::string line;
    while (std::getline(in, line)) {
    }
    return 0;
}
