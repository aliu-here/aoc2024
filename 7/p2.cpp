#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>


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
    return mul || add || concat;
}


int main()
{
    std::ifstream in("input.txt");
    std::string line;

    long long p2 = 0;
    int line_counter = 0;
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
