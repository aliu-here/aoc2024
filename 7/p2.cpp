#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>


long long deconcat(long long a, long long b)
{
    int temp_exp = std::log10(b) + 1;
    return (a - b) / std::pow(10, temp_exp);
}

bool can_concat(long long a, long long b)
{
    long long temp = a - b;
    int temp_exp = std::log10(b) + 1;
    long long temp_pow = std::pow(10, temp_exp);
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
//    std::cout << "calibration_val: " << calibration_val << " last_ele: " << last_ele << '\n';
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
