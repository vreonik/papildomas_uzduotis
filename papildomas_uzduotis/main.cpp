#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <regex>
#include <algorithm>

static bool lt_lower(unsigned char a, unsigned char b, std::string& out) {
    if (a == 0xC4 && b == 0x85) { out += "\xC4\x85"; return true; }
    if (a == 0xC4 && b == 0x8D) { out += "\xC4\x8D"; return true; }
    if (a == 0xC4 && b == 0x99) { out += "\xC4\x99"; return true; }
    if (a == 0xC4 && b == 0x97) { out += "\xC4\x97"; return true; }
    if (a == 0xC4 && b == 0xAF) { out += "\xC4\xAF"; return true; }
    if (a == 0xC5 && b == 0xA1) { out += "\xC5\xA1"; return true; }
    if (a == 0xC5 && b == 0xB3) { out += "\xC5\xB3"; return true; }
    if (a == 0xC5 && b == 0xAB) { out += "\xC5\xAB"; return true; }
    if (a == 0xC5 && b == 0xBE) { out += "\xC5\xBE"; return true; }
    return false;
}

static bool lt_upper_to_lower(unsigned char a, unsigned char b, std::string& out) {
    if (a == 0xC4 && b == 0x84) { out += "\xC4\x85"; return true; }
    if (a == 0xC4 && b == 0x8C) { out += "\xC4\x8D"; return true; }
    if (a == 0xC4 && b == 0x98) { out += "\xC4\x99"; return true; }
    if (a == 0xC4 && b == 0x96) { out += "\xC4\x97"; return true; }
    if (a == 0xC4 && b == 0xAE) { out += "\xC4\xAF"; return true; }
    if (a == 0xC5 && b == 0xA0) { out += "\xC5\xA1"; return true; }
    if (a == 0xC5 && b == 0xB2) { out += "\xC5\xB3"; return true; }
    if (a == 0xC5 && b == 0xAA) { out += "\xC5\xAB"; return true; }
    if (a == 0xC5 && b == 0xBD) { out += "\xC5\xBE"; return true; }
    return false;
}

static bool is_url_edge_punct(char c) {
    return c == '.' || c == ',' || c == ';' || c == ':' || c == '!' || c == '?'
        || c == ')' || c == ']' || c == '}' || c == '"' || c == '\'' || c == '/';
}

static std::string clean_url(std::string s) {
    while (!s.empty() && (s.front() == '(' || s.front() == '[' || s.front() == '{' || s.front() == '"' || s.front() == '\'')) {
        s.erase(s.begin());
    }
    while (!s.empty() && is_url_edge_punct(s.back())) s.pop_back();
    return s;
}

static void flush_word(std::string& word, int line_no,
                       std::map<std::string,int>& freq,
                       std::map<std::string,std::set<int>>& line_map) {
    if (!word.empty()) {
        freq[word]++;
        line_map[word].insert(line_no);
        word.clear();
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string input_file;
    std::cout << "Ivesk teksto failo pavadinima (pvz. tekstas.txt): ";
    std::getline(std::cin, input_file);
    if (input_file.empty()) input_file = "tekstas.txt";

    int mode = 1;
    std::cout << "Rikiavimas: 1 - pagal abecele, 2 - pagal dazni (mazejanciai): ";
    std::string mode_s;
    std::getline(std::cin, mode_s);
    if (!mode_s.empty()) mode = std::max(1, std::min(2, std::stoi(mode_s)));

    std::ifstream in(input_file);
    if (!in) {
        std::cerr << "Nepavyko atidaryti failo: " << input_file << "\n";
        return 1;
    }

    std::ofstream out("rezultatai.txt");
    if (!out) {
        std::cerr << "Nepavyko sukurti rezultatai.txt\n";
        return 1;
    }

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
            std::string u = clean_url(it->str());
            if (!u.empty()) urls.insert(u);
        }

        word.clear();
        for (size_t i = 0; i < line.size(); ) {
            unsigned char c = static_cast<unsigned char>(line[i]);

            if (c < 0x80) {
                if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
                    if (c >= 'A' && c <= 'Z') c = static_cast<unsigned char>(c + 32);
                    word.push_back(static_cast<char>(c));
                } else {
                    flush_word(word, line_no, freq, lines);
                }
                i += 1;
                continue;
            }

            if (i + 1 < line.size()) {
                unsigned char d = static_cast<unsigned char>(line[i + 1]);
                if (lt_upper_to_lower(c, d, word) || lt_lower(c, d, word)) {
                    i += 2;
                    continue;
                }
            }

            flush_word(word, line_no, freq, lines);

            if ((c >> 5) == 0x6) i += 2;
            else if ((c >> 4) == 0xE) i += 3;
            else if ((c >> 3) == 0x1E) i += 4;
            else i += 1;
        }
        flush_word(word, line_no, freq, lines);
    }

    std::vector<std::pair<std::string,int>> items;
    items.reserve(freq.size());
    for (const auto& kv : freq) {
        if (kv.second > 1) items.push_back(kv);
    }

    if (mode == 2) {
        std::sort(items.begin(), items.end(),
                  [](const auto& a, const auto& b) {
                      if (a.second != b.second) return a.second > b.second;
                      return a.first < b.first;
                  });
    }

    out << "Zodziai, pasikartojantys daugiau nei 1 karta \n";
    if (mode == 1) {
        for (const auto& kv : freq) {
            const auto& w = kv.first;
            int c = kv.second;
            if (c <= 1) continue;
            out << w << " : " << c << "\nEilutes: ";
            bool first = true;
            for (int ln : lines[w]) {
                if (!first) out << ", ";
                out << ln;
                first = false;
            }
            out << "\n\n";
        }
    } else {
        for (const auto& kv : items) {
            const auto& w = kv.first;
            int c = kv.second;
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

    out << "Rasti URL'ai\n";
    for (const auto& u : urls) out << u << "\n";

    std::cout << "OK. Rezultatai: rezultatai.txt\n";
    return 0;
}
