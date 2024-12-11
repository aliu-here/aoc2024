#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <unordered_map>

int main()
{
    std::ifstream in("input.txt");
    int val;
    std::unordered_map<long long, long long> vals, newvals;
    while (in >> val)
        vals[val]++;
    for (int i=0; i<75; i++) {
        newvals.clear();
        for (auto &[num, temp] : vals) {
            if (num == 0) {
                newvals[1] += vals[num];
            } else if (std::to_string(num).size() % 2 == 0) {
                std::string numstring = std::to_string(num);
                int numstring_len = numstring.size();
                long long firsthalf = std::stol(numstring.substr(0, numstring_len / 2)),\
                     sechalf = std::stol(numstring.substr(numstring_len / 2, numstring_len / 2));
                newvals[firsthalf] += vals[num];
                newvals[sechalf] += vals[num];
            } else {
                newvals[num * 2024] += vals[num];
            }
        }
        vals = newvals;
    }
    unsigned long long p2 = 0;
    for (auto &[temp, count] : vals) {
        p2 += count;
    }
    std::cout << "p2: " << p2 << '\n';
}
