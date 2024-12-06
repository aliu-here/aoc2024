#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>

#include <thread> //cant be bothered to figure out a faster solution
#include <atomic>

enum directions {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

bool check_if_loop(std::vector<std::string> &grid, int x, int y, int dir)
{
    std::set<std::vector<int>> visited = {{x, y}};
    int steps_taken = 0, xcopy=x, ycopy = y;
    while (2 * visited.size() >= steps_taken){
        if (dir == UP)
            (x)--;
        if (dir == DOWN)
            (x)++;
        if (dir == LEFT)
            (y)--;
        if (dir == RIGHT)
            (y)++;
        if ((x < 0 || x >= grid.size()) || (y < 0 || y >= grid[x].size())) {
            return 0;
        }
        if (grid[x][y] == '#') {
            if (dir == UP) //backtrack
                (x)++;
            if (dir == DOWN)
                (x)--;
            if (dir == LEFT)
                (y)++;
            if (dir == RIGHT)
                (y)--;
            dir = (dir + 1) % 4; //stupidish way to do 90 deg rotation
            continue;
        }
        visited.insert({x, y});
        steps_taken++;
    }
    return 1;
}

void check_posns(std::vector<std::string> grid, int x, int y, int row, int dir, std::atomic_int &out, std::atomic_int &counter) {
    int calc_val = 0;
    for (int i=0; i<grid[row].size(); i++) {
        std::vector<std::string> copy = grid;
        copy[row][i] = '#';
        calc_val += check_if_loop(copy, x, y, dir);
    }

    out.fetch_add(calc_val);
    counter.fetch_add(1);
}

int main()
{
    std::ifstream in("input.txt");
    std::vector<std::string> grid;
    std::string line;
    while (getline(in, line)) {
        grid.push_back(line);
    }

    for (std::string row : grid) {
        std::cout << row.size() << '\n';
    }

    int dir = -1, x, y;
    std::atomic_int p2=0, counter=0;
    for (int i=0; i<grid.size(); i++) {
        for (int j=0; j<grid[i].size(); j++) {
            if (grid[i][j] == '^')
                dir = UP;
            if (grid[i][j] == '<')
                dir = LEFT;
            if (grid[i][j] == 'V')
                dir = DOWN;
            if (grid[i][j] == '>')
                dir = RIGHT;
            if (dir != -1) {
                x = i;
                y = j;
                grid[i][j] = 'X';
                break;
            }
        }
        if (dir != -1)
            break;
    }

    for (int i=0; i<grid.size(); i++) {
        std::thread temp(check_posns, grid, x, y, i, dir, std::ref(p2), std::ref(counter));
        temp.detach();
    }

    while (counter < grid.size()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::cout << counter << '\n';
    }

    std::cout << "p2: " << p2 << '\n';
}

