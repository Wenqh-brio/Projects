// utf8_convert.cpp
// 示例：在数字（Unicode code point）与 UTF-8 字节序列之间相互转换
#include <bits/stdc++.h>
using namespace std;

string codepoint_to_utf8(uint32_t cp) {
    string out;
    if (cp <= 0x7F) {
        out.push_back(static_cast<char>(cp));
    } else if (cp <= 0x7FF) {
        out.push_back(static_cast<char>(0xC0 | ((cp >> 6) & 0x1F)));
        out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
    } else if (cp <= 0xFFFF) {
        out.push_back(static_cast<char>(0xE0 | ((cp >> 12) & 0x0F)));
        out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
    } else if (cp <= 0x10FFFF) {
        out.push_back(static_cast<char>(0xF0 | ((cp >> 18) & 0x07)));
        out.push_back(static_cast<char>(0x80 | ((cp >> 12) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
    }
    return out;
}

// 从 UTF-8 字符串的指定位置解码一个 code point，返回是否成功并写出消耗的字节数
bool utf8_decode_at(const string &s, size_t pos, uint32_t &out_cp, size_t &out_len) {
    out_cp = 0;
    out_len = 0;
    if (pos >= s.size()) return false;
    unsigned char c0 = static_cast<unsigned char>(s[pos]);
    if (c0 <= 0x7F) {
        out_cp = c0;
        out_len = 1;
        return true;
    }
    // Determine expected length
    int expected = 0;
    uint32_t cp = 0;
    if ((c0 & 0xE0) == 0xC0) { expected = 2; cp = c0 & 0x1F; }
    else if ((c0 & 0xF0) == 0xE0) { expected = 3; cp = c0 & 0x0F; }
    else if ((c0 & 0xF8) == 0xF0) { expected = 4; cp = c0 & 0x07; }
    else return false;
    if (pos + expected > s.size()) return false;
    for (int i = 1; i < expected; ++i) {
        unsigned char cb = static_cast<unsigned char>(s[pos + i]);
        if ((cb & 0xC0) != 0x80) return false;
        cp = (cp << 6) | (cb & 0x3F);
    }
    out_cp = cp;
    out_len = expected;
    return true;
}

string bytes_to_hex(const string &s) {
    std::ostringstream oss;
    oss << hex << setfill('0');
    for (unsigned char c : s) {
        oss << "0x" << setw(2) << (int)c << ' ';
    }
    return oss.str();
}

int main() {
    vector<uint32_t> cps = {0x24, 0xA2, 0x20AC, 0x10348, 0x4E2D, 0x1F600};
    cout << "数字 -> UTF-8 bytes -> 字符示例:\n";
    for (uint32_t cp : cps) {
        string utf8 = codepoint_to_utf8(cp);
        cout << "U+" << hex << uppercase << cp << dec << " : ";
        cout << bytes_to_hex(utf8);
        cout << " -> " << utf8 << "\n";
    }

    cout << "\nUTF-8 -> 数字 解码示例:\n";
    // 将上面生成的连成一个字符串，逐个解码
    string all;
    for (uint32_t cp : cps) all += codepoint_to_utf8(cp);
    size_t i = 0;
    while (i < all.size()) {
        uint32_t cp;
        size_t len;
        bool ok = utf8_decode_at(all, i, cp, len);
        if (!ok) {
            cout << "解码错误，位置 " << i << "\n";
            break;
        }
        string bytes = all.substr(i, len);
        cout << "bytes=" << bytes_to_hex(bytes) << " -> U+" << hex << uppercase << cp << dec << "\n";
        i += len;
    }

    return 0;
}
