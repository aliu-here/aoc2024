#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include <thread>
#include <atomic>
#include <chrono>

//dfs it out
bool num_works(long long calibration_val, std::vector<long long> vals)
{
    if (vals[0] > calibration_val)
        return 0;
    if (vals.size() == 1)
        return calibration_val == vals[0];
    bool mul, add, concat;
    std::vector<long long> newvec(vals.begin()+1, vals.end());

    newvec[0] = vals[0]+vals[1];
    if (num_works(calibration_val, newvec))
        return true;

    newvec[0] = vals[0]*vals[1];
    if (num_works(calibration_val, newvec))
        return true;

    newvec[0] = std::stol(std::to_string(vals[0]) + std::to_string(vals[1]));
    if (num_works(calibration_val, newvec))
        return true;
    return false;
}

void thread_worker(long long calibration_val, std::vector<long long> vals, std::atomic_llong &out, std::atomic_int &counter)
{
    long long temp = num_works(calibration_val, vals) * calibration_val;
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
