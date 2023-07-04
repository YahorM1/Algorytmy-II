/********************************
 * C++14
 *
 * 
 ********************************/


#include<iostream>
#include<vector>
#include<tuple>
#include<limits>
#include<set>

#include "HungarianClass/hungarian.h"


int main()
{
	//std::vector<mine_t> ores{
	//	mine_t {MineTypes::gold, {1.f, 5.f}},
	//	mine_t {MineTypes::gold, {4.f, 2.f}},
	//	mine_t {MineTypes::coal, {5.f, 3.f}},
	//	mine_t {MineTypes::coal, {7.f, 6.f}},
	//};

	//std::vector<dwarf_t> dwarfs{
	//	{{22.f, 73.f}, {{MineTypes::gold, 2.f}, {MineTypes::coal, 5.f}}},
	//	{{32.f, 18.f}, {{MineTypes::gold, 2.f}, {MineTypes::coal, 5.f}}},
	//	{{12.f, 45.f}, {{MineTypes::gold, 5.f}, {MineTypes::coal, 3.f}}},
	//	{{51.f, 37.f}, {{MineTypes::gold, 5.f}, {MineTypes::coal, 3.f}}},
	//};

	std::vector<mine_t> ores{
	mine_t {MineTypes::gold, {0.f, 0.f}},
	mine_t {MineTypes::gold, {3.f, 3.f}},
	mine_t {MineTypes::gold, {1.f, 4.f}},
	mine_t {MineTypes::coal, {2.f, 4.f}},
	mine_t {MineTypes::coal, {5.f, 5.f}},
	};

	std::vector<dwarf_t> dwarfs{
		{{1.f, 1.f}, {{MineTypes::gold, 5.f}, {MineTypes::coal, 2.f}}},
		{{2.f, 2.f}, {{MineTypes::gold, 3.f}, {MineTypes::coal, 5.f}}},
		{{2.f, 1.f}, {{MineTypes::gold, 5.f}, {MineTypes::coal, 2.f}}},

	};

	//float ex[3][3] = { {16,14,68}, {59,32,85}, {63,54,43} };
	//float ex[5][5] = { {78,65,74,32,4}, {93,94,47,7,48}, {72,42,66,11,23}, {17,94,11,59,47}, {63,13,6,6,67} };
	//Hungarian dwarfs_to_ores(reinterpret_cast<float*>(ex), 5);
	Hungarian dwarfs_to_ores(dwarfs, ores);

	std::cout << dwarfs_to_ores;
	dwarfs_to_ores.make_assignment();
	std::cout << dwarfs_to_ores;
	//std::cout << "Cost: " << cost << std::endl;
	//std::cout << "covering: " << dwarfs_to_ores.find_min_coverings() << std::endl;

	auto zeros = dwarfs_to_ores.get_zeros();

	Hungarian optima_distances(zeros, dwarfs, ores);
	optima_distances.make_assignment();
	std::cout << optima_distances;

}