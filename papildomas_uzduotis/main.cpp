#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <regex>
#include <cctype>

int main() {
    std::ifstream in("tekstas.txt");
    if (!in) return 1;

    std::ofstream out("rezultatai.txt");
    if (!out) return 1;

    std::map<std::string, int> freq;
    std::map<std::string, std::set<int>> lines;
    std::set<std::string> urls;
    std::regex url_rx(R"((https?:\/\/[^\s]+)|(www\.[^\s]+)|(\b[a-zA-Z0-9-]+\.[a-zA-Z]{2,}\b))");

    std::string line, word;
    int line_no = 0;

    while (std::getline(in, line)) {
        line_no++;

        for (std::sregex_iterator it(line.begin(), line.end(), url_rx);
             it != std::sregex_iterator(); ++it) {
            urls.insert(it->str());
        }

        word.clear();
        for (char c : line) {
            if (std::isalnum(static_cast<unsigned char>(c))) {
                word += std::tolower(static_cast<unsigned char>(c));
            } else if (!word.empty()) {
                freq[word]++;
                lines[word].insert(line_no);
                word.clear();
            }
        }
        if (!word.empty()) {
            freq[word]++;
            lines[word].insert(line_no);
        }
    }

    for (const auto& [w, c] : freq) {
        if (c > 1) {
            out << w << " : " << c << "\nEilutes: ";
            bool first = true;
            for (int ln : lines[w]) {
                if (!first) out << ", ";
                out << ln;
                first = false;
            }
            out << "\n\n";
        }
    }

    out << "=== Rasti URL'ai ===\n";
    for (const auto& u : urls) out << u << "\n";

    return 0;
}
