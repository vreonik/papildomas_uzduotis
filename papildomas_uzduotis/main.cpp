#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cctype>

int main() {
    std::ifstream in("tekstas.txt");
    if (!in) return 1;

    std::map<std::string, int> freq;
    std::string line, word;

    while (std::getline(in, line)) {
        word.clear();
        for (char c : line) {
            if (std::isalnum(static_cast<unsigned char>(c))) {
                word += std::tolower(static_cast<unsigned char>(c));
            } else if (!word.empty()) {
                freq[word]++;
                word.clear();
            }
        }
        if (!word.empty()) freq[word]++;
    }
    return 0;
}
