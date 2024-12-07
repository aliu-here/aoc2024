#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>

long long fastlogexp(long long num)
{
    long long val = 10;
    while (num >= val) {
        val *= 10;
    }
    return val;
}

long long deconcat(long long a, long long b)
{
    return (a - b) / fastlogexp(b);
}

bool can_concat(long long a, long long b)
{
    long long temp = a - b;
    long long temp_pow = fastlogexp(b);
    if (temp / temp_pow * temp_pow == temp)
        return true;
    return false;
}

//dfs it out
bool num_works(long long calibration_val, std::vector<long long> vals)
{
    if (vals.size() == 1)
        return calibration_val == vals[0];
    long long last_ele = vals[vals.size() - 1];
    bool out = false;
    vals.pop_back();
    if (can_concat(calibration_val, last_ele))
        out |= num_works(deconcat(calibration_val, last_ele), vals);
    if (calibration_val/last_ele * last_ele == calibration_val)
        out |= num_works(calibration_val / last_ele, vals);
    if (calibration_val >= last_ele)
        out |= num_works(calibration_val - last_ele, vals);
    return out;
}


int main()
{
    std::ifstream in("input.txt");
    std::string line;

    long long p2 = 0;
    while (getline(in, line)) {
        std::stringstream linestream(line);
        std::string calibration_val_str;
        linestream >> calibration_val_str;
        calibration_val_str = calibration_val_str.substr(0, calibration_val_str.size()-1);
        long long calibration_val = std::stol(calibration_val_str);

        long long temp;
        std::vector<long long> vals;
        while (linestream >> temp) {
            vals.push_back(temp);
        }
        p2 += num_works(calibration_val, vals) * calibration_val;
    }

    std::cout << "p2: " << p2 << '\n';
}
