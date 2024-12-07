#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>

#include <thread>
#include <atomic>
#include <chrono>


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

void thread_worker(long long calibration_val, std::vector<long long> vals, std::atomic_llong &out, std::atomic_int &counter)
{
    long long temp = num_works(calibration_val, vals) * calibration_val;
//    std::cout << temp << '\n';
    out.fetch_add(temp);
    counter.fetch_add(1);
}

int main()
{
    std::ifstream in("input.txt");
    std::string line;

    std::atomic_llong p2 = 0;
    std::atomic_int counter = 0;
    int line_count = 0;
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
        std::thread thread(thread_worker, calibration_val, vals, std::ref(p2), std::ref(counter));
        thread.detach();
        line_count++;
    }

    while (counter < line_count)
        std::this_thread::sleep_for(std::chrono::microseconds(1));

    std::cout << "p2: " << p2 << '\n';
}
