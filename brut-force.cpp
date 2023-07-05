#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <numeric>
#include <list>

//enum class MineTypes { gold = 1, copper, coal, iron, silver };
//enum class ZeroMark { notmarked = 0, starred, primed };
//
//struct mine_t
//{
//    MineTypes type;
//    std::pair<float, float> position;
//};
//
//struct dwarf_t {
//    std::pair<float, float> position;
//    std::unordered_map<MineTypes, float> skills;
//};

std::pair<std::vector<int>, std::vector<int>> brute_force(std::vector<std::vector<int>>& cost_matrix, bool maximize = false) {

    int h = cost_matrix[0].size();
    int w = cost_matrix[1].size();
    //std::cout << "h = " << h << std::endl;
    //std::cout << "w = " << w << std::endl;

    if (maximize) {
        std::vector<std::vector<int>> negated_cost_matrix = cost_matrix;
        for (auto& row : negated_cost_matrix) {
                for (auto& element : row) {
                    element = -element;
                }
        }
        cost_matrix = negated_cost_matrix;
    }

    int minimum_cost = std::numeric_limits<int>::max();
    std::vector<int> optimal_row_ind;
    std::vector<int> optimal_col_ind;

    if (h >= w) {
        std::vector<int> indices(h);
        std::iota(indices.begin(), indices.end(), 0);
        do {
            std::vector<int> row_ind(indices.begin(), indices.begin() + std::min(h, w));
            std::vector<int> col_ind(w);
            std::iota(col_ind.begin(), col_ind.end(), 0);
            int cost =0;
            for (int i = 0; i < row_ind.size(); i++) {
                cost += cost_matrix[row_ind[i]][col_ind[i]];
            }
            if (cost < minimum_cost) {
                minimum_cost = cost;
                optimal_row_ind = row_ind;
                optimal_col_ind = col_ind;
            }
        } while (std::next_permutation(indices.begin(), indices.end()));
    }
    if (h < w) {
        std::vector<int> indices(w);
        std::iota(indices.begin(), indices.end(), 0);
        do {
            std::vector<int> row_ind(h);
            std::iota(row_ind.begin(), row_ind.end(), 0);
            std::vector<int> col_ind(indices.begin(), indices.begin() + std::min(h, w));
            int cost = 0;
            for (int i = 0; i < row_ind.size(); i++) {
                cost += cost_matrix[row_ind[i]][col_ind[i]];
            }
            if (cost < minimum_cost) {
                minimum_cost = cost;
                optimal_row_ind = row_ind;
                optimal_col_ind = col_ind;
            }
        } while (std::next_permutation(indices.begin(), indices.end()));
    }

    return std::make_pair(optimal_row_ind, optimal_col_ind);
}

int main() {
//    std::vector<std::vector<int>> cost_matrix = {
//            {0, 0, 0, 3, 3, 6, 3, 3},
//            {2, 2, 2, 0, 0, 3, 5, 9},
//            {0, 0, 0, 3, 3, 4, 5, 5},
//            {5, 5, 5, 5, 5, 1, 1, 5},
//            {5, 5, 5, 5, 5, 0, 0, 3},
//            {1, 0, 8, 2, 2, 7, 4, 2},
//            {9, 1, 2, 3, 7, 8, 4, 4},
//            {4, 7, 2, 3, 3, 1, 0, 5}
//    };
    std::vector<std::vector<int>> cost_matrix = {
            {0, 0, 0, 3, 3},
            {2, 2, 2, 0, 0},
            {0, 0, 0, 3, 3},
            {5, 5, 5, 5, 5},
            {5, 5, 5, 5, 5},
    };

    std::pair<std::vector<int>, std::vector<int>> result = brute_force(cost_matrix);

    std::cout << "Result: " << std::endl;
    for(int i = 0; i < result.first.size(); i++){
        std::cout << "[" << result.first[i] << "," << result.second[i] << "]" << std::endl;
    }

    int min_cost = 0;
    for (int i = 0; i < result.first.size(); i++) {
        min_cost += cost_matrix[result.first[i]][result.second[i]];
    }
    std::cout << "min cost = " << min_cost << std::endl;
    return 0;
}
